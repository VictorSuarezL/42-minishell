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
/*
 Si las comillas dobles no impiden la concatenación de cadenas adyacentes
 fuera de las comillas. Por lo tanto, se quitan las comillas. Sin embargo si
 al remover las comillas no se concatena texto se dejan las comillas en la cadena.

 Si estoy en un caracter que es comillas y la anterior es una letra

 Por ejemplo:
 'Hola' 'Mundo' -> 'Hola' 'Mundo'
 'e' 'c' 'h' 'o' -> 'e' 'c' 'h' 'o'
 'e''c''h''o' -> echo
 'e'cho hola '> mundo' -> echo hola '> mundo'
 'echo' hola mundo -> 'echo' hola mundo
 e'cho' 'hola' mundo -> echo 'hola' mundo
 */

void	copy_until_quoute(char *str, char quote, int *i, int *j)
{
	while (str[*i] && str[*i] != quote)
		str[(*j)++] = str[(*i)++];
}

// void if_find_quote(char *str, int *i, int *j, char quote)
// {
// 	if (ft_strchr(" \t\v\n\r", str[*i - 1]))
// 	{
// 		str[(*j)++] = str[(*i)++];
// 		copy_until_quoute(str, quote, i, j);
// 		if (str[*i] == '\0')
// 			str[(*j)++] = str[(*i)++];
// 		else
// 			(*i)++;
// 	}
// 	else if (ft_isalpha(str[*i - 1]) || ft_strchr("\"'", str[*i - 1]))
// 	{
// 		quote = str[(*i)++];
// 		copy_until_quoute(str, quote, i, j);
// 	}
// }

void manage_initial_quotes(char *str, int *i, int *j)
{
	int flag = 0;
	char quote;

	if (flag == 1)
	{
		(*i)++;
		flag = 0;
	}
	else if (*i == 0 && ft_strchr("\"'", str[*i]))
	{
		quote = str[(*i)++];
		flag = 1;
		copy_until_quoute(str, quote, i, j);
	}
}

void	quote_manager(char *str, int i, int j, int flag)
{
	char	quote;
	int		in_quote;
	
	while (str[i])
	{
		if (i == 0 && ft_strchr("\"'", str[i]))
		{
			manage_initial_quotes(str, &i, &j);
		}
		else if (ft_strchr("\"'", str[i]))
		{
			quote = str[i];
			if (ft_strchr(" \t\v\n\r", str[i - 1]))
			{
				str[j++] = str[i++];
				copy_until_quoute(str, quote, &i, &j);
				if (i == ft_strlen(str) - 1)
					str[j++] = str[i++];
				else
					i++;
			}
			else if (ft_isalpha(str[i - 1]) || ft_strchr("\"'", str[i - 1]))
			{
				quote = str[i++];
				copy_until_quoute(str, quote, &i, &j);
			}
		}
		else
			str[j++] = str[i++];
	}
	str[j] = '\0';
}
