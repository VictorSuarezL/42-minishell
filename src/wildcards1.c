#include <minishell.h>

int	is_escaped(char *token, char *pos)
{
	int	count;

	count = 0;
	while (pos > token && *(pos - 1) == '\\')
	{
		count++;
		pos--;
	}
	return ((count % 2) != 0);
}

void	add_buf(char *result, char *token)
{
	ft_strcat(result, token);
	ft_strcat(result, " ");
}

int	wildcard_result(int wildcard_present, int any_pattern_found)
{
	if (!wildcard_present)
		return (0);
	else if (any_pattern_found)
		return (1);
	else
		return (-1);
}

void	expand_wildcards_helper(char *token, char *result,
		int *wildcard_present, int *any_pattern_found)
{
	int	pattern_found;

	if (process_token(token))
	{
		*wildcard_present = 1;
		pattern_found = 0;
		if (!expand_token(token, result, &pattern_found))
			add_buf(result, token);
		else
			*any_pattern_found = 1;
	}
	else
		add_buf(result, token);
}

int	expand_wildcards(char *buf)
{
	char	result[1000];
	char	*token;
	int		pattern_found;
	int		any_pattern_found;
	int		wildcard_present;

	initialize_variables(result, &pattern_found, &any_pattern_found,
		&wildcard_present);
	token = ft_strtok(buf, " ");
	while (token != NULL)
	{
		expand_wildcards_helper(token, result, &wildcard_present,
			&any_pattern_found);
		token = ft_strtok(NULL, " ");
	}
	finalize_result(buf, result);
	return (wildcard_result(wildcard_present, any_pattern_found));
}
