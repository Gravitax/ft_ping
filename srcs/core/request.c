#include "../../includes/main.h"


static char	ping_loop(t_env *env)
{
	int		i, addr_len, flag = 1;

	struct timespec		time_start, time_end;
	struct ping_pkt		pckt;
	struct sockaddr_in	r_addr;

	long double			rtt_msec = 0;

	// send icmp packet in an infinite loop
	while (env->pingloop)
	{
		// flag is whether packet was sent or not
		flag = 1;
	
		//filling packet
		bzero(&pckt, sizeof(pckt));
		
		pckt.hdr.type = ICMP_ECHO;
		pckt.hdr.un.echo.id = getpid();
		
		for (i = 0; i < sizeof(pckt.msg) - 1; i++)
			pckt.msg[i] = i + '0';
		
		pckt.msg[i] = 0;
		pckt.hdr.un.echo.sequence = env->msg_count++;
		pckt.hdr.checksum = ping_checksum(&pckt, sizeof(pckt));

		usleep(PING_SLEEP_RATE);

		//send packet
		clock_gettime(CLOCK_MONOTONIC, &time_start);
		if (sendto(env->sockfd, &pckt, sizeof(pckt), 0, (struct sockaddr *)&env->addr_con, sizeof(env->addr_con)) <= 0)
		{
			printf("Packet Sending Failed!\n");
			flag = 0;
		}

		//receive packet
		addr_len = sizeof(r_addr);

		if (recvfrom(env->sockfd, &pckt, sizeof(pckt), 0, (struct sockaddr *)&r_addr, &addr_len) <= 0 && env->msg_count > 1)
		{
			printf("Packet receive failed!\n");
		}
		else
		{
			clock_gettime(CLOCK_MONOTONIC, &time_end);
			
			double timeElapsed = ((double)(time_end.tv_nsec - time_start.tv_nsec)) / 1000000.0f;
			rtt_msec = (time_end.tv_sec - time_start.tv_sec) * 1000.0f + timeElapsed;
			
			// if packet was not sent, don't receive
			if (flag)
			{
				if (!(pckt.hdr.type == 69 && pckt.hdr.code == 0))
				{
					printf("Error..Packet received with ICMP type %d code %d\n", pckt.hdr.type, pckt.hdr.code);
				}
				else
				{
					printf("%d bytes from %s (h: %s) (%s) msg_seq=%d ttl=%d rtt=%Lf ms.\n",
						PING_PKT_S, env->reverse_hostname, env->addr,
						env->ip_addr, env->msg_count,
						env->ttl_val, rtt_msec);

					env->msg_received_count++;
				}
			}
		}
	}
}

// make a ping request
char		ping_request(t_env *env)
{
	env->ttl_val = 64;
	env->msg_count = 0;
	env->msg_received_count = 0;

	struct timespec		tfs, tfe;
	long double			total_msec = 0;
	struct timeval		tv_out;

	tv_out.tv_sec = RECV_TIMEOUT;
	tv_out.tv_usec = 0;

	clock_gettime(CLOCK_MONOTONIC, &tfs);
	
	// set socket options at ip to TTL and value to 64,
	// change to what you want by setting ttl_val
	if (setsockopt(env->sockfd, SOL_IP, IP_TTL, &env->ttl_val, sizeof(env->ttl_val)) != 0)
	{
		printf("Setting socket options to TTL failed!\n");
		return (0);
	}
	else
	{
		printf("Socket set to TTL...\n");
	}

	// setting timeout of recv setting
	setsockopt(env->sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv_out, sizeof(tv_out));


	ping_loop(env);


	clock_gettime(CLOCK_MONOTONIC, &tfe);
	double timeElapsed = ((double)(tfe.tv_nsec - tfs.tv_nsec)) / 1000000.0f;
	
	total_msec = (tfe.tv_sec - tfs.tv_sec) * 1000.0f + timeElapsed;
					
	printf("\n=== %s ping statistics ===\n", env->ip_addr);
	printf("%d packets sent, %d packets received, %f percent packet loss. Total time: %Lf ms.\n\n",
		env->msg_count, env->msg_received_count,
		((env->msg_count - env->msg_received_count) / env->msg_count) * 100.0f, total_msec);

	return (0);
}
