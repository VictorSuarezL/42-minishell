#include <minishell.h>

int	skip_command(char *input, int i)
{
	while (input[i] != ' ' && input[i] != '\0')
		i++;
	return (i);
}

int	is_echo_command(char *input, int *i)
{
	int		start;
	int		end;
	char	*command;

	start = *i;
	end = skip_command(input, start);
	command = strndup(input + start, end - start);
	if (ft_strcmp(command, "echo") != 0)
	{
		free(command);
		return (0);
	}
	free(command);
	*i = end;
	return (1);
}

int	handle_n_option(char *input, int i, int *line)
{
	int	j;

	while (input[i] == '-')
	{
		j = i + 1;
		while (input[j] == 'n')
			j++;
		if (input[j] == ' ' || input[j] == '\0')
		{
			*line = 0;
			i = j;
			i = skip_spaces(input, i);
		}
		else
			break ;
	}
	return (i);
}

int	handle_escape_sequences(char *input, int i)
{
	if (input[i] == '\\' && input[i + 1] != '\0')
	{
		if (input[i + 1] == 'n')
		{
			write(1, "\n", 1);
			i += 2;
		}
		else if (input[i + 1] == 't')
		{
			write(1, "\t", 1);
			i += 2;
		}
		else
		{
			write(1, &input[i], 1);
			i++;
		}
	}
	else
	{
		write(1, &input[i], 1);
		i++;
	}
	return (i);
}

void	handle_output(char *input, int i)
{
	while (input[i] != '\0')
	{
		if (input[i] != ' ')
			i = handle_escape_sequences(input, i);
		else
		{
			write(1, " ", 1);
			i = skip_spaces(input, i);
		}
	}
}
