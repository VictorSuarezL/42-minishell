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


int	save_fork(void)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		ft_perror("fork");
	return (pid);
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
		// if (!copy[count])
		// 	return (free_double(copy), NULL);
		count++;
	}
	copy[count] = NULL;
	return (copy);
}

// char *command_error(char *cmnd)
// {
// 	char *aux;

// 	aux = malloc(ft_strlen(str)+fr_strlen(cmnd)+1);
// 	if (!aux)
// 	{
// 		ft_perror("Malloc error");
// 	}
// 	while ()
// 	{

// 	}
	
	

// }

void wait_pipe(void)
{
		int status;
		int i = 0;

		while (i < 2) 
		{
            if (wait(&status) == -1) 
			{
                perror("wait failed");
                exit(errno);
            }
            if (WIFEXITED(status) && WEXITSTATUS(status) != 0) 
			{
                exit(WEXITSTATUS(status));
            }
			i++;
        }
}

void	runcmd(struct cmd *cmd, char **env_copy)
{
	int				p[2];
	struct execcmd	*ecmd;
	struct pipecmd	*pcmd;
	struct redircmd	*rcmd;
	char *cmd_path;

	if (!cmd)
	{
		exit(1);
	}
	if (cmd->type == EXEC)
	{
		ecmd = (struct execcmd *)cmd;
		if (!ecmd->argv[0])
			exit(1);
		// if (is_builtin(ecmd->argv[0]))
		// {
		// 	// Guardar en un entero la salida del execute_builtin
		// 	execute_builtin(ecmd->argv[0], NULL, &env_copy);
		// }
		// else if (execve(find_path(ecmd->argv[0], env_copy), ecmd->argv, env_copy) == -1)
		// {
		// 	ft_perror("error: execve");
		// }
		cmd_path = find_path(ecmd->argv[0], env_copy);
		if (!cmd_path)
		{
			ft_putendl_fd("Command not found", STDERR_FILENO);
			// printf("hererer!!\n");
			exit(127);
			// errno = ENOENT;
			// ft_perror("Error!");
		}
		// int i = -1;
		// while (ecmd->argv[++i])
		// {
		// 	if (ft_strcmp("$?", ecmd->argv[i]) == 0)
		// 	{
		// 		ecmd->argv[i] = ft_itoa(errno);
		// 		break;
		// 	}
			
		// }
		
		if (execve(cmd_path, ecmd->argv, env_copy) == -1)
		{
			// printf("here!here!\n");
			ft_perror("Error: execve");
			// fprintf(stderr, "%s: %s\n", ecmd->argv[0], strerror(errno));
			// exit(1);
		// ft_putendl_fd(strerror(errno), STDERR_FILENO);
		// exit(1);
		}
		// aux = find_path(ecmd->argv[0], env_copy);
		// printf("path: %s\n", aux);
		// free(aux);
		// free(env_copy);
		free(ecmd);
	}
	else if (cmd->type == PIPE)
	{
		pcmd = (struct pipecmd *)cmd;
		if (pipe(p) < 0)
			ft_perror("Error: piped out!");
		if (save_fork() == 0)
		{
			close(1);
			dup(p[1]);
			close(p[0]);
			close(p[1]);
			runcmd(pcmd->left, env_copy);
			// exit(errno);
		}
		if (save_fork() == 0)
		{
			close(0);
			dup(p[0]);
			close(p[0]);
			close(p[1]);
			runcmd(pcmd->right, env_copy);
			// exit(errno);
		}
		close(p[0]);
		close(p[1]);
		wait_pipe();
		// wait_status();
		// wait_status();

		// errno = wait_status();
		// errno = wait_status();
		// int status;
		// for (int i = 0; i < 2; i++) {
        //     if (wait(&status) == -1) {
        //         perror("wait failed");
        //         exit(errno);
        //     }
        //     if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
        //         // Captura el código de salida si es diferente de 0
        //         exit(WEXITSTATUS(status));
        //     }
        // }
	}
	else if (cmd->type == REDIR)
	{
		rcmd = (struct redircmd *)cmd;
		close(rcmd->fd);
		if (open(rcmd->file, rcmd->mode, 0644) < 0)
		{
			ft_perror("open failed: No such file or directory");
			// exit(errno);
			// ft_perror("mi error");
			// fprintf(stderr, "%s: %s\n", rcmd->file,
			// 	strerror(errno));
			// exit(1);
		}
		runcmd(rcmd->cmd, env_copy);
	}
	exit(0);
}

// int in_quote(char *str, int i, char c)
// {
// 	int j = 0;
// 	int flag = 0;
// 	while (str[j] && j < i)
// 	{
// 		if (str[j] == c && flag == 0)
// 		{
// 			flag = 1;
// 		}
// 		else if (str[j] == c && flag == 1)
// 		{
// 			flag = 0;
// 		}
// 		j++;
// 	}
// 	return flag;
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
    if (!exit_status_str) 
	{
        perror("ft_itoa failed");
        exit(EXIT_FAILURE);
    }

    buffer_size = len + strlen(exit_status_str) - 2 + 1;
    buffer = malloc(sizeof(char) * buffer_size);
    if (!buffer) 
	{
        free(exit_status_str);
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

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
	// char	line[100] = "ls -al | grepa ";
	// char	line[100] = "ls -al | echo $?";
	// char	line[100] = "echo $HOME";
	// char	line[100] = "echa";
	// char	line[100] = "/bin/echo hola";
	char	line[100] = "echo hola";
	// char	line[100] = "wc -l < b.txt < c.txt < a.txt";
	// char	line[100] = "cat /dev/random | head";
	// char	line[100] = "cat | cat | ls";

	
	replace_qmark(line, exit_status);
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


    // int status;
    // wait(&status); // Wait for the child process to finish
    // if (WIFEXITED(status)) 
	// {
	// 	errno = WEXITSTATUS(status);
    // //   int exit_status = WEXITSTATUS(status);
	// //   errno = exit_status;
    // //   printf("Child process exited with status: %d\nErrno = %d\n", exit_status, errno);
    // } 
	// else if (WIFSIGNALED(status)) 
	// {
    //   int signal_number = WTERMSIG(status);
	//   errno = signal_number;
    //   printf("Child process terminated by signal: %d\n", signal_number);
    // }
	// printf("errno: %d\n", my_perror);
	// free_all(export_env);
}

// int main() {
//     char *binaryPath = "/bin/ls";
//     // char *binaryPath = "usr/bin";
//     char *const argv[] = {"ls", "-l", NULL};
//     char *const envp[] = { "MY_VAR=Hello", NULL };

//     printf("Ejecutando execve...\n");

//     if (execve(binaryPath, argv, envp) == -1) {
//         perror("execve");
//         exit(EXIT_FAILURE);
//     }

//     printf("Esto no debería imprimirse\n");
//     return (0);
// }
