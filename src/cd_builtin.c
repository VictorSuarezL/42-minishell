#include <minishell.h>

char	*get_path(char **env)
{
	int		i;
	int		j;
	int		k;
	char	*path;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "HOME", ft_strlen("HOME")) == 0)
		{
			j = ft_strlen("HOME") + 1;
			path = malloc(ft_strlen(env[i][j]));
			if (!path)
				return (NULL);
			k = 0;
			while (env[i][j])
			{
				path[k] = env[i][j];
				j++;
			}
		}
		i++;
	}
	return (path);
}

int	cd_builtin(const char *path, char **env)
{
	char	*new_path;

	if (path == NULL)
		return (ft_putstr_fd("\n", 2), 0);
	if (path[0] == '~')
	{
		new_path = get_path(env);
		new_path = ft_strjoin(new_path, path);
		if (chdir(new_path) == -1)
			return (perror("cd"), 1);
		return (0);
	}
	if (chdir(path) == -1)
		return (perror("cd"), 1);
	return (0);
}
