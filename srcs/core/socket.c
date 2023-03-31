#include "../../includes/main.h"


char		ping_socket(t_env *env)
{
	// check strace ping 8.8.8.8 pour avoir plus dinfos sur les flags
	env->sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (env->sockfd < 0)
	{
		printf("Socket file descriptor not received!!\n");
		return (-1);
	}
	else
		printf("Socket file descriptor %d received\n", env->sockfd);
	return (0);
}

