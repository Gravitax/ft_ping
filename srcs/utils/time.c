#include "main.h"


unsigned long	get_time_now(void)
{
	struct timeval	timestamp;

	gettimeofday(&timestamp, NULL);
	return ((unsigned long)timestamp.tv_sec * 1000000 + (unsigned long)timestamp.tv_usec);
}

void			ft_usleep(unsigned int duration)
{
	unsigned long	start = get_time_now();

	while (get_time_now() - start < (unsigned long)duration)
		;
}
