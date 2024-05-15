#include <minishell.h>

int	env_builtin(char **env)
{
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
