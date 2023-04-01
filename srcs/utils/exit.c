#include "../../includes/main.h"


void			ping_help()
{
	printf("Usage:\n"
		"    ft_ping [options] <destination>\n"
		"\n"
		"Options:\n"
		"    <destination>		dns name or ip address\n"
		"    -v				verbose output\n"
		"    -h				print help and exit\n"
	);
}

static void		error_handler(t_env *env, int code)
{
	const char	*msg;

	// If code does not correspond to any existing code
	if (code >= ERR_MAX)
		return ;
	// Get the current error message
	msg = &errors[code].msg[0];
	// Display it
	if (errors[code].display == true)
		printf("ft_ping : error : %s\n", msg);
	// Display usage if needed
	if (errors[code].usage == true)
		ping_help();
}

int				ping_exit(t_env *env, int code)
{
	if (env->ip_addr)
		ft_strdel(&env->ip_addr);
	if (env->reverse_hostname)
		ft_strdel(&env->reverse_hostname);
	if (env->sockfd)
		close(env->sockfd);
	// put the singleton at NULL
	st_env(NULL, true);
	// display error message if there is one
	error_handler(env, code);
	return (code == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
