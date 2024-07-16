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

void	final_expand(char *str, char *result)
{
	ft_strcpy(str, result);
	free(result);
}

char	*expand_start(size_t *new_size, char *str, char **envp, char *result)
{
	*new_size = calculate_size(str, envp);
	result = malloc(*new_size + 1);
	if (!result)
		return (NULL);
	return (result);
}

void	setup_expand(const char **pos, char *str, char *result,
		int *in_single_quotes)
{
	*pos = str;
	*in_single_quotes = 0;
	check_result(result);
}

void	expand(char *str, char **envp)
{
	size_t		new_size;
	char		*result;
	const char	*pos;
	int			in_single_quotes;

	result = NULL;
	result = expand_start(&new_size, str, envp, result);
	setup_expand(&pos, str, result, &in_single_quotes);
	while (*pos)
	{
		if (*pos == '\'')
			handle_single_quote(&pos, &in_single_quotes, result);
		else if (*pos == '\\' && !in_single_quotes && *(pos + 1) == '$')
			handle_escaped_dollar(&pos, result);
		else if (*pos == '$' && !in_single_quotes)
		{
			if (!ft_isalnum(*(pos + 1)) && *(pos + 1) != '_')
				handle_regular_char(&pos, result);
			else
				handle_variable_expand(&pos, envp, result, new_size);
		}
		else
			handle_regular_char(&pos, result);
	}
	final_expand(str, result);
}
