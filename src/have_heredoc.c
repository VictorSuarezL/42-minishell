#include <minishell.h>

int	have_heredoc(char *str, int *pos)
{
	int	i;

	i = 0;
	while (str[i] && str[i + 1])
	{
		if ((str[i] == '<') && (str[i + 1] == '<'))
			*pos = i;
		i++;
	}
	if (*pos)
		return (1);
	return (0);
}

void	launch_heredoc(char *delimiter)
{
	char	*line = NULL;
	char	*result = NULL;
	char	*temp;

	while (1)
	{
		ft_putchar_fd('>', 1);
		line = get_next_line(1);
		if (!line)
		{
			ft_putstr_fd("Error!\n", 2);
			exit(1);
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free (line);
			break;
		}
		if (!result)
			result = ft_strdup(line);
		else
		{
			temp = ft_strjoin(result, line);
			free (result);
			result = temp;
		}
		free (line);
	}
	return (result);
}
