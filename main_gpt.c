#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/time.h>

#define BUFFER_SIZE 1024
#define TIMEOUT_SEC 1

void ping(const char* host);

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: %s <host>\n", argv[0]);
        exit(1);
    }
    ping(argv[1]);
    return 0;
}

unsigned short calculate_checksum(unsigned short *paddress, int len) {
    int nleft = len;
    int sum = 0;
    unsigned short *w = paddress;
    unsigned short answer = 0;

    while (nleft > 1) {
        sum += *w++;
        nleft -= 2;
    }

    if (nleft == 1) {
        *((unsigned char *)&answer) = *((unsigned char *)w);
        sum += answer;
    }

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer = ~sum;
    return answer;
}

void ping(const char* host) {
    struct sockaddr_in addr;
    int sock_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sock_fd < 0) {
        perror("socket() error");
        exit(-1);
    }

    struct hostent *host_entry;
    host_entry = gethostbyname(host);
    if (host_entry == NULL) {
        perror("gethostbyname() error");
        exit(-1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr = *((struct in_addr *) host_entry->h_addr_list[0]);

    struct icmp icmp_pkt;
    icmp_pkt.icmp_type = ICMP_ECHO;
    icmp_pkt.icmp_code = 0;
    icmp_pkt.icmp_id = htons(getpid());
    icmp_pkt.icmp_seq = htons(1);
    memset(&icmp_pkt.icmp_data, 0, sizeof(icmp_pkt.icmp_data));
    struct timeval tv;
    gettimeofday(&tv, NULL);
    *((struct timeval *) &icmp_pkt.icmp_data) = tv;
    icmp_pkt.icmp_cksum = calculate_checksum((unsigned short *) &icmp_pkt, sizeof(icmp_pkt));

    int bytes_sent;
    bytes_sent = sendto(sock_fd, &icmp_pkt, sizeof(icmp_pkt), 0, (struct sockaddr *) &addr, sizeof(addr));
    if (bytes_sent < 0) {
        perror("sendto() error");
        exit(-1);
    }

    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(sock_fd, &read_fds);

    struct timeval timeout;
    timeout.tv_sec = TIMEOUT_SEC;
    timeout.tv_usec = 0;

    int ready;
    ready = select(sock_fd + 1, &read_fds, NULL, NULL, &timeout);
    if (ready < 0) {
        perror("select() error");
        exit(-1);
    } else if (ready == 0) {
        printf("Timeout\n");
        return;
    }

    char buffer[BUFFER_SIZE];
    struct sockaddr_in recv_addr;
    socklen_t recv_addr_len = sizeof(recv_addr);

    int bytes_recv;
    bytes_recv = recvfrom(sock_fd,buffer, sizeof(buffer), 0, (struct sockaddr *) &recv_addr, &recv_addr_len);
	if (bytes_recv < 0) {
		perror("recvfrom() error");
		exit(-1);
	}

	struct iphdr *ip_hdr = (struct iphdr *) buffer;
	struct icmp *icmp_hdr = (struct icmp *) (buffer + (ip_hdr->ihl << 2));
	if (icmp_hdr->icmp_type == ICMP_ECHOREPLY && icmp_hdr->icmp_id == htons(getpid())) {
		struct timeval sent_time = *((struct timeval *) icmp_hdr->icmp_data);
		struct timeval curr_time, diff_time;
		gettimeofday(&curr_time, NULL);
		timersub(&curr_time, &sent_time, &diff_time);
		printf("Received ping response from %s: icmp_seq=%d ttl=%d time=%ld.%ldms\n",
			inet_ntoa(recv_addr.sin_addr), ntohs(icmp_hdr->icmp_seq), ip_hdr->ttl,
			diff_time.tv_sec * 1000 + diff_time.tv_usec / 1000, diff_time.tv_usec % 1000);
	}

	close(sock_fd);
}
