#include "../includes/main.h"


// Interrupt handler
static void	int_handler(int dummy)
{
	t_env	*env = st_env(NULL, false);
	
	env->pingloop = false;
}

static bool	is_ipv4_addr(char *addr)
{
	int		len = strlen(addr);

	return (len > 6 && len < 12 ? true : false);
}

static int	ft_ping(t_env *env, char *addr)
{
	int	code;

	// cloning env ptr in a singleton
	st_env(env, false);

	env->pingloop = true;

	if (is_ipv4_addr(addr) == false)
		return (ERR_IPV4);

	env->addr = addr;
	env->ttl_val = 64;

	printf("addr : %s, len: %ld, ttl: %d\n", env->addr, strlen(env->addr), env->ttl_val);

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

	ft_memset(&env, 0, sizeof(t_env));

	int		i = 0;

	// désactiver les messages d'erreur de getopt
	opterr = 0;
	while ((opt = getopt(argc, argv, "v:h"))) {
		if (argc == 2 && opt == -1 || ++i > argc - 1)
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
