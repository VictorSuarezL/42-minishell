#include <minishell.h>

int	exit_builtin(void)
{
	if (write(1, "", 0) == 0)
	{
		ft_putstr_fd("exit\n", 1);
		return (0);
	}
	return (1);
}
