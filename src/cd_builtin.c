#include <minishell.h>

char	*get_path(char **env)
{
	int			i;
	char		*path;
	const char	*home_prefix = "HOME=";
	size_t		home_len;

	home_len = ft_strlen(home_prefix);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], home_prefix, home_len) == 0)
		{
			path = malloc(ft_strlen(env[i]) - home_len + 1);
			if (!path)
				return (NULL);
			ft_strlcpy(path, env[i] + home_len, ft_strlen(env[i]) - home_len
				+ 1);
			return (path);
		}
		i++;
	}
	return (NULL);
}

int cd_builtin(const char *path, char **env)
{
    static char *prev_dir = NULL;
    char *new_path;
    char *full_path;
    char cwd[4096];

    if (!path || ft_strcmp(path, "~") == 0)
        path = "~";
    else if (ft_strcmp(path, "-") == 0)
    {
        if (!prev_dir)
            return ft_putstr_fd("OLDPWD not set\n", 2), 1;
        path = prev_dir;
        printf("%s\n", path);
    }
    if (ft_strcmp(path, "~") == 0)
    {
        new_path = get_path(env);
        if (!new_path)
            return ft_putstr_fd("Error getting home directory\n", 2), 1;
        path = new_path;
    } else {
        new_path = NULL;
    }
    if (chdir(path) == -1)
    {
        perror("cd");
        free(new_path);
        return (1);
    }
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("getcwd");
        free(new_path);
        return (1);
    }
    if (prev_dir)
        free(prev_dir);
    prev_dir = ft_strdup(cwd);
    free(new_path);
    return (0);
}