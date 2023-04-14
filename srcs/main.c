#include "../includes/main.h"


static int	ping_args(t_env *env, int argc, char **argv)
{
	int		opt, i;

	// turn off getopt's error
	opterr = 0;
	for (i = 0; i < argc - 1; i++)
	{
		// we use opt_long to handle args before and after the addr
		opt = getopt_long(argc, argv, "vh", NULL, NULL);
		// if opt == -1 we assume its the addr
		if (opt == -1) {
			env->addr = argv[i + 1];
			break ;
		}
		switch (opt) {
			case 'v': // verbose mode
				env->verbose = true;
				break ;
			case '?': // illegal usage
				return (ERR_ARGS);
			case 'h': default: // usage
				ping_help();
				return (ERR_NONE);
		}
	}
	return (env->addr == NULL ? ERR_ARGS : ERR_NONE);
}

// Interrupt handler
static void	sigint_handler(int dummy)
{
	t_env	*env = st_env(NULL, false);
	
	env->pingloop = false;
}

static int	ft_ping(t_env *env, int argc, char **argv)
{
	int	code;

	// parsing arguments || if addr is NULL then its usage or error
	if ((code = ping_args(env, argc, argv)) != ERR_NONE
			|| env->addr == NULL)
		return (code);

	// cloning env ptr in a singleton
	st_env(env, false);

	env->pingloop = true;
	env->ttl_val = PING_TTL;

	// catching interrupt
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigint_handler);

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
	int		code;

	// not enough args
	if (argc < 2) {
		ping_help();
		return (EXIT_FAILURE);
	}
	// no sudo
	if (getuid() != 0) {
		ping_exit(NULL, ERR_SUDO);
		return (EXIT_FAILURE);
	}
	ft_memset(&env, 0, sizeof(t_env));
	code = ft_ping(&env, argc, argv);
	return (ping_exit(&env, code));
}
