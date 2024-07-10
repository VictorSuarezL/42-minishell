#include <minishell.h>

int	procesarheredoc(char *input, char **env)
{
	char	*heredocstart;

	heredocstart = input;
	heredocstart = ft_strstr(heredocstart, "<<");
	while (heredocstart != NULL)
	{
		if (manejarprocesoheredoc(heredocstart, input, env))
			return (1);
		heredocstart += 2;
		heredocstart = ft_strstr(heredocstart, "<<");
	}
	return (0);
}

void	eliminararchivos(void)
{
	char	nombrearchivo[256];
	char	heredoccountstr[10];
	int		i;

	i = 1;
	while (1)
	{
		ft_strcpy(nombrearchivo, "archivo_creado_");
		int_to_str(i, heredoccountstr);
		ft_strcat(nombrearchivo, heredoccountstr);
		ft_strcat(nombrearchivo, ".txt");
		if (unlink(nombrearchivo) == 0)
			i++;
		else
			break ;
	}
}

void	int_to_str(int num, char *str)
{
	int	i;
	int	is_negative;

	i = 0;
	is_negative = 0;
	if (num == 0)
	{
		str[i++] = '0';
		str[i] = '\0';
		return ;
	}
	if (num < 0)
	{
		is_negative = 1;
		num = -num;
	}
	while (num != 0)
	{
		int rem = num % 10;
		str[i++] = rem + '0';
		num = num / 10;
	}
	if (is_negative)
		str[i++] = '-';
	str[i] = '\0';
	int start = 0;
	int end = i - 1;
	while (start < end)
	{
		char temp = str[start];
		str[start] = str[end];
		str[end] = temp;
		start++;
		end--;
	}
}
