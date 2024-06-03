#include <minishell.h>

// Function to split the variable into key and value
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

// Function to find if the key already exists and update it
int	find_and_update(char **export, char *variable, char *key)
{
	for (char **current = export; *current != NULL; current++)
	{
		if (ft_strncmp(*current, key, ft_strlen(key)) == 0
			&& ((*current)[ft_strlen(key)] == '='
			|| (*current)[ft_strlen(key)] == '\0'))
		{
			free(*current);
			*current = ft_strdup(variable);
			return ((*current == NULL) ? 1 : 0);
		}
	}
	return (-1); // Not found
}

// Function to allocate new export array
char	**allocate_new_export(char **export, int count)
{
	char	**new_export;

	new_export = (char **)malloc(sizeof(char *) * (count + 2));
	if (new_export == NULL)
	{
		perror("Failed to allocate memory");
	}
	return (new_export);
}

// Function to add new variable to export array
int	add_new_variable(char ***export, char *variable, char *key, char *value,
		int count)
{
	char	**new_export;

	new_export = allocate_new_export(*export, count);
	if (new_export == NULL)
	{
		free(key);
		return (1);
	}
	copy_export(new_export, *export, count);
	if (*export != NULL)
		free(*export);
	new_export[count] = (value != NULL) ? ft_strdup(variable) : ft_strdup(key);
	if (new_export[count] == NULL)
	{
		perror("Failed to allocate memory for variable");
		free(key);
		free(new_export);
		return (1);
	}
	new_export[count + 1] = NULL;
	*export = new_export;
	free(key);
	return (0);
}

int	add_variable(char *variable, char ***export)
{
	int		count;
	int		update_result;
	char	*key;
	char	*value;

	count = 0;
	split_variable(variable, &key, &value);
	if (*export != NULL)
	{
		update_result = find_and_update(*export, variable, key);
		if (update_result != -1)
		{
			free(key);
			return (update_result);
		}
		while ((*export)[count] != NULL)
			count++;
	}
	return (add_new_variable(export, variable, key, value, count));
}

int	export_builtin(char **variables, char ***export)
{
	if (variables == NULL || *variables == NULL)
	{
		sort_strings(*export);
		print_str(*export);
		return (0);
	}
	else
	{
		while (*variables != NULL)
		{
			if (add_variable(*variables, export) != 0)
				return (1);
			variables++;
		}
		sort_strings(*export);
		return (0);
	}
}
