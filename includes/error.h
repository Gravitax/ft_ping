
#ifndef ERROR_H
# define ERROR_H


# include <stdbool.h>

enum					e_error
{
	ERR_NONE,
	ERR_DNS,
	ERR_RDNS,
	ERR_SOCKET,
	ERR_TTL,
	ERR_MAX
};

typedef	struct			s_error
{
	char	msg[64];
	bool	display;
}						t_error;

static const t_error	errors[ERR_MAX] = {
	[ERR_DNS] = {"DNS lookup failed! Could not resolve hostname!", true},
	[ERR_RDNS] = {"Could not resolve reverse lookup of hostname", true},
	[ERR_SOCKET] = {"Socket file descriptor not received!", true},
	[ERR_TTL] = {"Setting socket options to TTL failed!", true},
};


#endif
