#include "minishell.h"

void	init_env_vars(char **env, char **export_vars, char ***env_vars)
{
	*env_vars = (char **)env;
	*(env_vars + 1) = export_vars;
}

void	init_paths(char **prev_dir, char **new_path, char ***env_vars)
{
	*prev_dir = NULL;
	*new_path = NULL;
	*(env_vars + 2) = prev_dir;
}

char	*resolve_new_path(const char *path, char **env)
{
	if (!path || is_empty_or_spaces(path))
		path = "~";
	return (get_new_path(path, env));
}

void	free_paths(char *new_path, const char *path, char *prev_dir)
{
	if (new_path != path)
		free(new_path);
	if (prev_dir)
		free(prev_dir);
}

int	cd_builtin(const char *path, char **env, char **export_vars)
{
	char	cwd[4096];
	int		result;
	char	*prev_dir;
	char	*new_path;
	char	**env_vars[3];

	init_env_vars(env, export_vars, env_vars);
	init_paths(&prev_dir, &new_path, env_vars);
	if (get_current_working_directory(cwd, sizeof(cwd)))
		return (1);
	new_path = resolve_new_path(path, env);
	if (!new_path)
	{
		printf("Error getting directory\n");
		return (1);
	}
	result = change_directory(new_path, cwd, sizeof(cwd), env_vars);
	free_paths(new_path, path, prev_dir);
	return (result);
}
