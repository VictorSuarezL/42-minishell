#include <minishell.h>

char *get_delimiter(char **delimiter)
{
    char *start;

	start = *delimiter;
    if (*delimiter)
	{
		while (**delimiter == '<' || **delimiter == ' ')
			(*delimiter)++;
		start = *delimiter;
		while (**delimiter != ' ' && **delimiter != '\0')
			(*delimiter)++;
		if (**delimiter == ' ')
			**delimiter = '\0';
	}
	return (start);
}

int have_heredoc(char *str, char **delimiter)
{
	int i;

	i = 0;
	while (str[i] && str[i + 1])
	{
		if ((str[i] == '<') && (str[i + 1] == '<'))
		{
			if (!*delimiter)
				*delimiter = &str[i];
			**delimiter = str[i];
		}
		i++;
	}
	*delimiter = get_delimiter(delimiter);
	//treat_delimiter(delimiter);
	if (*delimiter && **delimiter)
		return (1);
	return (0);
}

void	create_file(char *info)
{
	int fd;
    
	fd = open(".heredoc.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
	{
        perror("open");
		free(info);
        exit(1);
    }
	ft_putstr_fd(info, fd);
}

void	launch_heredoc(char *delimiter)
{
	char	*line = NULL;
	char	*result = NULL;
	char	*temp = NULL;

	while (1)
	{
		ft_putstr_fd("> ", 1);
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
	if (temp)
	{
		create_file(temp);
		free(temp);
	}
}
