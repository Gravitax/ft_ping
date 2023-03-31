#include "../../includes/main.h"


static void		error_handler(t_env *env, char code)
{
	const char	*msg;

	// If code does not correspond to any existing code
	if (code >= ERR_MAX)
		return ;
	// Get the current error message
	msg = &errors[code].msg[0];
	// Display it
	if (errors[code].msg[1])
		printf("ft_ping : error : %s\n", msg);
}

int				ping_exit(t_env *env, char code)
{
	if (env->ip_addr)
		ft_strdel(&env->ip_addr);
	if (env->reverse_hostname)
		ft_strdel(&env->reverse_hostname);
	if (env->sockfd)
		close(env->sockfd);
	error_handler(env, code);
	return (code == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
