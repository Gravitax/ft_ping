#include "../../includes/main.h"


// filling packet
static char	packet_fill(t_env *env)
{
	int	i;

	bzero(&env->pckt, sizeof(env->pckt));
		
	env->pckt.hdr.type = ICMP_ECHO;
	env->pckt.hdr.un.echo.id = getpid();
		
	for (i = 0; i < sizeof(env->pckt.msg) - 1; i++)
		env->pckt.msg[i] = i + '0';
		
	env->pckt.msg[i] = 0;
	env->pckt.hdr.un.echo.sequence = env->msg_count++;
	env->pckt.hdr.checksum = ping_checksum(&env->pckt, sizeof(env->pckt));
	return (0);
}

// send packet
static char	packet_send(t_env *env)
{
	usleep(PING_SLEEP_RATE);
	clock_gettime(CLOCK_MONOTONIC, &env->time_start);
	if (sendto(env->sockfd, &env->pckt, sizeof(env->pckt), 0,
		(struct sockaddr *)&env->addr_con, sizeof(env->addr_con)) <= 0)
	{
		printf("Packet Sending Failed!\n");
		env->flag = 0;
	}
	return (0);
}

// receive packet
static char	packet_receive(t_env *env)
{
	env->addr_len = sizeof(env->r_addr);

	if (recvfrom(env->sockfd, &env->pckt, sizeof(env->pckt), 0, (struct sockaddr *)&env->r_addr, &env->addr_len) <= 0
		&& env->msg_count > 1)
	{
		printf("Packet receive failed!\n");
	}
	else
	{
		clock_gettime(CLOCK_MONOTONIC, &env->time_end);
			
		env->time_elapsed = ((double)(env->time_end.tv_nsec - env->time_start.tv_nsec)) / 1000000.0f;
		env->rtt_msec = (env->time_end.tv_sec - env->time_start.tv_sec) * 1000.0f + env->time_elapsed;
			
		// if packet was not sent, don't receive
		if (env->flag)
		{
			if (!(env->pckt.hdr.type == 69 && env->pckt.hdr.code == 0))
			{
				printf("Error... Packet received with ICMP type %d code %d\n", env->pckt.hdr.type, env->pckt.hdr.code);
			}
			else
			{
				printf("%d bytes from %s (h: %s) (%s): icmp_seq=8=%d ttl=%d time=%.2Lf ms.\n",
					PING_PKT_S, env->reverse_hostname, env->addr,
					env->ip_addr, env->msg_count,
					env->ttl_val, env->rtt_msec);

				env->msg_received_count++;
			}
		}
	}
	return (0);
}

// send icmp packet in an infinite loop
static char	ping_loop(t_env *env)
{
	char	code;

	while (env->pingloop)
	{
		// flag is whether packet was sent or not
		env->flag = 1;
		if ((code = packet_fill(env)) != 0
				|| (code = packet_send(env)) != 0
				|| (code = packet_receive(env)) != 0)
			return (code);
	}
	return (0);
}

// print total ping stats
static void	ping_stats(t_env *env)
{
	long double		total_msec;

	env->time_elapsed = ((double)(env->tfe.tv_nsec - env->tfs.tv_nsec)) / 1000000.0f;
	
	total_msec = (env->tfe.tv_sec - env->tfs.tv_sec) * 1000.0f + env->time_elapsed;
					
	printf("\n=== %s ping statistics ===\n", env->ip_addr);
	printf("%d packets transmitted, %d received, %f%% packet loss, time %.2Lf ms.\n\n",
		env->msg_count, env->msg_received_count,
		((env->msg_count - env->msg_received_count) / env->msg_count) * 100.0f, total_msec);
}

// make a ping request
char		ping_request(t_env *env)
{
	char	code;

	env->ttl_val = 64;
	env->tv_out.tv_sec = RECV_TIMEOUT;

	clock_gettime(CLOCK_MONOTONIC, &env->tfs);
	
	// set socket options at ip to TTL and value to 64,
	// change to what you want by setting ttl_val
	if (setsockopt(env->sockfd, SOL_IP, IP_TTL, &env->ttl_val, sizeof(env->ttl_val)) != 0)
		return (ERR_TTL);
	else
		printf("Socket set to TTL...\n");

	// setting timeout of recv setting
	setsockopt(env->sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&env->tv_out, sizeof(env->tv_out));

	if ((code = ping_loop(env)) != 0)
		return (code);

	clock_gettime(CLOCK_MONOTONIC, &env->tfe);

	ping_stats(env);
	return (0);
}
