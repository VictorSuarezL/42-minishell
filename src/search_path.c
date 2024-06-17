#include <errno.h>
#include <minishell.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// // Función para buscar el ejecutable en los directorios especificados en PATH
// char	*buscar_en_path(const char *comando)
// {
// 	char	*path;
// 	char	*path_dup;
// 	char	*token;
// 	size_t	length;
// 	char	*full_path;

// 	path = getenv("PATH");
// 	if (path == NULL)
// 	{
// 		return (NULL);
// 	}
// 	path_dup = strdup(path);
// 	if (path_dup == NULL)
// 	{
// 		perror("strdup");
// 		exit(EXIT_FAILURE);
// 	}
// 	token = strtok(path_dup, ":");
// 	while (token != NULL)
// 	{
// 		// Construir la ruta completa
// 		length = snprintf(NULL, 0, "%s/%s", token, comando) + 1;
// 		full_path = malloc(length);
// 		if (full_path == NULL)
// 		{
// 			perror("malloc");
// 			free(path_dup);
// 			exit(EXIT_FAILURE);
// 		}
// 		snprintf(full_path, length, "%s/%s", token, comando);
// 		// Verificar si el archivo existe y es ejecutable
// 		if (access(full_path, X_OK) == 0)
// 		{
// 			free(path_dup);
// 			return (full_path);
// 		}
// 		free(full_path);
// 		token = strtok(NULL, ":");
// 	}
// 	free(path_dup);
// 	return (NULL);
// }

// int	main(void)
// {
// 	extern char	**environ;
// 	char		*binaryPath;

// 	char *comando = "echo";
// 		// Comando que queremos ejecutar
// 	char *const argv[] = {comando, "Hello, World!", NULL};
// 		// Argumentos del comando
// 	// Buscar el comando en PATH
// 	binaryPath = buscar_en_path(comando);
// 	if (binaryPath == NULL)
// 	{
// 		fprintf(stderr, "No se encontró el comando %s en PATH\n", comando);
// 		exit(EXIT_FAILURE);
// 	}
// 	printf("Ejecutando execve para %s...\n", binaryPath);
// 	if (execve(binaryPath, argv, environ) == -1)
// 	{
// 		perror("execve");
// 		free(binaryPath);
// 		exit(EXIT_FAILURE);
// 	}
// 	// Este código no se ejecutará si execve tiene éxito
// 	printf("Esto no debería imprimirse\n");
// 	return (0);
// }

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
		copy[count] = strdup(env[count]);
		// if (!copy[count])
		// 	return (free_double(copy), NULL);
		count++;
	}
	copy[count] = NULL;
	return (copy);
}

char *find_path(char *command, char **export_env)
{
    int i = 0;
	char **paths;
	char *tmp;
	// char **export_env = copy_env(env);

	while (export_env[i] && !strnstr(export_env[i], "PATH", 4))
	{
		// printf("Ejecutando execve para %s...\n", export_env[i]);
		i++;
	}
	paths = ft_split(&export_env[i][5], ':');
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		tmp = ft_strjoin(tmp, command);
        // printf("%i: %s\n", i, tmp);
		if (access(tmp, F_OK) != -1 && access(tmp, X_OK) != -1)
		{
            return(tmp);
		}
		// break ;
		i++;
	}
    return NULL;
}

int	main(int argc, char *argv[], char **env)
{
	char **export_env = copy_env(env);
	char line[100] = "echo hola";
	char *tmp;
	char **paths;
	int i = 0;
	char *comando = "echo";
	char *const args[] = {comando, "Hello, World!", NULL};

	while (export_env[i] && !strnstr(export_env[i], "PATH", 4))
	{
		printf("Ejecutando execve para %s...\n", export_env[i]);
		i++;
	}
    printf("->>Ejecutando execve para %s...\n", &export_env[i][5]);

    
	paths = ft_split(&export_env[i][5], ':');
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		tmp = ft_strjoin(tmp, comando);
        // printf("%i: %s\n", i, tmp);
		if (access(tmp, F_OK) != -1 && access(tmp, X_OK) != -1)
		{
			printf("!!Ejecutando execve para %s...\n", tmp);
			if (execve(tmp, args, export_env) == -1)
			{
                printf("execve error para: %s\n", tmp);
				// perror("execve");
				// free(tmp);
				// exit(EXIT_FAILURE);
			}
		}
		// break ;
		i++;
	}
	// printf("%s\n", tmp);

	// Comando que queremos ejecutar
	// Argumentos del comando
	// Buscar el comando en PATH
	// binaryPath = buscar_en_path(comando);
	// if (binaryPath == NULL)
	// {
	// 	fprintf(stderr, "No se encontró el comando %s en PATH\n", comando);
	// 	exit(EXIT_FAILURE);
	// }

	// printf("%zu\n", count_env_vars(env));
	// printf("%s\n", export_env[0]);

	// runcmd(parse_cmd(line), export_env);
}