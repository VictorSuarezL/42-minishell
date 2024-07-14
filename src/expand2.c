#include <minishell.h>

void	process_variable_expand(const char **pos, char **envp, char *result,
		size_t new_size)
{
	char		varname[256];
	size_t		varname_len;
	char		*value;
	const char	*start;

	start = *pos;
	while (**pos && (ft_isalnum(**pos) || **pos == '_'))
		(*pos)++;
	varname_len = *pos - start;
	ft_strncpy(varname, start, varname_len);
	varname[varname_len] = '\0';
	value = get_env_value(varname, envp);
	if (value)
		ft_strncat(result, value, new_size - ft_strlen(result));
	free(value);
}

void	handle_single_quote(const char **pos, int *in_single_quotes,
		char *result)
{
	*in_single_quotes = !(*in_single_quotes);
	ft_strncat(result, *pos, 1);
	(*pos)++;
}

void	handle_escaped_dollar(const char **pos, char *result)
{
	ft_strncat(result, *pos, 2);
	*pos += 2;
}

void	handle_variable_expand(const char **pos, char **envp, char *result,
		size_t new_size)
{
	(*pos)++;
	process_variable_expand(pos, envp, result, new_size);
}

void	handle_regular_char(const char **pos, char *result)
{
	char	temp[2];

	temp[0] = **pos;
	temp[1] = '\0';
	ft_strncat(result, temp, 1);
	(*pos)++;
}
