#include "../includes/main.h"

// C program to Implement Ping

// compile with > make
// run as sudo ./ft_ping <hostname>

// Interrupt handler
void	intHandler(int dummy)
{
	t_env	*env = st_env(NULL, false);
	
	env->pingloop = false;
}

char	ft_ping(t_env *env, char *addr)
{
	char	code;

	env->pingloop = true;
	env->addr = addr;

	// dns
	if ((code = ping_dns(env)) != 0)
		return (code);

	// socket
	// check strace ping 8.8.8.8 pour avoir plus dinfos sur les flags
	env->sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (env->sockfd < 0)
	{
		printf("Socket file descriptor not received!!\n");
		return (-1);
	}
	else
		printf("Socket file descriptor %d received\n", env->sockfd);

	// catching interrupt
	signal(SIGINT, intHandler);

	// send pings continuously
	if ((code = ping_request(env->sockfd, &env->addr_con, env->reverse_hostname, env->ip_addr, env->addr)) != 0)
		return (code);

	// saving the env in a global singleton
	st_env(env, false);
	return (0);
}

int		main(int argc, char **argv)
{
	t_env	env;
	char	code = -1;

	if (argc != 2)
	{
		printf("Usage: %s <address>\n", argv[0]);
		return (0);
	}
	ft_memset(&env, 0, sizeof(t_env));
	code = ft_ping(&env, argv[1]);
	return (code == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
