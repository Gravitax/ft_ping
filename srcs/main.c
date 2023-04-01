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

	// il faut checks i cest un ipv4 ou non
	env->addr = addr;

	printf("addr : %s\n", addr);

	env->ttl_val = 64;

	// catching interrupt
	signal(SIGINT, int_handler);

	// dns + socket + request loop
	if ((code = ping_dns(env)) != 0
			|| (code = ping_socket(env)) != 0
			|| (code = ping_request(env)) != 0)
		return (code);
	return (0);
}

int			main(int argc, char **argv)
{
	t_env	env;
	int		code, opt;

	// désactiver les messages d'erreur de getopt
	opterr = 0;

	ft_memset(&env, 0, sizeof(t_env));

	while ((opt = getopt(argc, argv, "v:h"))) {
		if (argc == 2 && opt == -1)
			break ;
		switch (opt) {
			case '?': case -1:
				code = ERR_ARGS;
				return (ping_exit(&env, code));
			case 'v':
				env.verbose = true;
			case 'h':
				ping_help();
				return (EXIT_SUCCESS);
		}
	}
	code = ft_ping(&env, argv[argc - 1]);
	return (ping_exit(&env, code));
}
