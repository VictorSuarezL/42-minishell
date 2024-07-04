#include <minishell.h>

struct cmd	*nulterminate(struct cmd *cmd)
{
	struct execcmd	*ecmd;
	struct pipecmd	*pcmd;
	struct redircmd	*rcmd;
	int				i;

	i = -1;
	if (cmd->type == EXEC)
	{
		ecmd = (struct execcmd *)cmd;
		while (ecmd->argv[++i])
			*ecmd->eargv[i] = 0;
	}
	else if (cmd->type == PIPE)
	{
		pcmd = (struct pipecmd *)cmd;
		nulterminate(pcmd->left);
		nulterminate(pcmd->right);
	}
	else if (cmd->type == REDIR)
	{
		rcmd = (struct redircmd *)cmd;
		nulterminate(rcmd->cmd);
		*rcmd->efile = 0;
	}
	return (cmd);
}

struct cmd	*parse_redirs(struct cmd *cmd, char **ps, char *es)
{
	int		tok;
	char	*q;
	char	*eq;

	while (peek(ps, es, "<>"))
	{
		tok = get_token(ps, es, 0, 0);
		if (get_token(ps, es, &q, &eq) != 'a')
			ft_perror("Error: missing file for redirection");
		if (tok == '<')
		{
			cmd = redir_in_cmd(cmd, q, eq, O_RDONLY);
		}
		else if (tok == '>')
		{
			cmd = redir_out_cmd(cmd, q, eq, O_WRONLY | O_CREAT | O_TRUNC);
		}
		else if (tok == '+')
		{
			cmd = redir_out_cmd(cmd, q, eq, O_WRONLY | O_CREAT | O_APPEND);
		}
	}
	return (cmd);
}

struct cmd	*parse_exec(char **p_str, char *e_str, int argc)
{
	char			*q;
	char			*end_q;
	int				tok;
	struct execcmd	*struct_execcmd;
	struct cmd		*cmd;

	cmd = exec_cmd();
	struct_execcmd = (struct execcmd *)cmd;
	cmd = parse_redirs(cmd, p_str, e_str);
	while (!peek(p_str, e_str, "|"))
	{
		tok = get_token(p_str, e_str, &q, &end_q);
		if (!tok)
			break;
		if (tok != 'a')
			ft_perror("syntax!\n");
		struct_execcmd->argv[argc] = q;
		struct_execcmd->eargv[argc] = end_q;
		argc++;
		cmd = parse_redirs(cmd, p_str, e_str);
	}
	struct_execcmd->argv[argc] = NULL;
	struct_execcmd->eargv[argc] = NULL;
	return (cmd);
}

// struct cmd	*parse_exec(char **p_str, char *e_str)
// {
// 	char		*q;
// 	char		*end_q;
// 	int			tok;
// 	int			argc;
// 	struct cmd	*cmd;

// 	cmd = exec_cmd();
// 	argc = 0;
// 	cmd = parse_redirs(cmd, p_str, e_str);
// 	while (!peek(p_str, e_str, "|"))
// 	{
// 		tok = get_token(p_str, e_str, &q, &end_q);
// 		if (!tok)
// 			break ;
// 		if (tok != 'a')
// 			ft_perror("syntax!\n");
// 		((struct execcmd *)cmd)->argv[argc] = q;
// 		((struct execcmd *)cmd)->eargv[argc] = end_q;
// 		argc++;
// 		cmd = parse_redirs(cmd, p_str, e_str);
// 	}
// 	((struct execcmd *)cmd)->argv[argc] = NULL;
// 	((struct execcmd *)cmd)->eargv[argc] = NULL;
// 	return (cmd);
// }

struct cmd	*parse_pipe(char **p_str, char *end_str)
{
	struct cmd	*cmd;

	cmd = parse_exec(p_str, end_str, 0);
	if (peek(p_str, end_str, "|"))
	{
		get_token(p_str, end_str, NULL, NULL);
		cmd = pipe_cmd(cmd, parse_pipe(p_str, end_str));
	}
	return (cmd);
}

struct cmd	*parse_cmd(char *str)
{
	struct cmd	*cmd;
	char		*end_str;

	end_str = &str[ft_strlen(str)];
	cmd = parse_pipe(&str, end_str);
	nulterminate(cmd);
	return (cmd);
}
