#include <minishell.h>

char	*get_env_value(const char *name, char **envp)
{
	size_t	name_len;
	int		i;

	i = 0;
	name_len = ft_strlen(name);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], name, name_len) == 0
			&& envp[i][name_len] == '=')
			return (ft_strdup(envp[i] + name_len + 1));
		i++;
	}
	return (NULL);
}

int	find_env_var(char **env, const char *var)
{
	size_t	var_len;
	int		i;

	var_len = ft_strlen(var);
	i = 0;
	while (env[i] != NULL)
	{
		if (ft_strncmp(env[i], var, var_len) == 0 && env[i][var_len] == '=')
			break ;
		i++;
	}
	return (i);
}

void	allocate_new_env_var(char **env, int i, const char *var,
		const char *value)
{
	size_t	var_len;
	size_t	value_len;

	var_len = ft_strlen(var);
	value_len = ft_strlen(value);
	env[i] = malloc(var_len + value_len + 2);
	if (env[i] == NULL)
	{
		perror("malloc");
		return ;
	}
	ft_strcpy(env[i], var);
	env[i][var_len] = '=';
	ft_strcpy(env[i] + var_len + 1, value);
	env[i + 1] = NULL;
}

void	free_and_allocate_env_var(char **env, int i, const char *var,
		const char *value)
{
	size_t	var_len;
	size_t	value_len;

	var_len = ft_strlen(var);
	value_len = ft_strlen(value);
	free(env[i]);
	env[i] = malloc(var_len + value_len + 2);
	if (env[i] == NULL)
	{
		perror("malloc");
		return ;
	}
	ft_strcpy(env[i], var);
	env[i][var_len] = '=';
	ft_strcpy(env[i] + var_len + 1, value);
}

void	update_existing_env_var(char **env, int i, const char *var,
		const char *value)
{
	free_and_allocate_env_var(env, i, var, value);
}
