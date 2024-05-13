#include <minishell.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// int main()
// {
//     char cwd[1024];
//     getcwd(cwd, sizeof(cwd));
//     printf("Before: %s\n", cwd);
//     cd_builtin("../Housing");
//     getcwd(cwd, sizeof(cwd));
//     printf("After: %s\n", cwd);
//     pwd_builtin();
//     return 0;
// }
int main(int argc, char **argv, char **envp)
{
    env_builtin(envp);
    exit_builtin(1);
    return 0;
}