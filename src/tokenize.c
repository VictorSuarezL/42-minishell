#include <minishell.h>

char	*skip_token(char *str, char *end)
{
	int	escaped;

	escaped = 0;
	while (str < end && (!ft_strchr(" \t\r\n\v", *str) || escaped == 1)
		&& (!ft_strchr("<>|", *str) || escaped == 1))
	{
		if (escaped == 1)
		{
			escaped = 0;
		}
		if (*str == '\\')
		{
			escaped = 1;
		}
		str++;
	}
	return (str);
}

int	handle_redirection(char **aux)
{
	int	ret;

	ret = **aux;
	(*aux)++;
	if (**aux == '>')
	{
		ret = '+';
		(*aux)++;
	}
	return (ret);
}

int	handle_quote(char **aux, char *end)
{
	char	quote;

	quote = **aux;
	(*aux)++;
	while (*aux < end && **aux != quote)
	{
		(*aux)++;
	}
	if (*aux < end && **aux == quote)
	{
		(*aux)++;
	}
	return ('a');
}

int	get_special_token(char **aux)
{
	int	ret;

	ret = **aux;
	(*aux)++;
	return (ret);
}

int	get_token(char **p_str, char *end_str, char **q, char **end_q)
{
	char	*aux;
	int		ret;

	ret = 0;
	aux = skip_whitespace(*p_str, end_str);
	if (q)
		*q = aux;
	if (*aux == '\0')
		ret = '\0';
	else if (ft_strchr("|<", *aux))
		ret = get_special_token(&aux);
	else if (*aux == '>' && *(aux - 1) != '\\')
		ret = handle_redirection(&aux);
	else
	{
		ret = 'a';
		aux = skip_token(aux, end_str);
	}
	if (end_q)
		*end_q = aux;
	*p_str = skip_whitespace(aux, end_str);
	return (ret);
}
