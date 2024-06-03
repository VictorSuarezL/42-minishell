#include <minishell.h>

static int	empty_space_string(char *str)
{
	return (str == NULL || *str == '\0' || *str == ' ');
}

int	env_builtin(char *input, char **env)
{
	if (!empty_space_string(input))
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
