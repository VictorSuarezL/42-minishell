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

int should_remove_env_var(char *env_var, char **vars)
{
    size_t var_len;
    size_t j;

	j = 0;
    while (vars[j])
	{
        var_len = ft_strlen(vars[j]);
        if ((ft_strncmp(env_var, vars[j], var_len) == 0) &&
            (env_var[var_len] == '=' || env_var[var_len] == '\0'))
		{
            return (1);
        }
        j++;
    }

    return (0);
}

size_t populate_new_env(char **new_env, char **old_env, char **vars)
{
    size_t i;
    size_t new_index;

	i = 0;
	new_index = 0;
    while (old_env[i])
	{
        if (should_remove_env_var(old_env[i], vars))
            free(old_env[i]);
        else
            new_env[new_index++] = old_env[i];
        i++;
    }
    return (new_index);
}


int unset_builtin(char ***env, char **vars)
{
    char **new_env;
    size_t new_index;
    size_t env_size;

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
