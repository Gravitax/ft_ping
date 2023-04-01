#include "../../includes/main.h"


int		ping_socket(t_env *env)
{
	// check strace ping 8.8.8.8 pour avoir plus dinfos sur les flags
	env->sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (env->sockfd < 0) {
		return (ERR_SOCKET);
	}
	printf("Socket file descriptor %d received\n", env->sockfd);
	return (ERR_NONE);
}

