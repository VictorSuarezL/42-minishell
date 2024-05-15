#include <minishell.h>

char	*display_prompt(void)
{
	char	*input;

	input = readline("Minishell :) ");
	//validar input
	add_history(input);
	return (input);
}