
#include "main.h"


// Performs a DNS lookup
static char		*dns_lookup(char *addr_host, struct sockaddr_in *addr_con)
{
	struct hostent	*host_entity;
	char			*ip = (char *)malloc(NI_MAXHOST * sizeof(char));

	if (ip == NULL)
		return (NULL);
	if ((host_entity = gethostbyname(addr_host)) == NULL) {
		ft_strdel(&ip);
		return (NULL);
	}
	
	// filling up address structure
	strcpy(ip, inet_ntoa(*(struct in_addr *)host_entity->h_addr));

	(*addr_con).sin_family = host_entity->h_addrtype;
	(*addr_con).sin_port = htons(PORT_NO);
	(*addr_con).sin_addr.s_addr = *(long *)host_entity->h_addr;
	return (ip);
}

// Resolves the reverse lookup of the hostname
static char		*reverse_dns_lookup(char *ip_addr)
{
	struct sockaddr_in	tmp_addr;
	char				host[NI_MAXHOST], *ret_buffer;

	tmp_addr.sin_family = AF_INET;
	tmp_addr.sin_addr.s_addr = inet_addr(ip_addr);

	ret_buffer = (char *)malloc((strlen(host) + 1) * sizeof(char));

	if (ret_buffer == NULL)
		return (NULL);
	if (getnameinfo((struct sockaddr *)&tmp_addr, sizeof(struct sockaddr_in), host, NI_MAXHOST, NULL, 0, NI_NAMEREQD)) {
		ft_strdel(&ret_buffer);
		return (NULL);
	}
	// filling up ret buffer
	strcpy(ret_buffer, host);
	return (ret_buffer);
}

int			ping_dns(t_env *env)
{
	// Performs a DNS lookup
	env->ip_addr = dns_lookup(env->addr, &env->addr_con);
	if (env->ip_addr == NULL)
		return (ERR_DNS);
	// Resolves the reverse lookup of the hostname
	env->reverse_hostname = reverse_dns_lookup(env->ip_addr);
	// because of test 42.fr if we got an ip addr and no reverse hostname we are ok
	return (env->ip_addr == NULL && env->reverse_hostname == NULL ? ERR_RDNS : ERR_NONE);
}
