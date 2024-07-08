#include <minishell.h>

int		g_signal;

void	process(char *buf)
{
	procesarredirecciones(buf);
	expand_wildcards(buf);
	pop_slash(buf);
}

void	process_commands(char *trimmed, char *buf, char ***copy_en,
		char ***copy_export, int *exit_status)
{
	int	a;

	ft_strcpy(buf, trimmed);
	quote_manager(buf, 0, 0);
	escape_special_chars(buf);
	if (procesarHeredoc(buf, *copy_en) == 1)
	{
		eliminarArchivos();
		return ;
	}
	replace_qmark(buf, *exit_status);
	expand(buf, *copy_en);
	process(buf);
	a = execute_cd(buf, *copy_en, *copy_export);
	if (a == 0 || a == 1)
		return ;
	if (is_builtin_env(buf) && (!ft_strstr(buf, "|") && !ft_strstr(buf, ">")
			&& !ft_strstr(buf, "<")))
	{
		execute_builtin(buf, copy_export, copy_en);
		if (ft_strcmp(buf, "exit") == 0 || ft_strncmp(buf, "exit ", 5) == 0)
			exit(0);
	}
	else
		setup_executor(buf, *copy_en, *copy_export, exit_status);
}

void	process_input(char *input, char ***copy_en, char ***copy_export,
		int *exit_status)
{
	char	buf[PATH_MAX];
	char	*trimmed;

	if (ft_strlen(input) > PATH_MAX - 1)
	{
		free(input);
		return ;
	}
	trimmed = ft_strtrim(input, " ");
	free(input);
	if (trimmed == NULL || trimmed[0] == '\0')
	{
		free(trimmed);
		return ;
	}
	if (!validator(trimmed))
	{
		ft_printf("syntax error near unexpected token `newline'\n");
		free(trimmed);
		return ;
	}
	process_commands(trimmed, buf, copy_en, copy_export, exit_status);
	free(trimmed);
}

int	main(int args, char **argv, char **env)
{
	char	**copy_export;
	char	**copy_en;
	char	*input;
	int		exit_status;

	exit_status = 0;
	(void)args;
	argv++;
	setup_shell(&copy_export, &copy_en, env);
	while (1)
	{
		g_signal = 1;
		input = display_prompt();
		if (input == NULL)
		{
			exit_builtin();
			break ;
		}
		process_input(input, &copy_en, &copy_export, &exit_status);
	}
	final_clean(copy_export, copy_en);
	return (exit_status);
}
