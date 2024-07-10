#include <minishell.h>

void	process(char *buf)
{
	procesarredirecciones(buf);
	expand_wildcards(buf);
	pop_slash(buf);
}

void	handle_special_chars(char *buf, char ***copy_en, int *exit_status)
{
	quote_manager(buf, 0, 0);
	escape_special_chars(buf);
	if (procesarheredoc(buf, *copy_en) == 1)
	{
		eliminararchivos();
		return ;
	}
	replace_qmark(buf, *exit_status);
	expand(buf, *copy_en);
	process(buf);
}

void	handle_execution(char *buf, char ***copy_en, char ***copy_export,
		int *exit_status)
{
	int	a;

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

void	process_commands(char *buf, char ***copy_en, char ***copy_export,
		int *exit_status)
{
	handle_special_chars(buf, copy_en, exit_status);
	handle_execution(buf, copy_en, copy_export, exit_status);
}

void	process_input(char *input, char ***copy_en, char ***copy_export,
		int *exit_status)
{
	char	buf[4096];
	char	*trimmed;

	if (ft_strlen(input) > 4096 - 1)
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
	ft_strcpy(buf, trimmed);
	process_commands(buf, copy_en, copy_export, exit_status);
	free(trimmed);
}
