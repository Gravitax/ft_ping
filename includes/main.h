#ifndef MAIN_H
# define MAIN_H


# include <stdio.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <unistd.h>
# include <string.h>
# include <stdlib.h>
# include <netinet/ip_icmp.h>
# include <time.h>
# include <fcntl.h>
# include <signal.h>
# include <time.h>

# include <stdbool.h>

# include "../libs/libft/libft.h"

// Define the Packet Constants
// ping packet size
# define PING_PKT_S 64

// Automatic port number
# define PORT_NO 0

// Automatic port number
// #define PING_SLEEP_RATE 1000000 x
# define PING_SLEEP_RATE 1000000 

// Gives the timeout delay for receiving packets
// in seconds
# define RECV_TIMEOUT 1


// ping packet structure
struct	ping_pkt
{
	struct icmphdr	hdr;
	char			msg[PING_PKT_S-sizeof(struct icmphdr)];
};

typedef struct	s_env
{
	bool				pingloop;
	int					sockfd;
	char				*addr;
	char				*ip_addr, *reverse_hostname;
	struct sockaddr_in	addr_con;
}				t_env;

// core
char			ping_dns(t_env *env);
void			ping_request(int ping_sockfd, struct sockaddr_in *ping_addr, char *ping_dom, char *ping_ip, char *rev_host);

// utils
unsigned short	ping_checksum(void *b, int len);
t_env			*st_env(t_env *env, bool unsave);


#endif
