#include <minishell.h>

void    exit_builtin(int status)
{
    ft_putstr_fd("exit\n", 1);
    exit(status);
}