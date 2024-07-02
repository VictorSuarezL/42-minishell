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
