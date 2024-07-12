#include <minishell.h>

char	*skip_whitespace(char *str, char *end)
{
	while (str < end && ft_strchr(" \t\r\n\v", *str))
	{
		str++;
	}
	return (str);
}

void	ft_perror(char *msg)
{
	if (errno)
	{
		perror("Error");
		exit(errno);
	}
	else
	{
		ft_putendl_fd(msg, STDERR_FILENO);
		exit(1);
	}
}

void	skip_initial_quotes(char *str, int *i, int *j)
{
	if (ft_strchr("\"'", str[*i]))
	{
		(*i)++;
		while (str[*i] && !ft_strchr("\"'", str[*i]))
			str[(*j)++] = str[(*i)++];
		(*i)++;
	}
}

void	quote_manager(char *str, int i, int j)
{
	char	quote;

	while (str[i])
	{
		if (ft_strchr("\"'", str[i]))
		{
			quote = str[i++];
			while (str[i] && str[i] != quote)
				str[j++] = str[i++];
			if (str[i] == quote)
				i++;
		}
		else
			str[j++] = str[i++];
	}
	str[j] = '\0';
}
