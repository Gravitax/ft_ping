#include "../includes/main.h"


// Interrupt handler
static void	int_handler(int dummy)
{
	t_env	*env = st_env(NULL, false);
	
	env->pingloop = false;
}

static int	ft_ping(t_env *env, char *addr)
{
	int	code;

	// cloning env ptr in a singleton
	st_env(env, false);

	env->pingloop = true;
	env->addr = addr;
	env->ttl_val = PING_TTL;

	// catching interrupt
	signal(SIGINT, int_handler);

	// dns + socket + request loop
	if ((code = ping_dns(env)) != ERR_NONE
			|| (code = ping_socket(env)) != ERR_NONE
			|| (code = ping_request(env)) != ERR_NONE)
		return (code);
	return (ERR_NONE);
}

int			main(int argc, char **argv)
{
	t_env	env;
	int		code, opt;

	if (argc < 2) {
		ping_help();
		return (EXIT_SUCCESS);
	}
	// if (getuid() != 0) {
	// 	ping_exit(NULL, ERR_SUDO);
	// 	return (EXIT_FAILURE);
	// }

	ft_memset(&env, 0, sizeof(t_env));

	// désactiver les messages d'erreur de getopt
	opterr = 0;
	for (int i = 0; i < argc; i++)
	{
		opt = getopt(argc, argv, "v:h");
		if (argc == 2 && opt == -1)
			break ;
		switch (opt) {
			case '?':
				code = ERR_ARGS;
				return (ping_exit(&env, code));
			case 'v':
				env.verbose = true;
				break ;
			case 'h':
				ping_help();
				return (EXIT_SUCCESS);
		}
	}
	code = ft_ping(&env, argv[argc - 1]);
	return (ping_exit(&env, code));
}
