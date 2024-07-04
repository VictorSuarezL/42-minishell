#include <minishell.h>

t_cmd	*nulterminate(t_cmd *cmd)
{
	t_execcmd	*ecmd;
	t_pipecmd	*pcmd;
	t_redircmd	*rcmd;
	int			i;

	i = -1;
	if (cmd->type == EXEC)
	{
		ecmd = (t_execcmd *)cmd;
		while (ecmd->argv[++i])
			*ecmd->eargv[i] = 0;
	}
	else if (cmd->type == PIPE)
	{
		pcmd = (t_pipecmd *)cmd;
		nulterminate(pcmd->left);
		nulterminate(pcmd->right);
	}
	else if (cmd->type == REDIR)
	{
		rcmd = (t_redircmd *)cmd;
		nulterminate(rcmd->cmd);
		*rcmd->efile = 0;
	}
	return (cmd);
}

t_cmd	*parse_redirs(t_cmd *cmd, char **ps, char *es)
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

t_cmd	*parse_exec(char **p_str, char *e_str, int argc)
{
	char		*q;
	char		*end_q;
	int			tok;
	t_execcmd	*struct_execcmd;
	t_cmd		*cmd;

	cmd = exec_cmd();
	struct_execcmd = (t_execcmd *)cmd;
	cmd = parse_redirs(cmd, p_str, e_str);
	while (!peek(p_str, e_str, "|"))
	{
		tok = get_token(p_str, e_str, &q, &end_q);
		if (!tok)
			break ;
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

t_cmd	*parse_pipe(char **p_str, char *end_str)
{
	t_cmd	*cmd;

	cmd = parse_exec(p_str, end_str, 0);
	if (peek(p_str, end_str, "|"))
	{
		get_token(p_str, end_str, NULL, NULL);
		cmd = pipe_cmd(cmd, parse_pipe(p_str, end_str));
	}
	return (cmd);
}

t_cmd	*parse_cmd(char *str)
{
	t_cmd	*cmd;
	char	*end_str;

	end_str = &str[ft_strlen(str)];
	cmd = parse_pipe(&str, end_str);
	nulterminate(cmd);
	// printf("cmd->type = %d\n", cmd->type);
	return (cmd);
}
