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

void quote_manager(char *str)
{
	int i = 0;
	int j = 0;
	int flag = 0;
	char quote;
	int in_quote = 0;

	char *aux = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!aux)
		ft_perror("Error in malloc");

	while(str[i])
	{
		if (flag == 1)
		{
			i++;
			flag = 0;
		}
		else if (i == 0 && ft_strchr("\"'", str[i]))
		{
			quote = str[i++];
			flag = 1;
			while (str[i] && str[i] != quote)
				aux[j++] = str[i++];
			// if (str[i] == quote)
			// 	i++;
			
		}
		else if (ft_strchr("\"'", str[i]))
		{
			if(ft_strchr(" \t\v\n\r", str[i-1]))
			{
				aux[j++] = str[i++];
				// quote = str[i++];
				while (str[i] && str[i] != quote)
					aux[j++] = str[i++];
				if (i == ft_strlen(str) - 1)
				{
					aux[j++] = str[i++];
				}
				else
				{
					i++;
				}
			}
			else if (ft_isalpha(str[i - 1]) && in_quote == 0)
			{
				quote = str[i++];
				while (str[i] && str[i] != quote)
					aux[j++] = str[i++];
				// if (str[i] == quote)
				// 	i++;
			}
			else if (ft_strchr("\"'", str[i - 1]) && in_quote == 0)
			{
				quote = str[i++];
				while (str[i] && str[i] != quote)
					aux[j++] = str[i++];
				// if (str[i] == quote)
				// 	i++;
			}
			// else
			// {
			// 	aux[j++] = str[i++];
			// 	in_quote = 0;
			// }
			
			// else
			// 	in_quote = 0;
			// while (str[i] && str[i] != quote)
			// 	aux[j++] = str[i++];
			// if (str[i] == quote)
			// 	i++;
		}
		else
			aux[j++] = str[i++];
	}
	printf("aux: %s\n", aux);
}
