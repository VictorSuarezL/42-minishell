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


// int wait_status(void)
// {
// 	int status;
// 	int out_status = 0;

// 	wait(&status);
//     if (WIFEXITED(status) && WEXITSTATUS(status) != 0) 
// 		out_status = WEXITSTATUS(status);
// 	else if (WIFEXITED(status) && WIFSIGNALED(status) != 0) 
// 		out_status = WTERMSIG(status);
// 	else
// 		out_status = 0;
// 	return(out_status);
// }



int	main(int argc, char *argv[], char **env)
{
	char buf[4097];
	int exit_status  = 0;
	// int my_perror = 0;
	char	**export_env;
	// char	line[100] = "echo 'hola $HOME << > >  | mundo' estamos aqui 'aquí vamos $$$$$ otra vez' adfa";
	// char	line[100] = "ls -al | grep d";
	// char	line[100] = "ls -al | echo $?";
	// char	line[100] = "echo $HOME";
	// char	line[100] = "echa";
	// char	line[100] = "/bin/echo hola";
	// char	line[100] = "echo 'hola echo mundo' esto es una prueba";
	// char	line[100] = "wc -l < b.txt < c.txt < a.txt";
	// char	line[100] = "cat /dev/random | head";
	// char	line[100] = "cat | cat | ls";
	// char	line[100] = "ls -al | grep d | wc -l";
	// char	line[100] = "ls -al > aa.txt";
	// char	line[100] = "echo 'hola \" son unas comillas'";
	char	line[100] = "echo 'hola \" son unas comillas' esto esta \" mal";

	if (!validator(line))
	{
		printf("syntax error!\n");
		exit(1);

	}
	
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



    runcmd(parse_cmd(line), env);

	
	// if(save_fork() == 0) 
	// {
    //   runcmd(parse_cmd(line), env);
    //   exit(0); // Exit the child process after executing the command
    // }
	exit_status = wait_status();
	printf("Exit status = %d\n", exit_status);
}
