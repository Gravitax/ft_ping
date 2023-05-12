#include "main.h"


int		ping_socket(t_env *env)
{
	int	code;

	# ifdef __APPLE__
		code = IPPROTO_IP;
	# else
		code = SOL_IP;
	# endif

	// check strace ping 8.8.8.8 pour avoir plus dinfos sur les flags
	env->sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (env->sockfd < 0)
		return (ERR_SOCKET);

	if (setsockopt(env->sockfd, code, IP_TTL, &env->ttl_val, sizeof(env->ttl_val)) != ERR_NONE)
		return (ERR_TTL);

	// setting timeout of recv setting
	setsockopt(env->sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&env->tv_out, sizeof(env->tv_out));

	return (ERR_NONE);
}
