#ifndef ICMP_H
# define ICMP_H


# include <stdint.h>


struct	icmphdr
{
	u_int8_t	type;
	u_int8_t	code;
	u_int16_t	checksum;
	u_int16_t	id;
	u_int16_t	sequence;
};

struct iphdr {
	u_int8_t	ihl:4;
	u_int8_t	version:4;
	u_int8_t	tos;
	u_int16_t	tot_len;
	u_int16_t	id;
	u_int16_t	frag_off;
	u_int8_t	ttl;
	u_int8_t	protocol;
	u_int16_t	check;
	u_int32_t	saddr;
	u_int32_t	daddr;
};


#endif
