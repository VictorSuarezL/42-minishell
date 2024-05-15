#include <minishell.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// int main()
// {
//     char cwd[1024];
//     getcwd(cwd, sizeof(cwd));
//     printf("Before: %s\n", cwd);
//     cd_builtin("..");
//     cd_builtin("-");
//     getcwd(cwd, sizeof(cwd));
//     printf("After: %s\n", cwd);
//     pwd_builtin();
//     return 0;
// }

//void patata()
//{
//    system("leaks minishell");
//}

//int main(int argc, char **argv, char **envp)
//{
//    char *prompt; 
//    while (1)
//        printf("%s\n", prompt = display_prompt());
//        free(prompt);
//    return (0);
//}

//int main()
//{
//    char info[1024] = "+cat << EOF ";
//    char    *del;
//    if (have_heredoc(info, &del))
//    {
//        printf("%s\n", del);
//        launch_heredoc(del);
//    }
//}

//int main(int argc, char *argv[], char *envp[])
//{
//    char **env_copy = copy_env(envp);
//    if (!env_copy)
//        return (printf("Failed to copy environment\n"),1);
//    printf("Original environment:\n");
//    for (int i = 0; envp[i]; i++)
//        printf("%s\n", envp[i]);
//    char *variable_to_unset = argv[1];
//    if (unset_builtin(env_copy, variable_to_unset) == -1)
//        return (printf("Failed to unset variable %s\n", variable_to_unset), free_double(env_copy), 1);
//    printf("\nEnvironment after unsetting %s:\n", variable_to_unset);
//    for (int i = 0; env_copy[i]; i++)
//        printf("%s\n", env_copy[i]);
//    free_double(env_copy);
//    return 0;
//}

//int main ()
//{
//    echo_builtin("        echo -n -n h -n Hola         mundo");
//}

//int main ()
//{
//    if (pwd_builtin())
//        printf("Ha fallado pwd\n");
//}