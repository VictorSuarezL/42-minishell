#include <minishell.h>

void env_builtin(char **env)
{
    while (*env)
    {
        ft_putstr_fd(*env, 1);
        ft_putchar_fd('\n', 1);
        env++;
    }
}
