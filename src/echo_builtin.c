#include <minishell.h>

void echo_builtin(char *input)
{
	int i = 0;
	int line = 1;
	// Skip leading spaces
	while (input[i] == ' ')
		i++;
	// Skip the "echo" command
	while (input[i] != ' ' && input[i] != '\0')
		i++;
	// Skip spaces after the "echo" command
	while (input[i] == ' ')
		i++;
	// Check for multiple -n options
	while (input[i] == '-' && input[i + 1] == 'n' && (input[i + 2] == ' ' || input[i + 2] == 'n')) {
		line = 0;
		i += 2;
		while (input[i] == ' ')
			i++;
	}
	// Print the remaining input
	while (input[i] != '\0') {
		if (input[i] == '\\' && input[i + 1] != '\0') {
			if (input[i + 1] == 'n') {
				ft_putchar_fd('\n', 1);
				i += 2;
				continue;
			} else if (input[i + 1] == 't') {
				ft_putchar_fd('\t', 1);
				i += 2;
				continue;
			}
		}
		if (input[i] != ' ') {
			ft_putchar_fd(input[i], 1);
			i++;
		} else {
			ft_putchar_fd(' ', 1);
			while (input[i] == ' ')
				i++;
		}
	}
	if (line)
		ft_putchar_fd('\n', 1);
}
