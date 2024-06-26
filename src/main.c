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



char			whitespace[] = " \t\r\n\v";
char			symbols[] = "<|>&;()";

// int execute_builtin(char *input, char ***export, char ***env) {
//     char **split_input;
//     if (strncmp(input, "echo", 4) == 0)
//         return (echo_builtin(input), 0);
//     if (strncmp(input, "exit", 4) == 0)
//         return (exit_builtin());
//     if (strncmp(input, "env", 3) == 0)
//         return (env_builtin(input, *env));
//     if (strncmp(input, "export", 6) == 0) {
//         split_input = ft_split(input, ' ');
//         int result = export_builtin(split_input, export, env);
//         free_double(split_input);
//         return result;
//     }
//     if (strncmp(input, "pwd", 3) == 0)
//         return (pwd_builtin(input));
//     if (strncmp(input, "unset", 5) == 0) {
//         split_input = ft_split(input, ' ');
//         int result = unset_builtin(export, split_input);
//         unset_builtin(env, split_input);
//         free_double(split_input);
//         return result;
//     }
//     return (47);
// }
// int is_builtin_env(char *input)
// {
//     if (ft_strncmp(input, "export", 6) == 0 || ft_strncmp(input, "unset", 5) == 0 || ft_strncmp(input, "env", 3) == 0 )
//         return (1);
//     return (0);
// }
// int is_builtin(char *input)
// {
//     if (ft_strncmp(input, "echo", 4) == 0 || ft_strncmp(input, "exit", 4) == 0 || ft_strncmp(input, "env", 3) == 0
//         || ft_strncmp(input, "export", 6) == 0 || ft_strncmp(input, "pwd", 3) == 0 || ft_strncmp(input, "unset", 5) == 0)
//         return (1);
//     return (0);
// }

int	save_fork(void)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		ft_error("fork");
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

void	runcmd(struct cmd *cmd, char **env_copy)
{
	int				p[2];
	struct execcmd	*ecmd;
	struct pipecmd	*pcmd;
	struct redircmd	*rcmd;
	char *aux;

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
		// 	ft_error("error: execve");
		// }
		if (execve(find_path(ecmd->argv[0], env_copy), ecmd->argv, env_copy) == -1)
		{
			ft_error("error: execve");
		}
		// aux = find_path(ecmd->argv[0], env_copy);
		// printf("path: %s\n", aux);
		free(aux);
		// free(env_copy);
		free(ecmd);
	}
	else if (cmd->type == PIPE)
	{
		pcmd = (struct pipecmd *)cmd;
		if (pipe(p) < 0)
			ft_error("pipe");
		if (save_fork() == 0)
		{
			close(1);
			dup(p[1]);
			close(p[0]);
			close(p[1]);
			runcmd(pcmd->left, env_copy);
		}
		if (save_fork() == 0)
		{
			close(0);
			dup(p[0]);
			close(p[0]);
			close(p[1]);
			runcmd(pcmd->right, env_copy);
		}
		close(p[0]);
		close(p[1]);
		wait(0);
		wait(0);
	}
	else if (cmd->type == REDIR)
	{
		rcmd = (struct redircmd *)cmd;
		close(rcmd->fd);
		if (open(rcmd->file, rcmd->mode, 0644) < 0)
		{
			ft_error("open failed: No such file or directory");

			// fprintf(stderr, "open %s failed: %s\n", rcmd->file,
			// 	strerror(errno));
			// exit(1);
		}
		runcmd(rcmd->cmd, env_copy);
	}
	exit(0);
}

int in_quote(char *str, int i, char c)
{
	int j = 0;
	int flag = 0;
	while (str[j] && j < i)
	{
		if (str[j] == c && flag == 0)
		{
			flag = 1;
		}
		else if (str[j] == c && flag == 1)
		{
			flag = 0;
		}
		j++;
	}
	return flag;
}



char *escape_special_chars(char *buf, char *str)
{
	int i = 0;
	int count = 0;
	// char *aux;
	int j = 0;

	// while (str[i])
	// {
	// 	if (str[i] == '$' || str[i] == '\'' || str[i] == '"')
	// 		count++;
	// 	i++;
	// }

	// aux = malloc(sizeof(char) * (ft_strlen(str) + count + 1));
	
	i = 0;
	while (str[i])
	{
		if (ft_strchr("'", str[i]) && in_quote(str, i, '"'))
		{
			buf[j] = '\\';
			j++;
			buf[j] = str[i];
		} 
		else
		{
			buf[j] = str[i];
		}
		j++;
		i++;
	}
	// printf("aux: %s\n", aux);
	return buf;
}

int	main(int argc, char *argv[], char **env)
{
	char buf[4097];
	char	**export_env;
	char	line[100] = "echo 'hola mundo'";
	char *aux;
	buf = escape_special_chars(buf, line);

	export_env = copy_env(env);
	runcmd(parse_cmd(line), export_env);
	free_all(export_env);
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