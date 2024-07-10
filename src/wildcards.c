#include <minishell.h>

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
	char	*pos;

	pos = ft_strchr(token, '*');
	while (pos != NULL)
	{
		if (!is_escaped(token, pos))
			return (1);
		pos = ft_strchr(pos + 1, '*');
	}
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
		while (dir != NULL)
		{
			if (ft_strcmp(dir->d_name, ".") == 0 || ft_strcmp(dir->d_name,
					"..") == 0)
			{
				dir = readdir(d);
				continue ;
			}
			if (match_pattern(token, dir->d_name))
			{
				ft_strcat(result, dir->d_name);
				ft_strcat(result, " ");
				*pattern_found = 1;
			}
			dir = readdir(d);
		}
	}
	return (closedir(d), *pattern_found);
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
	if (*pattern == '\\' && *(pattern + 1) == '*')
		return (*str == '*' && match_pattern(pattern + 2, str + 1));
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
