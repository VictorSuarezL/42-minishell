#include <minishell.h>

static int	is_pipe_end(char *str)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(str);
	if (str[len - 1] == '|')
	{
		return (1);
	}
	else
	{
		return (0);
	}
}

int	in_quote(char *str, int i, char c)
{
	int	j;
	int	flag;

	j = 0;
	flag = 0;
	while (str[j] && j < i)
	{
		if (str[j] == c && flag == 0)
		{
			flag = 1;
		}
		else if (str[j] == c && flag == 1)
		{
			flag = 0;
		}
		j++;
	}
	printf("in_quote = %i\n", flag);
	return (flag);
}

static int	quote_counter(char *str)
{
	int	i;
	int	flag;
	int	flag_a;
	int	flag_b;

	i = 0;
	flag = 0;
	flag_a = 0;
	flag_b = 0;
	while (str[i])
	{
		if (str[i] == '\'' && flag == 0 && in_quote(str, i, '"') == 0)
			flag = 1;
		else if (str[i] == '\'' && flag == 1 && in_quote(str, i, '"') == 0)
			flag = 0;
		if (str[i] == '\"' && flag == 0 && in_quote(str, i, '\'') == 0)
			flag = 1;
		else if (str[i] == '\"' && flag == 1 && in_quote(str, i, '\'') == 0)
			flag = 0;
		i++;
	}
	if (flag_a || flag_b)
		flag = 1;
	return (flag);
}

int	is_metacharacter_end(char *str)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	len = ft_strlen(str);
	if (str[len - 2] != '\\' && ft_strchr("<>|;", str[len - 1]))
	{
		return (1);
	}
	else
	{
		return (0);
	}
}

/**
 * Flag the number of single and double quotes in a given string.
 *
 * @param str The string to count quotes in.
 * @return The number of quotes found in the string.
 */
int	validator(char *str)
{
	if (quote_counter(str))
	{
		return (0);
	}
	else if (is_metacharacter_end(str))
	{
		return (0);
	}
	else if (is_pipe_end(str))
	{
		return (0);
	}
	else if (ft_strchr("|&", str[0]))
	{
		return (0);
	}
	else
	{
		return (1);
	}
}
