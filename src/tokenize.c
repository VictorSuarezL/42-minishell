#include <minishell.h>

int	peek(char **ps, char *es, char *toks)
{
	char	*s;

	int i = 0; // Índice para el acceso a través del string
	s = *ps;
	int length = es - s; // Longitud máxima calculada basada en es
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

int	get_token(char **p_str, char *end_str, char **q, char **end_q)
{
	char	*aux;
	char	quote;
	int		ret;

	aux = *p_str;
	while (aux < end_str && ft_strchr(" \t\r\n\v", *aux))
	{
		aux++;
	}
	if (q)
		*q = aux;
	ret = *aux;
	if (!*aux)
	{

	}
	else if (ft_strchr("|<", *aux))
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
		int i = 0;
		aux++;
		quote = *aux;
		ret = 'a';
		aux++;
		while (aux+1 < end_str && *aux+1 != quote)
		{
			aux++;
		}
	}
	
	// else if (*aux == '"' || *aux == '\'')
	// {
	// 	aux++;
	// 	quote = *aux;
	// 	ret = 'a';
	// 	while (aux < end_str && *aux != quote)
	// 	{
	// 		aux++;
	// 	}
	// 	if (*aux == quote)
	// 	{
	// 		aux++;
	// 	}
	// }
	else
	{
		ret = 'a';
		while (aux < end_str && !ft_strchr(" \t\r\n\v", *aux) && !ft_strchr("<|>",
				*aux))
			aux++;
	}
	if (end_q)
		*end_q = aux;
	while (aux < end_str && ft_strchr(" \t\r\n\v", *aux))
		aux++;
	*p_str = aux;
	return (ret);
}