#include "../../includes/main.h"


// filling packet
static int	packet_fill(t_env *env)
{
	int	i;

	bzero(&env->pckt, sizeof(env->pckt));

	# ifdef __APPLE__
		env->pckt.hdr.id = getpid();
		env->pckt.hdr.sequence = env->msg_count++;
	# else
		env->pckt.hdr.un.echo.id = getpid();
		env->pckt.hdr.un.echo.sequence = env->msg_count++;
	# endif

	env->pckt.hdr.type = ICMP_ECHO;

	for (i = 0; i < sizeof(env->pckt.msg) - 1; i++)
		env->pckt.msg[i] = i + '0';
	env->pckt.msg[i] = 0;

	env->pckt.hdr.checksum = ping_checksum(&env->pckt, sizeof(env->pckt));
	return (ERR_NONE);
}

// send packet
static int	packet_send(t_env *env)
{
	ft_usleep(PING_SLEEP_RATE);
	env->rtt.time_start = get_time_now();

	if (sendto(env->sockfd, &env->pckt, sizeof(env->pckt), 0,
		(struct sockaddr *)&env->addr_con, sizeof(env->addr_con)) <= 0)
	{
		printf("Packet Sending Failed!\n");
		env->flag = 0;
	}
	return (ERR_NONE);
}

static void	update_time(t_env *env)
{
	env->rtt.time_end = get_time_now();
	env->rtt.msec = (env->rtt.time_end - env->rtt.time_start) / 1000;
	env->rtt.total_msec += env->rtt.msec;

	env->rtt.curr = env->rtt.msec;
	if (env->rtt.min == 0)
		env->rtt.min = env->rtt.curr;
	env->rtt.min = env->rtt.msec < env->rtt.min ? env->rtt.msec : env->rtt.min;
	env->rtt.max = env->rtt.msec > env->rtt.max ? env->rtt.msec : env->rtt.max;
}

// receive packet
static int	packet_receive(t_env *env)
{
	env->addr_len = sizeof(env->r_addr);

	if (recvfrom(env->sockfd, &env->pckt, sizeof(env->pckt), 0, (struct sockaddr *)&env->r_addr, &env->addr_len) <= 0
		&& env->msg_count > 1)
	{
		printf("Packet receive failed!\n");
		env->flag = 0;
	}

	update_time(env);

	// if packet was not sent, don't receive
	if (env->flag) {
		if (!(env->pckt.hdr.type == 69 && env->pckt.hdr.code == 0))
		{
			printf("Error... Packet received with ICMP type %d code %d\n", env->pckt.hdr.type, env->pckt.hdr.code);
		}
		else
		{
			ping_stats_packet();
			env->msg_received_count++;
		}
	}
	return (ERR_NONE);
}

// send icmp packet in an infinite loop
static int	ping_loop(t_env *env)
{
	int	code;

	printf("PING %s (%s) %d bytes of data\n",
		env->ip_addr, env->reverse_hostname, PING_PKT_S - 8);
	env->rtt.tstart_proc = get_time_now();
	while (env->pingloop)
	{
		// flag is whether packet was sent or not
		env->flag = 1;
		if ((code = packet_fill(env)) != ERR_NONE
				|| (code = packet_send(env)) != ERR_NONE
				|| (code = packet_receive(env)) != ERR_NONE)
			return (code);
	}
	env->rtt.tend_proc = get_time_now();
	env->rtt.avg = env->rtt.total_msec / env->msg_received_count;
	env->rtt.sttdev = (env->rtt.max - env->rtt.min) / 2;
	return (ERR_NONE);
}

// make a ping request
int			ping_request(t_env *env)
{
	int	code;

	if ((code = ping_loop(env)) != ERR_NONE)
		return (code);

	ping_stats_total();
	return (ERR_NONE);
}
