#include <minishell.h>

void	run_redir_cmd(t_cmd *cmd, char **env_copy)
{
	t_redircmd	*rcmd;

	rcmd = (t_redircmd *)cmd;
	close(rcmd->fd);
	if (open(rcmd->file, rcmd->mode, 0644) < 0)
		ft_perror("open failed: No such file or directory");
	runcmd(rcmd->cmd, env_copy);
}

void	run_pipe_cmd(t_cmd *cmd, char **env_copy)
{
	t_pipecmd	*pcmd;
	int			p[2];

	pcmd = (t_pipecmd *)cmd;
	if (pipe(p) < 0)
		ft_perror("Error: piped out!");
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
	wait_pipe();
}

void	run_exec_cmd(t_cmd *cmd, char **env_copy)
{
	t_execcmd	*ecmd;
	char		*cmd_path;

	ecmd = (t_execcmd *)cmd;
	if (!ecmd->argv[0])
		exit(1);
	if (is_builtin(ecmd->argv[0]))
	{
		// Guardar en un entero la salida del execute_builtin
		execute_builtin(ecmd->argv[0], NULL, &env_copy);
	}
	else if (execve(find_path(ecmd->argv[0], env_copy), ecmd->argv,
			env_copy) == -1)
	{
		ft_perror("error: execve");
	}
	cmd_path = find_path(ecmd->argv[0], env_copy);
	if (!cmd_path)
	{
		ft_putendl_fd("Command not found", STDERR_FILENO);
		exit(127);
	}
	remove_quotes(ecmd);
	execve(cmd_path, ecmd->argv, env_copy);
}

void	runcmd(t_cmd *cmd, char **env_copy)
{
	if (!cmd)
	{
		exit(1);
	}
	if (cmd->type == EXEC)
	{
		run_exec_cmd(cmd, env_copy);
	}
	else if (cmd->type == PIPE)
	{
		run_pipe_cmd(cmd, env_copy);
	}
	else if (cmd->type == REDIR)
	{
		run_redir_cmd(cmd, env_copy);
	}
	exit(0);
}
