#include <minishell.h>

struct cmd	*exec_cmd(void)
{
	struct execcmd	*cmd;

	cmd = calloc(1, sizeof(*cmd));
	cmd->type = EXEC;
	return ((struct cmd *)cmd);
}

struct cmd	*pipe_cmd(struct cmd *left, struct cmd *right)
{
	struct pipecmd	*cmd;

	cmd = calloc(1, sizeof(*cmd));
	cmd->type = PIPE;
	cmd->left = left;
	cmd->right = right;
	return ((struct cmd *)cmd);
}

struct cmd	*redir_out_cmd(struct cmd *subcmd, char *file, char *efile,
		int mode)
{
	struct redircmd	*cmd;

	cmd = calloc(1, sizeof(*cmd));
	cmd->type = REDIR;
	cmd->cmd = subcmd;
	cmd->file = file;
	cmd->efile = efile;
	cmd->mode = mode;
	cmd->fd = STDOUT_FILENO;
	return ((struct cmd *)cmd);
}

struct cmd	*redir_in_cmd(struct cmd *subcmd, char *file, char *efile, int mode)
{
	struct redircmd	*cmd;

	cmd = calloc(1, sizeof(*cmd));
	cmd->type = REDIR;
	cmd->cmd = subcmd;
	cmd->file = file;
	cmd->efile = efile;
	cmd->mode = mode;
	cmd->fd = STDIN_FILENO;
	return ((struct cmd *)cmd);
}
