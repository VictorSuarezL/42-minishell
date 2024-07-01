#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
// Definiciones de tipos de comandos
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
struct			pipecmd
{
	int			type;
	struct cmd	*left;
	struct cmd	*right;
};
struct			listcmd
{
	int			type;
	struct cmd	*left;
	struct cmd	*right;
};
struct			backcmd
{
	int			type;
	struct cmd	*cmd;
};
int	save_fork(void); // Fork pero con panic en caso de fallo.
void			panic(char *);
struct cmd		*parsecmd(char *);
void			runcmd(struct cmd *) __attribute__((noreturn));
// Ejecuta cmd. Nunca retorna.
void	runcmd(struct cmd *cmd)
{
	int				p[2];
	struct backcmd	*bcmd;
	struct execcmd	*ecmd;
	struct listcmd	*lcmd;
	struct pipecmd	*pcmd;
	struct redircmd	*rcmd;

	if (cmd == 0)
		exit(1);
	switch (cmd->type)
	{
	default:
		panic("runcmd");
	case EXEC:
		ecmd = (struct execcmd *)cmd;
		if (ecmd->argv[0] == 0)
			exit(1);
		execvp(ecmd->argv[0], ecmd->argv);
		fprintf(stderr, "exec %s failed: %s\n", ecmd->argv[0], strerror(errno));
		break ;
	case REDIR:
		rcmd = (struct redircmd *)cmd;
		close(rcmd->fd);
		if (open(rcmd->file, rcmd->mode, 0644) < 0)
		{
			fprintf(stderr, "open %s failed: %s\n", rcmd->file,
				strerror(errno));
			exit(1);
		}
		runcmd(rcmd->cmd);
		break ;
	case LIST:
		lcmd = (struct listcmd *)cmd;
		if (save_fork() == 0)
			runcmd(lcmd->left);
		wait(0);
		runcmd(lcmd->right);
		break ;
	case PIPE:
		pcmd = (struct pipecmd *)cmd;
		if (pipe(p) < 0)
			panic("pipe");
		if (save_fork() == 0)
		{
			close(1);
			dup(p[1]);
			close(p[0]);
			close(p[1]);
			runcmd(pcmd->left);
		}
		if (save_fork() == 0)
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
		break ;
	case BACK:
		bcmd = (struct backcmd *)cmd;
		if (save_fork() == 0)
			runcmd(bcmd->cmd);
		break ;
	}
	exit(0);
}
int	getcmd(char *buf, int nbuf)
{
	write(2, "$ ", 2);
	memset(buf, 0, nbuf);
	if (fgets(buf, nbuf, stdin) == NULL)
		return (-1);
	return (0);
}
int	main(void)
{
	static char	buf[100] = "ls > test.txt";
	int			fd;

	// Asegurarse de que tres descriptores de archivo estén abiertos.
	while ((fd = open("/dev/tty", O_RDWR)) >= 0)
	{
		if (fd >= 3)
		{
			close(fd);
			break ;
		}
	}
	// Leer y ejecutar comandos de entrada.
	while (getcmd(buf, sizeof(buf)) >= 0)
	{
		if (buf[0] == 'c' && buf[1] == 'd' && buf[2] == ' ')
		{
			// Chdir debe ser llamado por el padre, no el hijo.
			buf[strlen(buf) - 1] = 0; // cortar \n
			if (chdir(buf + 3) < 0)
				fprintf(stderr, "cannot cd %s\n", buf + 3);
			continue ;
		}
		if (save_fork() == 0)
			runcmd(parsecmd(buf));
		wait(0);
	}
	exit(0);
}
void	panic(char *s)
{
	fprintf(stderr, "%s\n", s);
	exit(1);
}
int	save_fork(void)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		panic("fork");
	return (pid);
}
// Constructores
struct cmd	*execcmd(void)
{
	struct execcmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	memset(cmd, 0, sizeof(*cmd));
	cmd->type = EXEC;
	return ((struct cmd *)cmd);
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
struct cmd	*pipecmd(struct cmd *left, struct cmd *right)
{
	struct pipecmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	memset(cmd, 0, sizeof(*cmd));
	cmd->type = PIPE;
	cmd->left = left;
	cmd->right = right;
	return ((struct cmd *)cmd);
}
struct cmd	*listcmd(struct cmd *left, struct cmd *right)
{
	struct listcmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	memset(cmd, 0, sizeof(*cmd));
	cmd->type = LIST;
	cmd->left = left;
	cmd->right = right;
	return ((struct cmd *)cmd);
}
struct cmd	*backcmd(struct cmd *subcmd)
{
	struct backcmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	memset(cmd, 0, sizeof(*cmd));
	cmd->type = BACK;
	cmd->cmd = subcmd;
	return ((struct cmd *)cmd);
}
// Parsing
char			whitespace[] = " \t\r\n\v";
char			symbols[] = "<|>&;()";

int	gettoken(char **ps, char *es, char **q, char **eq)
{
	char	*s;
	int		ret;

	s = *ps;
	while (s < es && strchr(whitespace, *s))
		s++;
	if (q)
		*q = s;
	ret = *s;
	switch (*s)
	{
	case 0:
		break ;
	case '|':
	case '(':
	case ')':
	case ';':
	case '&':
	case '<':
		s++;
		break ;
	case '>':
		s++;
		if (*s == '>')
		{
			ret = '+';
			s++;
		}
		break ;
	default:
		ret = 'a';
		while (s < es && !strchr(whitespace, *s) && !strchr(symbols, *s))
			s++;
		break ;
	}
	if (eq)
		*eq = s;
	while (s < es && strchr(whitespace, *s))
		s++;
	*ps = s;
	return (ret);
}

/**
 * Function: peek
 * ----------------

	* This function is used to check if the current character in the string `s` is one of the
 * characters in the `toks` string. In summary,
	the `peek` function skips any leading whitespace

	* in the string and then checks if the next character is one of the specified tokens.

	* It updates `ps` to point to this character and returns a boolean value indicating whether the character is a token character.
 *
 * ps: A pointer to a pointer to the current character in the string `s`.
 * es: A pointer to the end of the string `s`.
 * toks: A string containing the characters to be checked.
 *
 * returns: 1 if the current character is one of the characters in `toks`,
	0 otherwise.
 */
int	peek(char **ps, char *es, char *toks)
{
	char	*s;

	s = *ps;
	while (s < es && strchr(whitespace, *s))
		s++;
	*ps = s;
	return (*s && strchr(toks, *s));
}
struct cmd		*parseline(char **, char *);
struct cmd		*parsepipe(char **, char *);
struct cmd		*parseexec(char **, char *);
struct cmd		*nulterminate(struct cmd *);

struct cmd	*parsecmd(char *s)
{
	char		*es;
	struct cmd	*cmd;

	es = s + strlen(s);
	cmd = parseline(&s, es);
	peek(&s, es, "");
	if (s != es)
	{
		fprintf(stderr, "leftovers: %s\n", s);
		panic("syntax");
	}
	nulterminate(cmd);
	return (cmd);
}

struct cmd	*parseline(char **ps, char *es)
{
	struct cmd	*cmd;

	cmd = parsepipe(ps, es);
	// Maneja comandos en segundo plano,
		// así cada vez que encuentre un & se llama a gettoken
	while (peek(ps, es, "&"))
	{
		gettoken(ps, es, 0, 0);
		cmd = backcmd(cmd);
	}
	if (peek(ps, es, ";"))
	{
		gettoken(ps, es, 0, 0);
		cmd = listcmd(cmd, parseline(ps, es));
	}
	return (cmd);
}

struct cmd	*parsepipe(char **ps, char *es)
{
	struct cmd	*cmd;

	cmd = parseexec(ps, es);
	if (peek(ps, es, "|"))
	{
		gettoken(ps, es, 0, 0);
		cmd = pipecmd(cmd, parsepipe(ps, es));
	}
	return (cmd);
}

struct cmd	*parseredirs(struct cmd *cmd, char **ps, char *es)
{
	int	tok;

	char *q, *eq;
	while (peek(ps, es, "<>"))
	{
		tok = gettoken(ps, es, 0, 0);
		if (gettoken(ps, es, &q, &eq) != 'a')
			panic("missing file for redirection");
		switch (tok)
		{
		case '<':
			cmd = redircmd(cmd, q, eq, O_RDONLY, 0);
			break ;
		case '>':
			cmd = redircmd(cmd, q, eq, O_WRONLY | O_CREAT | O_TRUNC, 1);
			break ;
		case '+': // >>
			cmd = redircmd(cmd, q, eq, O_WRONLY | O_CREAT | O_APPEND, 1);
			break ;
		}
	}
	return (cmd);
}

struct cmd	*parseblock(char **ps, char *es)
{
	struct cmd	*cmd;

	if (!peek(ps, es, "("))
		panic("parseblock");
	gettoken(ps, es, 0, 0);
	cmd = parseline(ps, es);
	if (!peek(ps, es, ")"))
		panic("syntax - missing )");
	gettoken(ps, es, 0, 0);
	cmd = parseredirs(cmd, ps, es);
	return (cmd);
}

struct cmd	*parseexec(char **ps, char *es)
{
	struct execcmd	*cmd;
	struct cmd		*ret;

	char *q, *eq;
	int tok, argc;
	if (peek(ps, es, "("))
		return (parseblock(ps, es));
	ret = execcmd();
	cmd = (struct execcmd *)ret;
	argc = 0;
	ret = parseredirs(ret, ps, es);
	while (!peek(ps, es, "|)&;"))
	{
		if ((tok = gettoken(ps, es, &q, &eq)) == 0)
			break ;
		if (tok != 'a')
			panic("syntax");
		cmd->argv[argc] = q;
		cmd->eargv[argc] = eq;
		// printf("argc: %d\n", argc);
		// printf("q: %s\n", q);
		// printf("eq: %s\n", eq);
		argc++;
		if (argc >= MAXARGS)
			panic("too many args");
		ret = parseredirs(ret, ps, es);
	}
	cmd->argv[argc] = 0;
	cmd->eargv[argc] = 0;
	return ret;
}
// NUL-terminate all the counted strings.

struct cmd	*nulterminate(struct cmd *cmd)
{
	int i;
	struct backcmd *bcmd;
	struct execcmd *ecmd;
	struct listcmd *lcmd;
	struct pipecmd *pcmd;
	struct redircmd *rcmd;
	if (cmd == 0)
		return 0;
	switch (cmd->type)
	{
	case EXEC:
		ecmd = (struct execcmd *)cmd;
		for (i = 0; ecmd->argv[i]; i++)
			*ecmd->eargv[i] = 0;
		break ;
	case REDIR:
		rcmd = (struct redircmd *)cmd;
		nulterminate(rcmd->cmd);
		*rcmd->efile = 0;
		break ;
	case PIPE:
		pcmd = (struct pipecmd *)cmd;
		nulterminate(pcmd->left);
		nulterminate(pcmd->right);
		break ;
	case LIST:
		lcmd = (struct listcmd *)cmd;
		nulterminate(lcmd->left);
		nulterminate(lcmd->right);
		break ;
	case BACK:
		bcmd = (struct backcmd *)cmd;
		nulterminate(bcmd->cmd);
		break ;
	}
	return cmd;
}