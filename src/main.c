#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define EXEC 1
#define REDIR 2
#define PIPE 3
#define LIST 4
#define BACK 5
#define MAXARGS 10

struct			cmd
{
	int			type;
};
struct			pipecmd
{
	int			type;
	struct cmd	*left;
	struct cmd	*right;
};
struct			execcmd
{
	int			type;
	char		*argv[MAXARGS];
	char		*eargv[MAXARGS];
};
struct			redircmd
{
	int			type;
	struct cmd	*cmd;
	char		*file;
	char		*efile;
	int			mode;
	int			fd;
};

char			whitespace[] = " \t\r\n\v";
char			symbols[] = "<|>&;()";

int	fork1(void)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		printf("fork");
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


void	runcmd(struct cmd *cmd)
{
	int				p[2];
	struct execcmd	*ecmd;
	struct pipecmd	*pcmd;
	struct redircmd	*rcmd;

	if (!cmd)
	{
		exit(1);
	}
	if (cmd->type == EXEC)
	{
		ecmd = (struct execcmd *)cmd;
		if (!ecmd->argv[0])
			exit(1);
		execvp(ecmd->argv[0], ecmd->argv);
        execve(ecmd->argv[0], ecmd->argv, )
	}
	else if (cmd->type == PIPE)
	{
		pcmd = (struct pipecmd *)cmd;
		if (pipe(p) < 0)
			printf("pipe");
		if (fork1() == 0)
		{
			close(1);
			dup(p[1]);
			close(p[0]);
			close(p[1]);
			runcmd(pcmd->left);
		}
		if (fork1() == 0)
		{
			close(0);
			dup(p[0]);
			close(p[0]);
			close(p[1]);
			runcmd(pcmd->right);
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
		runcmd(rcmd->cmd);
	}
	exit(0);
}

// Constructores
struct cmd	*exec_cmd(void)
{
	struct execcmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	memset(cmd, 0, sizeof(*cmd));
	cmd->type = EXEC;
	return ((struct cmd *)cmd);
}

struct cmd	*pipe_cmd(struct cmd *left, struct cmd *right)
{
	struct pipecmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	memset(cmd, 0, sizeof(*cmd));
	cmd->type = PIPE;
	cmd->left = left;
	cmd->right = right;
	return ((struct cmd *)cmd);
}

struct cmd	*nulterminate(struct cmd *cmd)
{
	struct execcmd	*ecmd;
	struct pipecmd	*pcmd;
	struct redircmd	*rcmd;
	int				i;

	i = 0;
	if (cmd->type == EXEC)
	{
		ecmd = (struct execcmd *)cmd;
		while (ecmd->argv[i])
		{
			*ecmd->eargv[i] = 0;
			i++;
		}
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

int	peek(char **ps, char *es, char *toks)
{
	char	*s;

	int i = 0; // Índice para el acceso a través del string
	s = *ps;
	int length = es - s; // Longitud máxima calculada basada en es
	// Salta caracteres de espacio en blanco
	while (i < length && strchr(" \t\r\n\v", s[i]))
		i++;
	*ps = &s[i]; // Usando &s[i] para la claridad
	// Verifica si el caracter actual está dentro de los toks buscados
	if (i < length && s[i] && strchr(toks, s[i]))
		return (1);
	else
		return (0);
}

int	get_token(char **p_str, char *end_str, char **q, char **end_q)
{
	char	*aux;
	char	quote;
	int		ret;

	aux = *p_str;
	while (aux < end_str && strchr(whitespace, *aux))
	{
		aux++;
	}
	if (q)
		*q = aux;
	ret = *aux;
	if (*aux == 0)
	{
	}
	else if (strchr("|()<", *aux))
	{
		aux++;
	}
	else if (*aux == '>')
	{
		aux++;
		if (*aux == '>')
		{
			ret = '+';
			aux++;
		}
	}
	else if (*aux == '"' || *aux == '\'')
	{
		aux++;
		quote = *aux;
		ret = 'a';
		while (aux < end_str && *aux != quote)
		{
			aux++;
		}
		if (*aux == quote)
		{
			aux++;
		}
	}
	else
	{
		ret = 'a';
		while (aux < end_str && !strchr(whitespace, *aux) && !strchr(symbols,
				*aux))
			aux++;
	}
	if (end_q)
		*end_q = aux;
	while (aux < end_str && strchr(whitespace, *aux))
		aux++;
	*p_str = aux;
	return (ret);
}

struct cmd	*redircmd(struct cmd *subcmd, char *file, char *efile, int mode,
		int fd)
{
	struct redircmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	memset(cmd, 0, sizeof(*cmd));
	cmd->type = REDIR;
	cmd->cmd = subcmd;
	cmd->file = file;
	cmd->efile = efile;
	cmd->mode = mode;
	cmd->fd = fd;
	return ((struct cmd *)cmd);
}

struct cmd	*parseredirs(struct cmd *cmd, char **ps, char *es)
{
	int	tok;

	char *q, *eq;
	while (peek(ps, es, "<>"))
	{
		tok = get_token(ps, es, 0, 0);
		if (get_token(ps, es, &q, &eq) != 'a')
			printf("missing file for redirection");
		if (tok == '<')
		{
			cmd = redircmd(cmd, q, eq, O_RDONLY, 0);
		}
		else if (tok == '>')
		{
			cmd = redircmd(cmd, q, eq, O_WRONLY | O_CREAT | O_TRUNC, 1);
		}
		else if (tok == '+')
		{
			cmd = redircmd(cmd, q, eq, O_WRONLY | O_CREAT | O_APPEND, 1);
		}
	}
	return (cmd);
}

struct cmd	*parse_exec(char **p_str, char *e_str)
{
	char			*q;
	char			*end_q;
	int				tok;
	int				argc;
	struct execcmd	*struct_execcmd;
	struct cmd		*cmd;

	cmd = exec_cmd();
	struct_execcmd = (struct execcmd *)cmd;
	argc = 0;
	cmd = parseredirs(cmd, p_str, e_str);
	while (!peek(p_str, e_str, "|"))
	{
		tok = get_token(p_str, e_str, &q, &end_q);
		if (tok == 0)
		{
			break ;
		}
		if (tok != 'a')
		{
			printf("syntax!\n");
		}
		struct_execcmd->argv[argc] = q;
		struct_execcmd->eargv[argc] = end_q;
		argc++;
		cmd = parseredirs(cmd, p_str, e_str);
	}
	struct_execcmd->argv[argc] = NULL;
	struct_execcmd->eargv[argc] = NULL;
	// nulterminate(cmd);
	return (cmd);
}

struct cmd	*parse_pipe(char **p_str, char *end_str)
{
	struct cmd	*cmd;

	cmd = parse_exec(p_str, end_str);
	if (peek(p_str, end_str, "|"))
	{
		get_token(p_str, end_str, 0, 0);
		cmd = pipe_cmd(cmd, parse_pipe(p_str, end_str));
	}
	return (cmd);
}

struct cmd	*parse_cmd(char *str)
{
	struct cmd	*cmd;
	char		*end_str;

	end_str = &str[strlen(str)];
	cmd = parse_pipe(&str, end_str);
	nulterminate(cmd);
	return (cmd);
}

int	main(void)
{
	char line[100] = "ls > e.txt > f.txt";

	runcmd(parse_cmd(line));
}