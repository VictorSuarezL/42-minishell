#include <minishell.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*  int main()
 {
		char cwd[1024];
		getcwd(cwd, sizeof(cwd));
		printf("Before: %s\n", cwd);
		cd_builtin("..");
		cd_builtin("-");
		getcwd(cwd, sizeof(cwd));
		printf("After: %s\n", cwd);
		pwd_builtin();
		return (0);
 }

void	patata(void)
{
	system("leaks minishell");
}

 int main(int argc, char **argv, char **envp)
 {
		char *prompt;
		while (1)
			printf("%s\n", prompt = display_prompt());
			free(prompt);
		return (0);
 }

int	main(void)
{
	char	info[1024] = "+cat << EOF ";
	char	*del;
	char	**env_copy;
	char	*variable_to_unset;
	char	input1[] = "echo Hello world";
	char	input2[] = "echo -n Hello world";
	char	input3[] = "echo -n -n-nHello world";
	char	input4[] = "echo -n -n -n \"Hello world\"";
	char	input5[] = "echo -n -n -n  Hello   world";

	if (have_heredoc(info, &del))
	{
		printf("%s\n", del);
		launch_heredoc(del);
	}
} */
// int main(int argc, char *argv[], char *envp[])
// {
// 	char **env_copy = copy_env(envp);
// 	if (!env_copy)
// 		return (printf("Failed to copy environment\n"),1);
// 	if (unset_builtin(env_copy, argv) == 1)
// 		return (printf("Failed to unset variable %s\n", "xd"),
// 			free_double(env_copy), 1);
// 	for (int i = 0; env_copy[i]; i++)
// 		printf("%s\n", env_copy[i]);
// 	free_double(env_copy);
// 	return (0);
// }

/* int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        return 1;
    }

    echo_builtin(argv[1]);

    return 0;
} */


// int main(int args, char **argv, char **env)
// {
// 	cd_builtin(argv[1], env);
// 	pwd_builtin();
// }

/* int	main(int argc, char *argv[], char **env)
{
	char **export = copy_env(env);

	if (export == NULL)
	{
		fprintf(stderr, "Failed to initialize export list\n");
		return (1);
	}
	if (argc == 1)
	{
		// If no arguments are provided, print the current export list
		export_builtin(NULL, &export);
	}
	else
	{
		export_builtin(argv + 1, &export);
		print_str(export);
	}
	for (char **ptr = export; *ptr != NULL; ptr++)
	{
		free(*ptr);
	}
	free(export);
	return (0);
} */

int main(int argc, char *argv[], char *envp[])
{
    const char *path = argv[1];
    if (cd_builtin(path, envp) == 1) {
        printf("Failed to change directory to %s\n", path);
        return 1;
    }

    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working directory: %s\n", cwd);
    } else {
        perror("getcwd");
    }	
	print_str(envp);
    return 0;
}

/* int main(int argc, char *argv[], char *envp[])
{
    const char *path = argv[1];
    if (cd_builtin(path, envp) == 1) {
        printf("Failed to change directory to %s\n", path);
        return 1;
    }

    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working directory: %s\n", cwd);
    } else {
        perror("getcwd");
    }	

    return 0;
} */


/* #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int main(int argc, char *argv[])
{
    // Combine all command line arguments into a single string
    size_t input_len = 0;
    for (int i = 1; i < argc; i++) {
        input_len += strlen(argv[i]) + 1;
    }

    char *input = malloc(input_len);
    if (!input) {
        perror("malloc");
        return 1;
    }

    input[0] = '\0';
    for (int i = 1; i < argc; i++) {
        strcat(input, argv[i]);
        if (i < argc - 1) {
            strcat(input, " ");
        }
    }
    echo_builtin(input);

    free(input);
    return 0;
} */

// int main(int argc, char *argv[], char *envp[])
// {
//     (void)argc;  // Suppress unused parameter warning
//     (void)argv;  // Suppress unused parameter warning
    
//     if (env_builtin(argv[1], envp) == 1) {
//         return 1;
//     }

//     return 0;
// }
