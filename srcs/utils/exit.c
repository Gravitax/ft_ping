#include "main.h"


// display usage
void			ping_help(void)
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

// display error message
static void		error_handler(int code)
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

// free env + handle error + exit code
int				ping_exit(t_env *env, int code)
{
	if (env) {
		if (env->ip_addr)
			ft_strdel(&env->ip_addr);
		if (env->reverse_hostname)
			ft_strdel(&env->reverse_hostname);
		if (env->sockfd)
			close(env->sockfd);
		// put the singleton at NULL
		st_env(NULL, true);
	}
	// display error message if there is one
	if (code > ERR_NONE)
		error_handler(code);
	return (code == ERR_NONE ? EXIT_SUCCESS : EXIT_FAILURE);
}
