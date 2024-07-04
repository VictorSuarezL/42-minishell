#include <minishell.h>

t_cmd	*exec_cmd(void)
{
	t_execcmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
		ft_perror("Error in malloc");
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = EXEC;
	return ((t_cmd *)cmd);
}

t_cmd	*pipe_cmd(t_cmd *left, t_cmd *right)
{
	t_pipecmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
		ft_perror("Error in malloc");
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = PIPE;
	cmd->left = left;
	cmd->right = right;
	return ((t_cmd *)cmd);
}

t_cmd	*redir_in_cmd(t_cmd *subcmd, char *file, char *efile, int mode)
{
	t_redircmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
		ft_perror("Error in malloc");
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = REDIR;
	cmd->cmd = subcmd;
	cmd->file = file;
	cmd->efile = efile;
	cmd->mode = mode;
	cmd->fd = 0;
	return ((t_cmd *)cmd);
}

t_cmd	*redir_out_cmd(t_cmd *subcmd, char *file, char *efile, int mode)
{
	t_redircmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
		ft_perror("Error in malloc");
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = REDIR;
	cmd->cmd = subcmd;
	cmd->file = file;
	cmd->efile = efile;
	cmd->mode = mode;
	cmd->fd = 1;
	return ((t_cmd *)cmd);
}
