#include <minishell.h>

void	echo_builtin(char *input)
{
	int	i;
	int	line;

	i = 0;
	line = 1;
	i = skip_spaces(input, i);
	if (!is_echo_command(input, &i))
		return ;
	i = skip_spaces(input, i);
	i = handle_n_option(input, i, &line);
	handle_output(input, i);
	if (line)
		write(1, "\n", 1);
}
