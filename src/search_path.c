#include <minishell.h>

void	free_all(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

int	check_path(char **export_env)
{
	int	i;

	i = 0;
	if (!export_env[i])
		ft_perror("Error: No env variables found");
	while (export_env[i] && !ft_strnstr(export_env[i], "PATH", 4))
		i++;
	if (!export_env[i])
		ft_perror("Error: No PATH variable found");
	return (i);
}

char	*find_path(char *command, char **export_env)
{
	int		i;
	char	**paths;
	char	*tmp;
	char	*tmp2;

	if (access(command, F_OK) != -1 && access(command, X_OK) != -1)
		return (command);
	i = check_path(export_env);
	paths = ft_split(&export_env[i][5], ':');
	i = -1;
	while (paths[++i])
	{
		tmp = ft_strjoin(paths[i], "/");
		tmp2 = ft_strjoin(ft_strjoin(paths[i], "/"), command);
		free(tmp);
		if (access(tmp2, F_OK) != -1 && access(tmp2, X_OK) != -1)
			return (free_all(paths), tmp2);
		else
			free(tmp2);
	}
	free_all(paths);
	return (0);
}
/*
int	main(int argc, char *argv[], char **env)
{
	char	**export_env;
	char	line[100] = "echo hola";
	char	*tmp;
	char	**paths;
	int		i;
	char	*comando;

	export_env = copy_env(env);
	i = 0;
	comando = "echo";
	char *const args[] = {comando, "Hello, World!", NULL};
	// find_path(comando, export_env);
	tmp = find_path(comando, export_env);
	printf("env_path: %s\n", tmp);
	free(tmp);
	free_all(export_env);
	return (0);
}
 */
