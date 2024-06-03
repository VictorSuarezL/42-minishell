#include <minishell.h>

// Function to skip the command part
int	skip_command(char *input, int i)
{
	while (input[i] != ' ' && input[i] != '\0')
		i++;
	return (i);
}

// Function to handle the -n option
int	handle_n_option(char *input, int i, int *line)
{
	while (input[i] == '-' && input[i + 1] == 'n' && (input[i + 2] == ' '
			|| input[i + 2] == 'n'))
	{
		*line = 0;
		i += 2;
		i = skip_spaces(input, i);
	}
	return (i);
}

// Function to handle escape sequences
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

// Function to handle output
void	handle_output(char *input, int i)
{
	while (input[i] != '\0')
	{
		if (input[i] != ' ')
		{
			i = handle_escape_sequences(input, i);
		}
		else
		{
			write(1, " ", 1);
			i = skip_spaces(input, i);
		}
	}
}

void	echo_builtin(char *input)
{
	int	i;
	int	line;

	i = 0;
	line = 1;
	i = skip_spaces(input, i);
	i = skip_command(input, i);
	i = skip_spaces(input, i);
	i = handle_n_option(input, i, &line);
	handle_output(input, i);
	if (line)
		write(1, "\n", 1);
}
