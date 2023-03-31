#include "../../includes/main.h"


int		ping_exit(t_env *env, char code)
{
	if (env->ip_addr)
		ft_strdel(&env->ip_addr);
	if (env->reverse_hostname)
		ft_strdel(&env->reverse_hostname);
	if (env->sockfd)
		close(env->sockfd);
	return (code == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
