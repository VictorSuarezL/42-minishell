#include <minishell.h>

char	*get_path(char **env)
{
	int			i;
	char		*path;
	const char	*home_prefix = "HOME=";
	size_t		home_len;

	home_len = ft_strlen(home_prefix);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], home_prefix, home_len) == 0)
		{
			path = malloc(ft_strlen(env[i]) - home_len + 1);
			if (!path)
				return (NULL);
			ft_strlcpy(path, env[i] + home_len, ft_strlen(env[i]) - home_len
				+ 1);
			return (path);
		}
		i++;
	}
	return (NULL);
}

int	cd_builtin(const char *path, char **env)
{
	char	*new_path;
	char	*full_path;
	int		i;

	i = 2;
	if (path == NULL)
	{
		path = "~";
		i = 0;
	}
	while (path[i] == ' ')
		i++;
	if (path[i] == '~')
	{
		new_path = get_path(env);
		if (new_path == NULL)
			return (ft_putstr_fd("Error getting home directory\n", 2), 1);
		if (path[i + 1] != '\0')
		{
			full_path = ft_strjoin(new_path, path + i + 1);
			free(new_path);
			if (full_path == NULL)
				return (ft_putstr_fd("Memory allocation error\n", 2), 1);
		}
		else
		{
			full_path = new_path;
		}
		if (chdir(full_path) == -1)
		{
			perror("cd");
			free(full_path);
			return (1);
		}
		free(full_path);
		return (0);
	}
	else
	{
		if (chdir(path) == -1)
		{
			perror("cd");
			return (1);
		}
		return (0);
	}
}
