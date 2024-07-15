#include <minishell.h>

void	run_redir_cmd(t_cmd *cmd, char **env_copy, char **export_copy)
{
	t_redircmd	*rcmd;

	rcmd = (t_redircmd *)cmd;
	close(rcmd->fd);
	if (open(rcmd->file, rcmd->mode, 0644) < 0)
		ft_perror("open failed: No such file or directory");
	runcmd(rcmd->cmd, env_copy, export_copy);
}

void	run_pipe_cmd(t_cmd *cmd, char **env_copy, char **export_copy)
{
	t_pipecmd	*pcmd;
	int			p[2];

	pcmd = (t_pipecmd *)cmd;
	// t_execcmd *left = (t_execcmd *)pcmd->left;
	// t_execcmd *right = (t_execcmd *)pcmd->right;
	// free(pcmd);
	if (pipe(p) < 0)
		ft_perror("Error: piped out!");
	if (save_fork() == 0)
	{
		close(1);
		dup(p[1]);
		close(p[0]);
		close(p[1]);
		// t_execcmd *right = (t_execcmd *)pcmd->right;
		// t_cmd *left = (t_cmd *)pcmd->left;
		// free(right);
		// runcmd((t_cmd *)left, env_copy, export_copy);
		runcmd(pcmd->left, env_copy, export_copy);
		// free(pcmd);
	}
	if (save_fork() == 0)
	{
		close(0);
		dup(p[0]);
		close(p[0]);
		close(p[1]);
		// t_execcmd *left = (t_execcmd *)pcmd->left;
		// t_cmd *right = (t_cmd *)pcmd->right;
		// free(left);
		// runcmd((t_cmd *)right, env_copy, export_copy);
		runcmd(pcmd->right, env_copy, export_copy);
		// free(pcmd);
	}
	close(p[0]);
	close(p[1]);
	final_clean(export_copy, env_copy);
	wait_pipe();
	// t_execcmd *left = (t_execcmd *)pcmd->left;
	// t_execcmd *right = (t_execcmd *)pcmd->right;
	// for(int i = 0; left->argv[i]; i++)
	// 	printf("left->argv[%d]: %s\n", i, left->argv[i]);
	// free_all(left->argv);
	// free(left);
	// free(right);
	// free(pcmd);
	// // free_all(pcmd->left);
	// // free_all(pcmd->right);
	// free(pcmd);
}

void	run_exec_cmd(t_cmd *cmd, char **env_copy, char **export_copy)
{
	t_execcmd	*ecmd;
	char		*cmd_path;

	ecmd = (t_execcmd *)cmd;
	if (!ecmd->argv[0])
		exit(1);
	remove_quotes(ecmd);
	if (is_builtin(ecmd->argv[0]))
		builtin_exec(ecmd, env_copy, export_copy);
	// free_all(export_copy);
	cmd_path = find_path(ecmd->argv[0], env_copy);
	if (!cmd_path)
	{
		ft_putendl_fd("Command not found", STDERR_FILENO);
		free(ecmd);
		free(cmd_path);
		final_clean(env_copy, export_copy);
		exit(127);
	}
	if (execve(cmd_path, ecmd->argv, env_copy) < 0)
	{
		free(cmd_path);
		free(ecmd);
		final_clean(env_copy, export_copy);
		ft_perror("execve failed");
	}
}

void	runcmd(t_cmd *cmd, char **env_copy, char **export_copy)
{
	if (!cmd)
	{
		exit(1);
	}
	if (cmd->type == EXEC)
	{
		run_exec_cmd(cmd, env_copy, export_copy);
	}
	else if (cmd->type == PIPE)
	{
		// t_pipecmd	*pcmd = (t_pipecmd *)cmd;

		// t_execcmd *left = (t_execcmd *)pcmd->left;
		// t_execcmd *right = (t_execcmd *)pcmd->right;
		run_pipe_cmd(cmd, env_copy, export_copy);
		// free(left);
		// free(right);
		// free(pcmd);
	}
	else if (cmd->type == REDIR)
	{
		run_redir_cmd(cmd, env_copy, export_copy);
	}
	// free(cmd);
	exit(0);
}
