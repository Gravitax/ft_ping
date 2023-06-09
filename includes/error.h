
#ifndef ERROR_H
# define ERROR_H


# include <stdbool.h>

enum					e_error
{
	ERR_NONE = 0,
	ERR_ARGS,
	ERR_SUDO,
	ERR_DNS,
	ERR_RDNS,
	ERR_SOCKET,
	ERR_TTL,
	ERR_MAX
};

typedef	struct			s_error
{
	char	msg[256];
	bool	display;
	bool	usage;
}						t_error;

static const t_error	errors[ERR_MAX] = {
	[ERR_ARGS] = { "Invalid usage", true, true },
	[ERR_SUDO] = { "You need SUDO privileges", true, true },
	[ERR_DNS] = { "DNS lookup failed! Could not resolve hostname!", true, false },
	[ERR_RDNS] = { "RDNS lookup failed! Could not resolve reverse lookup of hostname", true, false },
	[ERR_SOCKET] = { "Socket file descriptor not received!", true, false },
	[ERR_TTL] = { "Setting socket options to TTL failed!", true, false }
};


#endif
