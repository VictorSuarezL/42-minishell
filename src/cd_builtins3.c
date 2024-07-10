#include <minishell.h>

int	get_current_working_directory(char *cwd, size_t size)
{
	if (getcwd(cwd, size) == NULL)
	{
		perror("getcwd");
		return (1);
	}
	return (0);
}

char	*get_new_path(const char *path, char **env)
{
	if (ft_strcmp(path, "~") == 0)
		return (get_env_value("HOME", env));
	else if (ft_strcmp(path, "-") == 0)
		return (get_env_value("OLDPWD", env));
	else
		return (expand_path(path, env));
}

void	update_environment_vars(char **env_vars[3], const char *cwd)
{
	update_env_var(env_vars[0], "OLDPWD", cwd);
	update_env_var(env_vars[1], "OLDPWD", cwd);
}

void	update_prev_dir(char **prev_dir, const char *cwd)
{
	if (*prev_dir)
		free(*prev_dir);
	*prev_dir = ft_strdup(cwd);
}

int	change_directory(const char *path, char *cwd, size_t size,
		char **env_vars[3])
{
	if (chdir(path) == -1)
	{
		perror("cd");
		return (1);
	}
	update_environment_vars(env_vars, cwd);
	if (get_current_working_directory(cwd, size))
		return (1);
	update_env_var(env_vars[0], "PWD", cwd);
	update_env_var(env_vars[1], "PWD", cwd);
	update_prev_dir(env_vars[2], cwd);
	return (0);
}
