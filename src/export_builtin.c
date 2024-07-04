#include <minishell.h>
#include <string.h>

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
int find_and_update(char **export, char *variable, char *key)
{
    char **current;
    size_t key_len;
    char *new_value;

    current = export;
    key_len = ft_strlen(key);
    while (*current != NULL)
	{
        if (ft_strncmp(*current, key, key_len) == 0 &&
            ((*current)[key_len] == '=' || (*current)[key_len] == '\0')) {
            free(*current);
            new_value = ft_strdup(variable);
            *current = new_value;

            if (*current == NULL)
                return (1);
            return (0);
        }
        current++;
    }
    return (-1); // Not found
}


// Function to allocate new export array
char	**allocate_new_export(int count)
{
	char	**new_export;

	new_export = (char **)malloc(sizeof(char *) * (count + 2));
	if (new_export == NULL)
		perror("Failed to allocate memory");
	return (new_export);
}

// Function to add new variable to export array
int	add_new_variable(char ***export, char *variable, char *key, char *value, int count)
{
	char	**new_export;

	new_export = allocate_new_export(count);
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

int is_valid_variable_name(const char *name)
{
    const char *p;

    // Verificar si el nombre es nulo o no comienza con una letra o '_'
    if (!name || (!ft_isalpha(name[0]) && name[0] != '_'))
        return 0;
    // Verificar el resto de los caracteres
    p = name;
    while (*p)
	{
        if (!ft_isalnum(*p) && *p != '_')
            return (0);
        p++;
    }
    return (1);
}


int add_variable(char *variable, char ***export)
{
    int count;
    int update_result;
    char *key;
    char *value;

    count = 0;
    split_variable(variable, &key, &value);
    if (!is_valid_variable_name(key))// Validate variable name
	{
		ft_printf("export: %s: not a valid identifier\n", key);
        free(key);
        return (1); // Invalid variable name
    }
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

int	export_builtin(char **variables, char ***export, char ***env)
{
	int i;

	i = 0;
	if (variables == NULL || *variables == NULL || *(variables + 1) == NULL)
	{
		sort_strings(*export);
		print_str(*export);
		return (0);
	}
	else
	{
		variables++;
		while (*variables != NULL)
		{
			if (add_variable(*variables, export) != 0)
				i = 1;
			if (strstr(*variables, "="))
				if (add_variable(*variables, env) != 0)
					i = 1;
			variables++;
		}
		sort_strings(*export);
		return (i);
	}
}
