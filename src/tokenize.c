#include <minishell.h>

// char	*skip_quote(char *str, char *end, char quote)
// {
// 	str++;
// 	while (str < end && *str != quote)
// 	{
// 		str++;
// 	}
// 	if (str < end && *str == quote)
// 	{
// 		str++;
// 	}
// 	return (str);
// }

char	*skip_token(char *str, char *end)
{
	while (str < end && !ft_strchr(" \t\r\n\v", *str) && !ft_strchr("<|>",
			*str))
	{
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
	// *aux = skip_quote(*aux, end, quote);
	(*aux)++; //TODO TENER EN CUENTA ESTO
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

// Función principal refactorizada
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
	else if (*aux == '>')
		ret = handle_redirection(&aux);
	else if (*aux == '"' || *aux == '\'')
		ret = handle_quote(&aux, end_str);
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

// int	get_token(char **p_str, char *end_str, char **q, char **end_q)
// {
// 	char	*aux;
// 	char	quote;
// 	int		ret;
// 	int		i;

// 	aux = *p_str;
// 	while (aux < end_str && ft_strchr(" \t\r\n\v", *aux))
// 	{
// 		aux++;
// 	}
// 	if (q)
// 		*q = aux;
// 	ret = *aux;
// 	if (!*aux)
// 	{
// 	}
// 	else if (ft_strchr("|<", *aux))
// 	{
// 		aux++;
// 	}
// 	else if (*aux == '>')
// 	{
// 		aux++;
// 		if (*aux == '>')
// 		{
// 			ret = '+';
// 			aux++;
// 		}
// 	}
// 	else if (*aux == '\'')
// 	{
// 		int i = 0;

// 		aux++;
// 		// if (q)
// 		// 	*q = aux;
// 		ret = 'a';
// 		while (aux < end_str && *aux != '\'')
// 		{
// 			// printf("i = %i\t aux[i] = %c\n", i++, *aux);
// 			aux++;
// 		}
// 		if (*aux == '\'')
// 		{
// 			// printf("aux = %c\n", *aux);
// 			aux++;
// 		}
// 	}
// 	// else if (*aux == '"' || *aux == '\'')
// 	// {
// 	// 	aux++;
// 	// 	quote = *aux;
// 	// 	ret = 'a';
// 	// 	while (aux < end_str && *aux != quote)
// 	// 	{
// 	// 		aux++;
// 	// 	}
// 	// 	if (*aux == quote)
// 	// 	{
// 	// 		aux++;
// 	// 	}
// 	// }
// 	else
// 	{
// 		ret = 'a';
// 		while (aux < end_str && !ft_strchr(" \t\r\n\v", *aux)
// 			&& !ft_strchr("<|>", *aux))
// 			aux++;
// 	}
// 	if (end_q)
// 		*end_q = aux;
// 	while (aux < end_str && ft_strchr(" \t\r\n\v", *aux))
// 		aux++;
// 	*p_str = aux;
// 	return (ret);
// }
