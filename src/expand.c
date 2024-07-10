#include <minishell.h>

void	check_result(char *result)
{
	if (result == NULL)
	{
		ft_printf("Error: unable to allocate memory\n");
		exit(1);
	}
	else
	{
		result[0] = '\0';
	}
}

void	expand(char *str, char **envp)
{
	size_t		new_size;
	char		*result;
	const char	*pos;
	int			in_single_quotes;

	new_size = calculate_size(str, envp);
	result = malloc(new_size + 1);
	pos = str;
	in_single_quotes = 0;
	check_result(result);
	while (*pos)
	{
		if (*pos == '\'')
			handle_single_quote(&pos, &in_single_quotes, result);
		else if (*pos == '\\' && !in_single_quotes && *(pos + 1) == '$')
			handle_escaped_dollar(&pos, result);
		else if (*pos == '$' && !in_single_quotes)
			handle_variable_expand(&pos, envp, result, new_size);
		else
			handle_regular_char(&pos, result);
	}
	ft_strcpy(str, result);
	free(result);
}
