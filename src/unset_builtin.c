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

int	unset_builtin(char **env, char **vars)
{
	char	**new_env;
	size_t	new_index;
	size_t	i;
	size_t	j;
	size_t	var_len;
	int		should_free;

	if (!vars[0])
		return (1);
	new_env = malloc((count_env_vars(env) + 1) * sizeof(char *));
	if (!new_env)
		return (1);
	new_index = 0;
	i = 0;
	while (env[i])
	{
		should_free = 0;
		j = 0;
		while (vars[j])
		{
			var_len = ft_strlen(vars[j]);
			if (ft_strncmp(env[i], vars[j], var_len) == 0
				&& env[i][var_len] == '=')
			{
				free(env[i]);
				should_free = 1;
				break ;
			}
			j++;
		}
		if (!should_free)
			new_env[new_index++] = env[i];
		i++;
	}
	new_env[new_index] = NULL;
	i = 0;
	while (i < new_index)
	{
		env[i] = new_env[i];
		i++;
	}
	env[new_index] = NULL;
	free(new_env);
	return (0);
}
