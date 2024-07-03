#include <minishell.h>

// void	run_redir_cmd(struct cmd *cmd, char **env_copy)
// {
// 	struct redircmd	*rcmd;

// 	rcmd = (struct redircmd *)cmd;
// 	close(rcmd->fd);
// 	if (open(rcmd->file, rcmd->mode, 0644) < 0)
// 		ft_perror("open failed: No such file or directory");
// 	runcmd(rcmd->cmd, env_copy);
// }

// void	run_pipe_cmd(struct cmd *cmd, char **env_copy)
// {
// 	struct pipecmd	*pcmd;
// 	int				p[2];

// 	pcmd = (struct pipecmd *)cmd;
// 	if (pipe(p) < 0)
// 		ft_perror("Error: piped out!");
// 	if (save_fork() == 0)
// 	{
// 		close(1);
// 		dup(p[1]);
// 		close(p[0]);
// 		close(p[1]);
// 		runcmd(pcmd->left, env_copy);
// 	}
// 	if (save_fork() == 0)
// 	{
// 		close(0);
// 		dup(p[0]);
// 		close(p[0]);
// 		close(p[1]);
// 		runcmd(pcmd->right, env_copy);
// 	}
// 	close(p[0]);
// 	close(p[1]);
// 	wait_pipe();
// }

// void	run_exec_cmd(struct cmd *cmd, char **env_copy)
// {
// 	struct execcmd	*ecmd;
// 	char			*cmd_path;

// 	ecmd = (struct execcmd *)cmd;
// 	if (!ecmd->argv[0])
// 		exit(1);
// 	// if (is_builtin(ecmd->argv[0]))
// 	// {
// 	// 	// Guardar en un entero la salida del execute_builtin
// 	// 	execute_builtin(ecmd->argv[0], NULL, &env_copy);
// 	// }
// 	// else if (execve(find_path(ecmd->argv[0], env_copy), ecmd->argv,
// 			// env_copy) == -1)
// 	// {
// 	// 	ft_perror("error: execve");
// 	// }
// 	cmd_path = find_path(ecmd->argv[0], env_copy);
// 	if (!cmd_path)
// 	{
// 		ft_putendl_fd("Command not found", STDERR_FILENO);
// 		exit(127);
// 	}
// 	// remove_quotes(ecmd);
// 	// for (size_t i = 0; ecmd->argv[i]; i++)
// 	// {
// 	// 	printf("argv[%zu] = %s\n", i, ecmd->argv[i]);
// 	// }
	
// 	execve(cmd_path, ecmd->argv, env_copy);
// }

// void	runcmd(struct cmd *cmd, char **env_copy)
// {
// 	struct execcmd	*ecmd;

// 	if (!cmd)
// 	{
// 		exit(1);
// 	}
// 	if (cmd->type == EXEC)
// 	{
// 		run_exec_cmd(cmd, env_copy);
// 	}
// 	else if (cmd->type == PIPE)
// 	{
// 		run_pipe_cmd(cmd, env_copy);
// 	}
// 	else if (cmd->type == REDIR)
// 	{
// 		run_redir_cmd(cmd, env_copy);
// 	}
// 	exit(0);
// }

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
