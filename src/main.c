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

int	fork1(void)
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
		copy[count] = strdup(env[count]);
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
	char			*binaryPath;
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
		if (execve(find_path(ecmd->argv[0], env_copy), ecmd->argv, env_copy) == -1)
		{
			ft_error("error: execve");
		}
		// aux = find_path(ecmd->argv[0], env_copy);
		printf("path: %s\n", aux);
		free(aux);
		// free(env_copy);
		free(ecmd);
	}
	else if (cmd->type == PIPE)
	{
		pcmd = (struct pipecmd *)cmd;
		if (pipe(p) < 0)
			ft_error("pipe");
		if (fork1() == 0)
		{
			close(1);
			dup(p[1]);
			close(p[0]);
			close(p[1]);
			runcmd(pcmd->left, env_copy);
		}
		if (fork1() == 0)
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
			fprintf(stderr, "open %s failed: %s\n", rcmd->file,
				strerror(errno));
			exit(1);
		}
		runcmd(rcmd->cmd, env_copy);
	}
	exit(0);
}

int	main(int argc, char *argv[], char **env)
{
	char	**export_env;
	char	line[100] = "ls -al";

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