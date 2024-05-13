#include <minishell.h>

void pwd_builtin(void)
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        printf("%s\n", cwd);
    else
    {
        perror("getcwd() error");
        exit(EXIT_FAILURE);
    }
}
