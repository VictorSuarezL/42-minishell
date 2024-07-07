#include <minishell.h>

static int	valid_input_string(char *str)
{
	if (str == NULL)
		return (1);
	while (*str == ' ' || *str == '\t' || *str == '\n')
		str++;
	while (*str)
	{
		if (ft_strncmp(str, "env", 3) != 0)
			return (0);
		str += 3;
		while (*str == ' ' || *str == '\t' || *str == '\n')
			str++;
	}
	return (1);
}

int	env_builtin(char *input, char **env)
{
	if (!valid_input_string(input))
	{
		ft_printf("env: %s: No such file or directory\n", input);
		return (1);
	}
	if (env == NULL)
		return (1);
	while (*env)
	{
		ft_putstr_fd(*env, 1);
		ft_putchar_fd('\n', 1);
		env++;
	}
	return (0);
}

