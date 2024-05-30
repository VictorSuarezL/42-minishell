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
/* int main(int argc, char *argv[], char *envp[])
{
	env_copy = copy_env(envp);
	if (!env_copy)
		return (printf("Failed to copy environment\n"),1);
	printf("Original environment:\n");
	for (int i = 0; envp[i]; i++)
		printf("%s\n", envp[i]);
	variable_to_unset = argv[1];
	if (unset_builtin(env_copy, variable_to_unset) == -1)
		return (printf("Failed to unset variable %s\n", variable_to_unset),
			free_double(env_copy), 1);
	printf("\nEnvironment after unsetting %s:\n", variable_to_unset);
	for (int i = 0; env_copy[i]; i++)
		printf("%s\n", env_copy[i]);
	free_double(env_copy);
	return (0);
} */

/* int main ()
{
	if (pwd_builtin())
		printf("Ha fallado pwd\n");
} */

/* int main()
{
	printf("Testing input: \"%s\"\n", input1);
	printf("---------------------------------\n");
	echo_builtin(input1);
	printf("Testing input: \"%s\"\n", input2);
	printf("---------------------------------\n");
	echo_builtin(input2);
	printf("Testing input: \"%s\"\n", input3);
	printf("---------------------------------\n");
	echo_builtin(input3);
	printf("Testing input: \"%s\"\n", input4);
	printf("---------------------------------\n");
	echo_builtin(input4);
	printf("Testing input: \"%s\"\n", input5);
	printf("---------------------------------\n");
	echo_builtin(input5);
	return (0);
} */

/* int	export_builtin(char *variable, char ***export)
{
	int		count;
	char	**new_export;
	int		i;

	if (empty_string(variable))
	{
		print_str(*export);
		return (0);
	}
	else
	{
		count = 0;
		if (*export != NULL)
			while ((*export)[count] != NULL)
				count++;
		new_export = malloc(sizeof(char *) * (count + 2));
		if (new_export == NULL)
		{
			perror("Failed to allocate memory");
			return (1);
		}
		i = -1;
		while (++i < count)
			new_export[i] = (*export)[i];
		new_export[count] = ft_strdup(variable);
		if (new_export[count] == NULL)
		{
			perror("Failed to allocate memory for variable");
			free(new_export);
			return (1);
		}
		new_export[count + 1] = NULL;
		*export = new_export;
		return (0);
	}
} */

/* int main(int args, char **argv, char **env)
{
	cd_builtin(argv[1], env);
	pwd_builtin();
} */

int	main(int argc, char *argv[], char **env)
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
}