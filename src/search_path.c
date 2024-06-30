#include <errno.h>
#include <minishell.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// size_t	count_env_vars(char **env)
// {
// 	size_t	count;

// 	count = 0;
// 	while (env[count])
// 		count++;
// 	return (count);
// }

// char	**copy_env(char **env)
// {
// 	size_t	count;
// 	char	**copy;

// 	count = count_env_vars(env);
// 	copy = malloc((count + 1) * sizeof(char *));
// 	if (!copy)
// 		return (NULL);
// 	count = 0;
// 	while (env[count])
// 	{
// 		copy[count] = ft_strdup(env[count]);
// 		// if (!copy[count])
// 		// 	return (free_double(copy), NULL);
// 		count++;
// 	}
// 	copy[count] = NULL;
// 	return (copy);
// }

void free_all(char **str)
{
	int i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

char	*find_path(char *command, char **export_env)
{
	int		i;
	char	**paths;
	char	*tmp;
	char *tmp2;

	if (access(command, F_OK) != -1 && access(command, X_OK) != -1)
		return command;
	
	if (!export_env[i])
		ft_perror("Error: No env variables found");
	i = 0;
	while (export_env[i] && !ft_strnstr(export_env[i], "PATH", 4))
		i++;
	if (!export_env[i])
		ft_perror("Error: No PATH variable found");
	
	paths = ft_split(&export_env[i][5], ':');
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		tmp2 = ft_strjoin(tmp, command);
		free(tmp);
		if (access(tmp2, F_OK) != -1 && access(tmp2, X_OK) != -1)
		{
			free_all(paths);
			return (tmp2);
		}
		else
			free(tmp2);
		i++;
	}
	free_all(paths);
	return (0);
}

// int	main(int argc, char *argv[], char **env)
// {
// 	char **export_env = copy_env(env);
// 	char line[100] = "echo hola";
// 	char *tmp;
// 	char **paths;
// 	int i = 0;
// 	char *comando = "echo";
// 	char *const args[] = {comando, "Hello, World!", NULL};

// 	// find_path(comando, export_env);

// 	tmp = find_path(comando, export_env);
// 	printf("env_path: %s\n", tmp);
// 	free(tmp);

// 	free_all(export_env);
// 	return 0;
// }

