#ifndef MAIN_H
# define MAIN_H


# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <stdlib.h>
# include <stdbool.h>

# include <sys/types.h>
# include <sys/socket.h>

# include <arpa/inet.h>

# include <netinet/in.h>
# include <netinet/ip.h>
# include <netinet/ip_icmp.h>

# include <netdb.h>
# include <time.h>
# include <fcntl.h>

# include <signal.h>

#include <getopt.h>

# ifdef __APPLE__
	#include "./icmp.h"
# endif


# include "../libs/libft/libft.h"
# include "./error.h"

// Define the Packet Constants
// ping packet size
# define PING_PKT_S 64

// Automatic port number
# define PORT_NO 0

# define PING_TTL			64
# define PING_SLEEP_RATE	1000000 

// Gives the timeout delay for receiving packets
// in seconds
# define RECV_TIMEOUT 1

// ping packet structure
struct			ping_pkt
{
	struct icmphdr	hdr;
	char			msg[PING_PKT_S - sizeof(struct icmphdr)];
};

typedef struct	s_env
{
	bool				verbose, pingloop;
	int					sockfd, flag;
	int					ttl_val, msg_count, msg_received_count;
	unsigned int		addr_len;
	char				*addr, *ip_addr, *reverse_hostname;
	struct sockaddr_in	addr_con, r_addr;
	struct ping_pkt		pckt;
	struct timeval		tv_out;
	struct timespec		time_start, time_end, tfs, tfe;
	double				time_elapsed;
	long double			rtt_msec, total_msec;
}				t_env;

// core
int				ping_dns(t_env *env);
int				ping_request(t_env *env);
int				ping_socket(t_env *env);

// utils
unsigned short	ping_checksum(void *b, int len);
void			ping_help();
int				ping_exit(t_env *env, int code);
t_env			*st_env(t_env *env, bool unsave);


#endif
