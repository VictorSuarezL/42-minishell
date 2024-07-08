#include <minishell.h>

void	escape_d_chars(char *str, char *aux, int *i, int *j)
{
	aux[(*j)++] = str[(*i)++];
	while (str[*i] && str[*i] != '"')
	{
		if (ft_strchr("'<>|\\*", str[*i]))
		{
			aux[*j] = '\\';
			(*j)++;
		}
		aux[(*j)++] = str[(*i)++];
	}
	if (str[*i] == '"')
		aux[(*j)++] = str[(*i)++];
}

void	escape_s_chars(char *str, char *aux, int *i, int *j)
{
	aux[(*j)++] = str[(*i)++];
	while (str[*i] && str[*i] != '\'')
	{
		if (ft_strchr("$\"<>|\\*", str[*i]))
		{
			aux[*j] = '\\';
			(*j)++;
		}
		aux[(*j)++] = str[(*i)++];
	}
	if (str[*i] == '\'')
		aux[(*j)++] = str[(*i)++];
}

// Función principal para escapar caracteres especiales en una cadena
void	escape_special_chars(char *str)
{
	int		i;
	int		j;
	char	*aux;

	i = 0;
	j = 0;
	aux = malloc(strlen(str) * 2 + 1);
	if (!aux)
		ft_perror("Error in malloc");
	while (str[i])
	{
		if (str[i] == '"')
			escape_d_chars(str, aux, &i, &j);
		else if (str[i] == '\'')
			escape_s_chars(str, aux, &i, &j);
		else
			aux[j++] = str[i++];
	}
	aux[j] = '\0';
	ft_strcpy(str, aux);
	free(aux);
}

void	pop_slash(char *str)
{
	int		i;
	int		j;
	char	*aux;

	i = 0;
	j = 0;
	aux = malloc(ft_strlen(str));
	if (!aux)
		ft_perror("Error in malloc");
	while (str[i])
	{
		if (str[i] == '\\')
			i++;
		else
			aux[j++] = str[i++];
	}
	ft_strcpy(str, aux);
	free(aux);
}

void	replace_qmark(char *line, int exit_status)
{
	char	*pos;
	char	*exit_status_str;
	int		len;
	int		buffer_size;
	char	*buffer;

	len = strlen(line);
	exit_status_str = ft_itoa(exit_status);
	buffer_size = len + strlen(exit_status_str) - 2 + 1;
	buffer = malloc(sizeof(char) * buffer_size);
	if (!buffer)
		ft_perror("Error in malloc");
	pos = ft_strnstr(line, "$?", len);
	if (!pos || (line[0] == '$' && line[1] == '?'))
	{
		free(buffer);
		free(exit_status_str);
		return ;
	}
	ft_strlcpy(buffer, line, pos - line + 1);
	ft_strlcat(buffer, exit_status_str, buffer_size);
	ft_strlcat(buffer, pos + 2, buffer_size);
	ft_strcpy(line, buffer);
	free(buffer);
	free(exit_status_str);
}
