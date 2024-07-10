#include <minishell.h>

void	split_variable(char *variable, char **key, char **value)
{
	*key = ft_strdup(variable);
	*value = ft_strchr(*key, '=');
	if (*value != NULL)
	{
		**value = '\0';
		(*value)++;
	}
}

int	find_and_update(char **export, char *variable, char *key, char *value)
{
	char	**current;
	size_t	key_len;
	char	*new_value;

	current = export;
	key_len = ft_strlen(key);
	while (*current != NULL)
	{
		if (ft_strncmp(*current, key, key_len) == 0
			&& ((*current)[key_len] == '=' || (*current)[key_len] == '\0'))
		{
			if (value != NULL || ft_strchr(variable, '=') != NULL)
			{
				free(*current);
				new_value = ft_strdup(variable);
				*current = new_value;
				if (*current == NULL)
					return (1);
			}
			return (0);
		}
		current++;
	}
	return (-1);
}

char	**allocate_new_export(int count)
{
	char	**new_export;

	new_export = (char **)malloc(sizeof(char *) * (count + 2));
	if (new_export == NULL)
		perror("Failed to allocate memory");
	return (new_export);
}

int	add_new_variable(char ***export, char *variable, char *key, char *value)
{
	char	**new_export;
	int		count;

	count = 0;
	if (*export != NULL)
		while ((*export)[count] != NULL)
			count++;
	new_export = allocate_new_export(count);
	if (new_export == NULL)
		return (free(key), 1);
	copy_export(new_export, *export, count);
	if (*export != NULL)
		free(*export);
	if (value != NULL)
		new_export[count] = ft_strdup(variable);
	else
		new_export[count] = ft_strdup(key);
	if (new_export[count] == NULL)
		return (perror("Failed to allocate memory for variable"), free(key),
			free(new_export), 1);
	new_export[count + 1] = NULL;
	*export = new_export;
	free(key);
	return (0);
}

int	is_valid_variable_name(const char *name)
{
	const char	*p;

	if (!name || (!ft_isalpha(name[0]) && name[0] != '_'))
		return (0);
	p = name;
	while (*p)
	{
		if (!ft_isalnum(*p) && *p != '_')
			return (0);
		p++;
	}
	return (1);
}
