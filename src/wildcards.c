#include <minishell.h>

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
		if (process_token(token))
		{
			wildcard_present = 1;
			pattern_found = 0;
			if (!expand_token(token, result, &pattern_found))
				add_buf(result, token);
			else
				any_pattern_found = 1;
		}
		else
			add_buf(result, token);
		token = ft_strtok(NULL, " ");
	}
	finalize_result(buf, result);
	return (wildcard_result(wildcard_present, any_pattern_found));
}

void	initialize_variables(char *result, int *pattern_found,
		int *any_pattern_found, int *wildcard_present)
{
	result[0] = '\0';
	*pattern_found = 0;
	*any_pattern_found = 0;
	*wildcard_present = 0;
}

int	process_token(char *token)
{
	if (ft_strchr(token, '*') != NULL)
		return (1);
	return (0);
}

int	expand_token(char *token, char *result, int *pattern_found)
{
	DIR				*d;
	struct dirent	*dir;

	d = opendir(".");
	if (d)
	{
		dir = readdir(d);
		while ((dir != NULL))
		{
			if (ft_strcmp(dir->d_name, ".") == 0 || ft_strcmp(dir->d_name,
					"..") == 0)
				continue ;
			if (match_pattern(token, dir->d_name))
			{
				ft_strcat(result, dir->d_name);
				ft_strcat(result, " ");
				*pattern_found = 1;
			}
		}
		closedir(d);
	}
	return (*pattern_found);
}

void	finalize_result(char *buf, char *result)
{
	size_t	len;

	len = ft_strlen(result);
	if (len > 0 && result[len - 1] == ' ')
		result[len - 1] = '\0';
	ft_strcpy(buf, result);
}

int	match_pattern(const char *pattern, const char *str)
{
	if (*pattern == '\0' && *str == '\0')
		return (1);
	if (*pattern == '*')
	{
		while (*(pattern + 1) == '*')
			pattern++;
		return (match_pattern(pattern + 1, str) || (*str
				&& match_pattern(pattern, str + 1)));
	}
	if (*pattern == *str)
		return (match_pattern(pattern + 1, str + 1));
	return (0);
}
