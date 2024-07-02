#include <errno.h>
#include <fcntl.h>
#include <minishell.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


#define BACK 5
#define MAXARGS 10

void ft_perror(char *msg)
{
    if (errno) // Si hay un error definido en errno
    {
		// printf("inside ft_error with errno!\n original msg: %s\t errno = %d\n", msg, errno);
        perror("Error"); // Imprime el mensaje de error asociado a errno
	    exit(errno); // Sale del programa con un código de error
    }
    else // Si errno es 0, es decir, no hay error
    {
		// printf("inside ft_error!\n");
        ft_putendl_fd(msg, STDERR_FILENO); // Imprime el mensaje de error personalizado
	    exit(1); // Sale del programa con un código de error
    }
        // ft_putendl_fd(msg, STDERR_FILENO); // Imprime el mensaje de error personalizado
	    // exit(1); // Sale del programa con un código de error
}

/* int execute_builtin(char *input, char ***export, char ***env) {
    char **split_input;
    if (strncmp(input, "echo", 4) == 0)
        return (echo_builtin(input), 0);
    if (strncmp(input, "exit", 4) == 0)
        return (exit_builtin());
    if (strncmp(input, "env", 3) == 0)
        return (env_builtin(input, *env));
    if (strncmp(input, "export", 6) == 0) {
        split_input = ft_split(input, ' ');
        int result = export_builtin(split_input, export, env);
        free_double(split_input);
        return result;
    }
    if (strncmp(input, "pwd", 3) == 0)
        return (pwd_builtin(input));
    if (strncmp(input, "unset", 5) == 0) {
        split_input = ft_split(input, ' ');
        int result = unset_builtin(export, split_input);
        unset_builtin(env, split_input);
        free_double(split_input);
        return result;
    }
    return (47);
}
int is_builtin_env(char *input)
{
    if (ft_strncmp(input, "export", 6) == 0 || ft_strncmp(input, "unset", 5) == 0 || ft_strncmp(input, "env", 3) == 0 )
        return (1);
    return (0);
}
int is_builtin(char *input)
{
    if (ft_strncmp(input, "echo", 4) == 0 || ft_strncmp(input, "exit", 4) == 0 || ft_strncmp(input, "env", 3) == 0
        || ft_strncmp(input, "export", 6) == 0 || ft_strncmp(input, "pwd", 3) == 0 || ft_strncmp(input, "unset", 5) == 0)
        return (1);
    return (0);
} */




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

void escape_d_chars(char *str, char *aux, int *i, int *j) 
{
	aux[(*j)++] = str[(*i)++];
    while (str[*i] && str[*i] != '"') 
	{
		if (ft_strchr("'<>|\\", str[*i]))
		{
		    aux[*j] = '\\';
			(*j)++;
		}
	aux[(*j)++] = str[(*i)++];
    }
    if (str[*i] == '"') 
		aux[(*j)++] = str[(*i)++];
}

void escape_s_chars(char *str, char *aux, int *i, int *j) 
{
	aux[(*j)++] = str[(*i)++];
    while (str[*i] && str[*i] != '\'') 
	{
        if (ft_strchr("$\"<>|\\", str[*i])) 
		{
            aux[*j] = '\\';
			(*j)++;
        }
	aux[(*j)++] = str[(*i)++];
    }
    if (str[*i] == '\'') 
		aux[(*j)++] = str[(*i)++];
}

// Función principal para escapar caracteres especiales en una cadena
void escape_special_chars(char *str) {
    int i = 0;
	int j = 0;
    char *aux;
	
	aux = malloc(strlen(str) * 2 + 1);
	if (!aux)
		ft_perror("Error in malloc");

    while (str[i]) 
	{
        if (str[i] == '"') 
            escape_d_chars(str, aux, &i, &j);
		else if (str[i] == '\'') 
            escape_s_chars(str, aux, &i, &j);
		else 
            aux[j++] = str[i++];
    }
    aux[j] = '\0';
    strcpy(str, aux);
    free(aux);
}

void pop_slash(char *str)
{
	int i = 0;
	int j = 0;
	char *aux;

	aux = malloc(ft_strlen(str));

	while (str[i])
	{
		if (str[i] == '\\')
			i++;
		else
			aux[j++] = str[i++];
	}
	strcpy(str, aux);
	free(aux);
}

int wait_status(void)
{
	int status;
	int out_status = 0;

	wait(&status);
    if (WIFEXITED(status) && WEXITSTATUS(status) != 0) 
		out_status = WEXITSTATUS(status);
	else if (WIFEXITED(status) && WIFSIGNALED(status) != 0) 
		out_status = WTERMSIG(status);
	else
		out_status = 0;
	return(out_status);
}

void replace_qmark(char *line, int exit_status) 
{
    char *pos;
    char *exit_status_str;
    int len = strlen(line);
    int buffer_size; 
    char *buffer;

    exit_status_str = ft_itoa(exit_status);
    // if (!exit_status_str) 
	// {
    //     perror("ft_itoa failed");
    //     exit(EXIT_FAILURE);
    // }

    buffer_size = len + strlen(exit_status_str) - 2 + 1;
    buffer = malloc(sizeof(char) * buffer_size);
    // if (!buffer) 
	// {
    //     free(exit_status_str);
    //     perror("malloc failed");
    //     exit(EXIT_FAILURE);
    // }

    pos = ft_strnstr(line, "$?", len);
    if (!pos || (line[0] == '$' && line[1] == '?')) 
	{
        free(buffer);
        free(exit_status_str);
        return;
    }

    // int prefix_len = pos - line;
    ft_strlcpy(buffer, line, pos - line + 1);

    ft_strlcat(buffer, exit_status_str, buffer_size);
    ft_strlcat(buffer, pos + 2, buffer_size);

    strcpy(line, buffer);

    free(buffer);
    free(exit_status_str);
}

int	main(int argc, char *argv[], char **env)
{
	char buf[4097];
	int exit_status  = 0;
	// int my_perror = 0;
	char	**export_env;
	// char	line[100] = "echo 'hola $HOME << > >  | mundo' estamos aqui 'aquí vamos $$$$$ otra vez' adfa";
	char	line[100] = "ls -al | grep d";
	// char	line[100] = "ls -al | echo $?";
	// char	line[100] = "echo $HOME";
	// char	line[100] = "echa";
	// char	line[100] = "/bin/echo hola";
	// char	line[100] = "echo 'hola echo mundo' esto es una prueba";
	// char	line[100] = "wc -l < b.txt < c.txt < a.txt";
	// char	line[100] = "cat /dev/random | head";
	// char	line[100] = "cat | cat | ls";
	// char	line[100] = "ls -al | grep d | wc -l";

	if (!validator(line))
	{
		printf("syntax error!\n");

	}
	
	// replace_qmark(line, exit_status);
	// printf("line = %s\n", line);

	// escape_special_chars(line);
	// printf("line: %s\n", line);
	// pop_slash(line);
	// printf("line poped: %s\n", line);

	// export_env = copy_env(env);
	// runcmd(parse_cmd(line), export_env, &my_perror);
	// SOLUCION PARA EL ERROR DEL HIJO:
	
	// Sustituir en line la cadena de texto "$?" por el valor de exit_status

	// char *aux = ft_atoi(exit_status);




	
	if(save_fork() == 0) 
	{
      runcmd(parse_cmd(line), env);
      exit(0); // Exit the child process after executing the command
    }
	exit_status = wait_status();
	printf("Exit status = %d\n", exit_status);
}
