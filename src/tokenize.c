#include <minishell.h>

int	peek(char **ps, char *es, char *toks)
{
	char	*s;
	int		i;
	int		length;

	i = 0;
	s = *ps;
	length = es - s; // Longitud máxima calculada basada en es
	// Salta caracteres de espacio en blanco
	while (i < length && ft_strchr(" \t\r\n\v", s[i]))
		i++;
	*ps = &s[i]; // Usando &s[i] para la claridad
	// Verifica si el caracter actual está dentro de los toks buscados
	if (i < length && s[i] && ft_strchr(toks, s[i]))
		return (1);
	else
		return (0);
}

#include <stdio.h>

// Funciones auxiliares
int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\v');
}

char	*skip_whitespace(char *str, char *end)
{
	while (str < end && is_whitespace(*str))
	{
		str++;
	}
	return (str);
}

char	*skip_quote(char *str, char *end, char quote)
{
	str++;
	while (str < end && *str != quote)
	{
		str++;
	}
	if (str < end && *str == quote)
	{
		str++;
	}
	return (str);
}

char	*skip_token(char *str, char *end)
{
	while (str < end && !is_whitespace(*str) && !ft_strchr("<|>", *str))
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
	*aux = skip_quote(*aux, end, quote);
	return ('a');
}

// Función principal refactorizada
int	get_token(char **p_str, char *end_str, char **q, char **end_q)
{
	char	*aux;
	int		ret;

	aux = *p_str;
	ret = 0;
	aux = skip_whitespace(aux, end_str);
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
	aux = skip_whitespace(aux, end_str);
	*p_str = aux;
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
// 		i = 0;
// 		aux++;
// 		quote = *aux;
// 		ret = 'a';
// 		aux++;
// 		while (aux < end_str && *aux + 1 != '\'')
// 		{
// 			aux++;
// 		}
// 	}
// 	else if (*aux == '"' || *aux == '\'')
// 	{
// 		aux++;
// 		quote = *aux;
// 		ret = 'a';
// 		while (aux < end_str && *aux != quote)
// 		{
// 			aux++;
// 		}
// 		if (*aux == quote)
// 		{
// 			aux++;
// 		}
// 	}
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