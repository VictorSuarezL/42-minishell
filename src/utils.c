#include <minishell.h>

int	wait_status(void)
{
	int	status;
	int	out_status;

	out_status = 0;
	wait(&status);
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		out_status = WEXITSTATUS(status);
	else if (WIFEXITED(status) && WIFSIGNALED(status) != 0)
		out_status = WTERMSIG(status);
	else
		out_status = 0;
	return (out_status);
}

void	wait_pipe(void)
{
	int	status;
	int	i;

	i = 0;
	while (i < 2)
	{
		if (wait(&status) == -1)
		{
			perror("wait failed");
			exit(errno);
		}
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		{
			exit(WEXITSTATUS(status));
		}
		i++;
	}
}

int	peek(char **ps, char *es, char *toks)
{
	char	*s;
	int		i;
	int		length;

	i = 0;
	s = *ps;
	length = es - s;
	while (i < length && ft_strchr(" \t\r\n\v", s[i]))
		i++;
	*ps = &s[i];
	if (i < length && s[i] && ft_strchr(toks, s[i]))
		return (1);
	else
		return (0);
}

// Utils
int	save_fork(void)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		ft_perror("fork");
	return (pid);
}

// Utils
void	remove_quotes(t_execcmd *ecmd)
{
	int		i;
	// size_t	len;
	char *str;
	i = -1;
	while (ecmd->argv[++i])
	{
		str = ecmd->argv[i];
		quote_manager(str, 0, 0);
		pop_slash(str);
	}
}
