#include "../includes/main.h"


// Interrupt handler
static void	intHandler(int dummy)
{
	t_env	*env = st_env(NULL, false);
	
	env->pingloop = false;
}

static char	ft_ping(t_env *env, char *addr)
{
	char	code;

	// cloning env ptr in a singleton
	st_env(env, false);

	env->pingloop = true;
	env->addr = addr;

	// catching interrupt
	signal(SIGINT, intHandler);

	// dns + socket + request loop
	if ((code = ping_dns(env)) != 0
			|| (code = ping_socket(env)) != 0
			|| (code = ping_request(env)) != 0)
		return (code);
	return (0);
}

int		main(int argc, char **argv)
{
	t_env	env;
	char	code = -1;

	if (argc != 2)
	{
		printf("Usage: sudo ./%s <address>\n", argv[0]);
		return (EXIT_FAILURE);
	}
	ft_memset(&env, 0, sizeof(t_env));
	code = ft_ping(&env, argv[1]);
	return (ping_exit(&env, code));
}
