#include <minishell.h>

int	processheredoc(char *input, char **env)
{
	char	*heredocstart;

	heredocstart = input;
	heredocstart = ft_strstr(heredocstart, "<<");
	while (heredocstart != NULL)
	{
		if (handlehredocprocess(heredocstart, input, env))
			return (1);
		heredocstart += 2;
		heredocstart = ft_strstr(heredocstart, "<<");
	}
	return (0);
}

void	deletefiles(void)
{
	char	nombrearchivo[256];
	char	heredoccountstr[10];
	int		i;
	char	*str;

	i = 1;
	while (1)
	{
		ft_strcpy(nombrearchivo, "archivo_creado_");
		str = ft_itoa(i);
		ft_strcpy(heredoccountstr, str);
		free(str);
		ft_strcat(nombrearchivo, heredoccountstr);
		ft_strcat(nombrearchivo, ".txt");
		if (unlink(nombrearchivo) == 0)
			i++;
		else
			break ;
	}
}

void	num_to_str(int num, char *str)
{
	int	i;
	int	rem;

	i = 0;
	if (num == 0)
	{
		str[i++] = '0';
		str[i] = '\0';
		return ;
	}
	while (num != 0)
	{
		rem = num % 10;
		str[i++] = rem + '0';
		num = num / 10;
	}
	str[i] = '\0';
}

void	reverse_str(char *str)
{
	int		start;
	int		end;
	char	temp;

	start = 0;
	end = 0;
	while (str[end] != '\0')
		end++;
	end--;
	while (start < end)
	{
		temp = str[start];
		str[start] = str[end];
		str[end] = temp;
		start++;
		end--;
	}
}

void	int_to_str(int num, char *str)
{
	num_to_str(num, str);
	reverse_str(str);
}
