#include <minishell.h>

int	execute_builtin(char *input, char ***export, char ***env)
{
	char	**split_input;
	int		result;

	if (ft_strncmp(input, "echo", 4) == 0)
		return (echo_builtin(input), 0);
	if (ft_strncmp(input, "exit", 4) == 0)
		return (exit_builtin());
	if (ft_strncmp(input, "env", 3) == 0)
		return (env_builtin(input, *env));
	if (ft_strncmp(input, "export", 6) == 0)
	{
		split_input = ft_split(input, ' ');
		result = export_builtin(split_input, export, env);
		return (free_double(split_input), result);
	}
	if (ft_strncmp(input, "pwd", 3) == 0)
		return (pwd_builtin(input));
	if (ft_strncmp(input, "unset", 5) == 0)
	{
		split_input = ft_split(input, ' ');
		result = unset_builtin(export, split_input);
		unset_builtin(env, split_input);
		return (free_double(split_input), result);
	}
	return (47);
}

int	is_builtin_env(char *input)
{
	if (ft_strncmp(input, "export", 6) == 0 || ft_strncmp(input, "unset",
			5) == 0 || ft_strncmp(input, "env", 3) == 0 || ft_strcmp(input,
			"exit") == 0 || ft_strncmp(input, "exit ", 5) == 0)
		return (1);
	return (0);
}

int	is_builtin(char *input)
{
	if (ft_strncmp(input, "echo", 4) == 0 || ft_strncmp(input, "exit", 4) == 0
		|| ft_strncmp(input, "env", 3) == 0 || ft_strncmp(input, "export",
			6) == 0 || ft_strncmp(input, "pwd", 3) == 0 || ft_strncmp(input,
			"unset", 5) == 0)
		return (1);
	return (0);
}

int	execute_cd(char *buf, char **env, char **export)
{
	const char	*path;
	int			result;

	if (((ft_strncmp(buf, "cd", 2) == 0 && (buf[2] == ' ' || buf[2] == '\0')))
		&& (!ft_strstr(buf, "|") && (!ft_strstr(buf, "<") && (!ft_strstr(buf,
						">")))))
	{
		if (buf[2] == ' ')
			path = buf + 3;
		else
			path = NULL;
		result = cd_builtin(path, env, export);
		return (result);
	}
	return (47);
}