#include <minishell.h>

char	*display_prompt(void)
{
	char	*input;

	input = readline("Minishell :) ");
	if (input && ft_strcmp(input, "\n") != 0 && ft_strcmp(input, "") != 0)
		add_history(input);
	return (input);
}
