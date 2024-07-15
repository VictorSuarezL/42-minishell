#include <minishell.h>

static int	is_pipe_end(char *str)
{
	int	len;

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

static int	quote_counter(char *str)
{
	int	i;
	int	flag;
	int	flag_double;
	int	flag_single;

	i = 0;
	flag = 0;
	flag_double = 0;
	flag_single = 0;
	while (str[i])
	{
		if (str[i] == '\'' && flag_double == 0 && flag_single == 0)
			flag_single = 1;
		else if (str[i] == '\'' && flag_single == 1 && flag_double == 0)
			flag_single = 0;
		if (str[i] == '\"' && flag_single == 0 && flag_double == 0)
			flag_double = 1;
		else if (str[i] == '\"' && flag_double == 1 && flag_single == 0)
			flag_double = 0;
		i++;
	}
	if (flag_double || flag_single)
		flag = 1;
	return (flag);
}

int	is_metacharacter_end(char *str)
{
	int	len;

	if (!str || str[0] == '\0')
	{
		return (1);
	}
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

int	validator(char *str)
{
	if (!str[0] || !str)
		return (1);
	else if (quote_counter(str))
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
