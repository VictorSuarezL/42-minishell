#include <minishell.h>

void	free_double(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		free(str[i++]);
	free(str);
}

size_t	count_env_vars(char **env)
{
	size_t	count;

	count = 0;
	while (env[count])
		count++;
	return (count);
}

char	**copy_env(char **env)
{
	size_t	count;
	char	**copy;

	count = count_env_vars(env);
	copy = malloc((count + 1) * sizeof(char *));
	if (!copy)
		return (NULL);
	count = 0;
	while (env[count])
	{
		copy[count] = ft_strdup(env[count]);
		if (!copy[count])
			return (free_double(copy), NULL);
		count++;
	}
	copy[count] = NULL;
	return (copy);
}

int	unset_builtin(char **env, char *var)
{
	char	**new_env;
	size_t	new_index;
	size_t	i;

	if (!var)
		return (-1);
	new_env = malloc((count_env_vars(env) + 1) * sizeof(char *));
	if (!new_env)
		return (-1);
	new_index = 0;
	i = -1;
	while (env[++i])
	{
		if (ft_strncmp(env[i], var, ft_strlen(var)) == 0)
			free(env[i]);
		else
			new_env[new_index++] = env[i];
	}
	new_env[new_index] = NULL;
	i = -1;
	while (++i < new_index)
		env[i] = new_env[i];
	env[new_index] = NULL;
	return (free(new_env), 0);
}
