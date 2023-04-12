#include "../../includes/main.h"


// save env as static
t_env	*st_env(t_env *env, bool unsave)
{
	static t_env	*save = NULL;

	if (save == NULL && env)
		save = env;
	if (unsave == true)
		save = NULL;
	return (save);
}
