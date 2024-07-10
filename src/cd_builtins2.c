#include <minishell.h>

void	update_env_var(char **env, const char *var, const char *value)
{
	int	i;

	i = find_env_var(env, var);
	if (env[i] != NULL)
		update_existing_env_var(env, i, var, value);
	else
		allocate_new_env_var(env, i, var, value);
}

char	*create_expanded_path(const char *home, const char *path)
{
	size_t	home_len;
	size_t	path_len;
	char	*expanded_path;

	home_len = ft_strlen(home);
	path_len = ft_strlen(path);
	expanded_path = malloc(home_len + path_len);
	if (!expanded_path)
	{
		perror("malloc");
		return (NULL);
	}
	ft_strcpy(expanded_path, home);
	if (path[1])
		ft_strcat(expanded_path, path + 1);
	return (expanded_path);
}

char	*handle_virguline(const char *path, char **env)
{
	char	*home;
	char	*expanded_path;

	home = get_env_value("HOME", env);
	if (!home)
	{
		ft_printf("Error getting home directory\n");
		return (NULL);
	}
	expanded_path = create_expanded_path(home, path);
	free(home);
	return (expanded_path);
}

char	*expand_path(const char *path, char **env)
{
	if (path[0] == '~')
		return (handle_virguline(path, env));
	return (ft_strdup(path));
}

int	is_empty_or_spaces(const char *str)
{
	if (str == NULL || *str == '\0')
		return (1);
	while (*str != '\0')
	{
		if (*str != ' ')
			return (0);
		str++;
	}
	return (1);
}
