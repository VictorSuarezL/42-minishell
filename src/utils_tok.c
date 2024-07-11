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

	skip_initial_quotes(str, &i, &j);
	while (str[i])
	{
		if (ft_strchr("\"'", str[i]) && ft_strchr(" \t\v\n\r", str[i - 1]))
		{
			quote = str[i];
			str[j++] = str[i++];
			while (str[i] && str[i] != quote)
				str[j++] = str[i++];
			str[j++] = str[i++];
		}
		else if (ft_strchr("\"'", str[i]) && (ft_isalpha(str[i - 1])
				|| ft_strchr("\"'", str[i - 1])))
		{
			quote = str[i++];
			while (str[i] && str[i] != quote)
				str[j++] = str[i++];
			i++;
		}
		else
			str[j++] = str[i++];
	}
	str[j] = '\0';
}