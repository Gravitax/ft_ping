
#include "../../includes/main.h"


// Performs a DNS lookup
static char		*dns_lookup(char *addr_host, struct sockaddr_in *addr_con)
{
	struct hostent	*host_entity;
	char			*ip = (char *)malloc(NI_MAXHOST * sizeof(char));
	int				i;

	if (ip == NULL || (host_entity = gethostbyname(addr_host)) == NULL)
		return (NULL);
	
	//filling up address structure
	strcpy(ip, inet_ntoa(*(struct in_addr *)host_entity->h_addr));

	(*addr_con).sin_family = host_entity->h_addrtype;
	(*addr_con).sin_port = htons(PORT_NO);
	(*addr_con).sin_addr.s_addr = *(long *)host_entity->h_addr;

	return (ip);
}

// Resolves the reverse lookup of the hostname
static char		*reverse_dns_lookup(char *ip_addr)
{
	struct sockaddr_in	temp_addr;
	socklen_t			len;
	char				buf[NI_MAXHOST], *ret_buf;

	temp_addr.sin_family = AF_INET;
	temp_addr.sin_addr.s_addr = inet_addr(ip_addr);
	len = sizeof(struct sockaddr_in);

	ret_buf = (char *)malloc((strlen(buf) + 1) * sizeof(char));

	if (ret_buf == NULL || getnameinfo((struct sockaddr *) &temp_addr, len, buf, sizeof(buf), NULL, 0, NI_NAMEREQD))
		return (NULL);

	strcpy(ret_buf, buf);
	return (ret_buf);
}

char			ping_dns(t_env *env)
{
	printf("Resolving DNS...\n");
	// Performs a DNS lookup
	env->ip_addr = dns_lookup(env->addr, &env->addr_con);
	if (env->ip_addr == NULL)
	{
		printf("DNS lookup failed! Could not resolve hostname!\n");
		return (-1);
	}
	// Resolves the reverse lookup of the hostname
	env->reverse_hostname = reverse_dns_lookup(env->ip_addr);
	if (env->reverse_hostname == NULL)
	{
		printf("Could not resolve reverse lookup of hostname\n");
		return (-1);
	}
	printf("Trying to connect to '%s' IP: %s\n", env->addr, env->ip_addr);
	printf("Reverse Lookup domain: %s\n", env->reverse_hostname);
	return (0);
}
