#include <minishell.h>

void	ft_swap(char **a, char **b)
{
	char	*temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

void	sort_strings(char **str)
{
	int	i;
	int	j;

	i = 0;
	while (str[i])
	{
		j = 0;
		while (str[j + 1])
		{
			if (ft_strcmp(str[j], str[j + 1]) > 0)
				ft_swap(&str[j], &str[j + 1]);
			j++;
		}
		i++;
	}
}

int	empty_string(char *str)
{
	return (str == NULL || *str == '\0');
}

void	print_str(char **str_array)
{
	if (str_array == NULL)
		return ;
	while (*str_array)
	{
        ft_printf("declare -x ");
		ft_printf("%s\n", *str_array);
		str_array++;
	}
}

int add_variable(char *variable, char ***export)
{
    int count = 0;
    char **new_export;
    char *key;
    char *value;
    char **current;

    key = ft_strdup(variable);
    value = ft_strchr(key, '=');
    if (value != NULL)
    {
        *value = '\0';
        value++;
    }
    if (*export != NULL)
    {
        for (current = *export; *current != NULL; current++)
        {
            if (ft_strncmp(*current, key, ft_strlen(key)) == 0 && ((*current)[ft_strlen(key)] == '=' || (*current)[ft_strlen(key)] == '\0'))
            {
                free(*current);
                *current = ft_strdup(variable);
                free(key);
                return (*current == NULL) ? 1 : 0;
            }
        }
        while ((*export)[count] != NULL)
            count++;
    }
    new_export = (char **)malloc(sizeof(char *) * (count + 2));
    if (new_export == NULL)
    {
        perror("Failed to allocate memory");
        free(key);
        return 1;
    }
    for (int i = 0; i < count; i++)
        new_export[i] = (*export)[i];
    if (*export != NULL)
        free(*export);
    if (value != NULL)
        new_export[count] = ft_strdup(variable);
    else
        new_export[count] = ft_strdup(key);
    if (new_export[count] == NULL)
    {
        perror("Failed to allocate memory for variable");
        free(key);
        free(new_export);
        return 1;
    }
    new_export[count + 1] = NULL;
    *export = new_export;
    free(key);
    return 0;
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
