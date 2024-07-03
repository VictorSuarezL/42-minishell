#include <minishell.h>

struct cmd	*exec_cmd(void)
{
	struct execcmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = EXEC;
	return ((struct cmd *)cmd);
}

struct cmd	*pipe_cmd(struct cmd *left, struct cmd *right)
{
	struct pipecmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = PIPE;
	cmd->left = left;
	cmd->right = right;
	return ((struct cmd *)cmd);
}

struct cmd	*redir_in_cmd(struct cmd *subcmd, char *file, char *efile, int mode)
{
	struct redircmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = REDIR;
	cmd->cmd = subcmd;
	cmd->file = file;
	cmd->efile = efile;
	cmd->mode = mode;
	cmd->fd = 0;
	return ((struct cmd *)cmd);
}

struct cmd	*redir_out_cmd(struct cmd *subcmd, char *file, char *efile, int mode)
{
	struct redircmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = REDIR;
	cmd->cmd = subcmd;
	cmd->file = file;
	cmd->efile = efile;
	cmd->mode = mode;
	cmd->fd = 1;
	return ((struct cmd *)cmd);
}

// struct cmd	*redir_cmd(struct cmd *subcmd, char *file, char *efile, int mode,
// 		int fd)
// {
// 	struct redircmd	*cmd;

// 	cmd = malloc(sizeof(*cmd));
// 	memset(cmd, 0, sizeof(*cmd));
// 	cmd->type = REDIR;
// 	cmd->cmd = subcmd;
// 	cmd->file = file;
// 	cmd->efile = efile;
// 	cmd->mode = mode;
// 	cmd->fd = fd;
// 	return ((struct cmd *)cmd);
// }
