#include <minishell.h>

/* char	*get_path(char **env)
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
} */
char	*get_path(char **env)
{
	// This function should return the path from env, e.g., getenv("HOME")
	// Mock implementation for example purposes
	for (int i = 0; env[i] != NULL; i++)
	{
		if (strncmp(env[i], "HOME=", 5) == 0)
		{
			return (strdup(env[i] + 5));
		}
	}
	return (NULL);
}

/* void	update_env_var(char **env, const char *var, const char *value)
{
	size_t	var_len;
	int		i;

	var_len = strlen(var);
	for (int i = 0; env[i] != NULL; i++)
	{
		if (strncmp(env[i], var, var_len) == 0 && env[i][var_len] == '=')
		{
			free(env[i]);
			env[i] = malloc(var_len + strlen(value) + 2); // +2 for '=' and '\0'
			if (env[i] != NULL)
			{
				sprintf(env[i], "%s=%s", var, value);
			}
			return ;
		}
	}
	// If variable not found, add it to the environment
	i = 0;
	while (env[i] != NULL)
		i++;
	env[i] = malloc(var_len + strlen(value) + 2); // +2 for '=' and '\0'
	if (env[i] != NULL)
	{
		sprintf(env[i], "%s=%s", var, value);
	}
	env[i + 1] = NULL; // Null-terminate the environment array
} */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void update_env_var(char **env, const char *var, const char *value) {
    size_t var_len = strlen(var);
    size_t value_len = strlen(value);
    int i;

    // Buscar si la variable ya existe en el entorno
    for (i = 0; env[i] != NULL; i++) {
        if (strncmp(env[i], var, var_len) == 0 && env[i][var_len] == '=') {
            // La variable ya existe, actualizar su valor
            size_t old_value_len = strlen(env[i] + var_len + 1); // Longitud del valor actual
            if (old_value_len < value_len) {
                // Liberar memoria asignada anteriormente si el nuevo valor es más largo
                free(env[i]);
                env[i] = malloc(var_len + value_len + 2); // +2 for '=' and '\0'
                if (env[i] == NULL) {
                    perror("malloc");
                    return;
                }
            }
            // Construir la entrada de la variable de entorno
            strcpy(env[i], var);
            env[i][var_len] = '=';
            strcpy(env[i] + var_len + 1, value);
            return;
        }
    }

    // Si la variable no se encontró, agregarla al entorno con el valor especificado
    env[i] = malloc(var_len + value_len + 2); // +2 for '=' and '\0'
    if (env[i] == NULL) {
        perror("malloc");
        return;
    }
    // Construir la entrada de la variable de entorno
    strcpy(env[i], var);
    env[i][var_len] = '=';
    strcpy(env[i] + var_len + 1, value);
    env[i + 1] = NULL; // Null-terminate the environment array
}



char	*get_oldpwd(char **env)
{
	for (int i = 0; env[i] != NULL; i++)
	{
		if (strncmp(env[i], "OLDPWD=", 7) == 0)
		{
			return (env[i] + 7);
		}
	}
	return (NULL);
}

int	cd_builtin(const char *path, char **env)
{
	static char *prev_dir = NULL;
	char *new_path;
	char cwd[4096];

	if (!path || ft_strcmp(path, "~") == 0)
	{
		path = "~";
	}
	else if (ft_strcmp(path, "-") == 0)
	{
		char *oldpwd = get_oldpwd(env);
		if (!oldpwd)
		{
			return (ft_putstr_fd("OLDPWD not set\n", 2), 1);
		}
		path = oldpwd;
		printf("%s\n", path);
	}

	if (ft_strcmp(path, "~") == 0)
	{
		new_path = get_path(env);
		if (!new_path)
		{
			return (ft_putstr_fd("Error getting home directory\n", 2), 1);
		}
		path = new_path;
	}
	else
	{
		new_path = NULL;
	}

	if (chdir(path) == -1)
	{
		perror("cd");
		free(new_path);
		return (1);
	}

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("getcwd");
		free(new_path);
		return (1);
	}

	if (prev_dir)
	{
		update_env_var(env, "OLDPWD", prev_dir);
			// Actualiza OLDPWD con el directorio anterior
		free(prev_dir);
	}
	prev_dir = ft_strdup(cwd);
		// Guarda el directorio actual para futuras llamadas a "-"

	free(new_path);
	return (0);
}