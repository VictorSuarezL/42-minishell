#include <minishell.h>

char	*display_prompt(void)
{
	char	*input;

	input = readline("\033[32m$ \033[0m");
	if (input && ft_strcmp(input, "\n") != 0 && ft_strcmp(input, "") != 0)
		add_history(input);
	return (input);
}
