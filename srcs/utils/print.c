#include "main.h"


// print ping paquet stats
void		ping_stats_packet(void)
{
	t_env	*env = st_env(NULL, false);
	int		id;

	if (env) {
		if (env->verbose == true) {

			# ifdef __APPLE__
				id = env->pckt.hdr.id;
			# else
				id = env->pckt.hdr.un.echo.id;
			# endif

			printf("%d bytes from %s (%s): id=%d icmp_seq=%d ttl=%d time=%.2Lf ms\n",
				PING_PKT_S, env->ip_addr, env->reverse_hostname,
				id, env->msg_count,
				env->ttl_val, env->rtt.msec);
		} else {
			printf("%d bytes from %s: time=%.2Lf ms\n",
				PING_PKT_S, env->ip_addr, env->rtt.msec);
		}					
	}
}

// print total ping stats
void		ping_stats_total(void)
{
	t_env	*env = st_env(NULL, false);

	if (env) {
		printf("\n=== %s ping statistics ===\n", env->ip_addr);
		printf("%d packets transmitted, %d received, %.f%% packet loss, time %.2Lf ms\n",
			env->msg_count, env->msg_received_count,
			((env->msg_count - env->msg_received_count) / env->msg_count) * 100.0f,
			(env->rtt.tend_proc - env->rtt.tstart_proc) / 1000);
		if (env->msg_received_count > 0) {
			printf("rtt min/avg/max/sttdev = %.3Lf/%.3Lf/%.3Lf/%.3Lf ms\n",
				env->rtt.min, env->rtt.avg, env->rtt.max, env->rtt.sttdev);
		}
	}
}
