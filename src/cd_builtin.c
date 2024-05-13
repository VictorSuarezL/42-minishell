#include <minishell.h>

void cd_builtin(const char *path)
{
    if (path == NULL)
    {
        ft_putstr_fd("\n", 2);
        return;
    }
    if (chdir(path) == -1)
        perror("cd");
}