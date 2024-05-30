#include <minishell.h>

int	exit_builtin(void)
{
	if (write(1, "", 0) == 0)
		return (ft_putstr_fd("exit\n", 1), 0);
	return (1);
}
