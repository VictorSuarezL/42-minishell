#include <minishell.h>

void	process_variable_size(const char **str, char **envp, size_t *size)
{
	const char	*start = *str;
	char		varname[256];
	size_t		varname_len;
	char		*value;

	while (**str && (ft_isalnum(**str) || **str == '_'))
		(*str)++;
	varname_len = *str - start;
	ft_strncpy(varname, start, varname_len);
	varname[varname_len] = '\0';
	value = get_env_value(varname, envp);
	if (value)
		*size += ft_strlen(value);
}

void	process_single_character(const char **str, size_t *size)
{
	(*size)++;
	(*str)++;
}

void	process_single_quote(const char **str, size_t *size,
		int *in_single_quotes)
{
	*in_single_quotes = !*in_single_quotes;
	(*size)++;
	(*str)++;
}

void	process_escape_sequence(const char **str, size_t *size)
{
	(*size) += 2;
	(*str) += 2;
}

size_t	calculate_size(const char *str, char **envp)
{
	size_t	size;
	int		in_single_quotes;

	size = 0;
	in_single_quotes = 0;
	while (*str)
	{
		if (*str == '\'')
			process_single_quote(&str, &size, &in_single_quotes);
		else if (*str == '\\' && !in_single_quotes && *(str + 1) == '$')
		{
			process_escape_sequence(&str, &size);
		}
		else if (*str == '$' && !in_single_quotes)
		{
			str++;
			process_variable_size(&str, envp, &size);
		}
		else
		{
			process_single_character(&str, &size);
		}
	}
	return (size);
}
