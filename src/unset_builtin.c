#include <minishell.h>

int	unset_builtin(char ***env, char **vars)
{
	char	**new_env;
	size_t	new_index;
	size_t	env_size;

	if (!vars[0])
		return (1);
	env_size = count_env_vars(*env);
	new_env = malloc((env_size + 1) * sizeof(char *));
	if (!new_env)
		return (1);
	new_index = populate_new_env(new_env, *env, vars);
	new_env[new_index] = NULL;
	free(*env);
	*env = new_env;
	return (0);
}
