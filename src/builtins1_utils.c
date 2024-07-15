#include <minishell.h>

void	copy_export(char **new_export, char **export, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		new_export[i] = export[i];
		i++;
	}
}

int	calculate_total_length(char **input)
{
	int	total_length;
	int	i;

	total_length = 0;
	i = 0;
	while (input[i] != NULL)
	{
		total_length += ft_strlen(input[i]);
		total_length += 1;
		i++;
	}
	return (total_length);
}

char	*join_strings_with_spaces(char **input)
{
	int		total_length;
	int		i;
	char	*result;

	total_length = 0;
	i = 0;
	if (input == NULL)
		return (NULL);
	total_length = calculate_total_length(input);
	if (total_length == 0)
		return (NULL);
	result = (char *)malloc(total_length * sizeof(char));
	if (result == NULL)
		return (NULL);
	result[0] = '\0';
	i = 0;
	while (input[i] != NULL)
	{
		ft_strcat(result, input[i]);
		i++;
		if (input[i] != NULL)
			ft_strcat(result, " ");
	}
	return (result);
}

void	builtin_exec(t_execcmd *ecmd, char **env_copy, char **export_copy)
{
	char	*input;
	int		exit_status;

	input = join_strings_with_spaces(ecmd->argv);
	exit_status = execute_builtin(input, &export_copy, &env_copy);
	free(ecmd);
	free(input);
	free_double(export_copy);
	free_double(env_copy);
	exit(exit_status);
}
