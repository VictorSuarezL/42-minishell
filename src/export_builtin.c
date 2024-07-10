#include <minishell.h>

int	add_variable(char *variable, char ***export)
{
	int		count;
	int		update_result;
	char	*key;
	char	*value;

	count = 0;
	split_variable(variable, &key, &value);
	if (!is_valid_variable_name(key))
	{
		ft_printf("export: %s: not a valid identifier\n", key);
		free(key);
		return (1);
	}
	if (*export != NULL)
	{
		update_result = find_and_update(*export, variable, key, value);
		if (update_result != -1)
		{
			free(key);
			return (update_result);
		}
		while ((*export)[count] != NULL)
			count++;
	}
	return (add_new_variable(export, variable, key, value));
}

int	export_builtin(char **variables, char ***export, char ***env)
{
	int	i;

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
			if (ft_strstr(*variables, "="))
				if (add_variable(*variables, env) != 0)
					i = 1;
			variables++;
		}
		sort_strings(*export);
		return (i);
	}
}
