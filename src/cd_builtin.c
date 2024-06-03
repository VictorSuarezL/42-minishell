#include <minishell.h>

char *get_path(char **env)
{
    for (int i = 0; env[i] != NULL; i++)
    {
        if (ft_strncmp(env[i], "HOME=", 5) == 0)
        {
            return ft_strdup(env[i] + 5);
        }
    }
    return NULL;
}

void update_env_var(char **env, const char *var, const char *value)
{
    size_t var_len = ft_strlen(var);
    size_t value_len = ft_strlen(value);
    int i;

    for (i = 0; env[i] != NULL; i++) {
        if (ft_strncmp(env[i], var, var_len) == 0 && env[i][var_len] == '=') {
            size_t old_value_len = ft_strlen(env[i] + var_len + 1);
            if (old_value_len < value_len) {
                free(env[i]);
                env[i] = malloc(var_len + value_len + 2);
                if (env[i] == NULL) {
                    perror("malloc");
                    return;
                }
            }
            ft_strcpy(env[i], var);
            env[i][var_len] = '=';
            ft_strcpy(env[i] + var_len + 1, value);
            return;
        }
    }

    env[i] = malloc(var_len + value_len + 2);
    if (env[i] == NULL) {
        perror("malloc");
        return;
    }
    ft_strcpy(env[i], var);
    env[i][var_len] = '=';
    ft_strcpy(env[i] + var_len + 1, value);
    env[i + 1] = NULL;
}

char *get_oldpwd(char **env)
{
    for (int i = 0; env[i] != NULL; i++)
    {
        if (ft_strncmp(env[i], "OLDPWD=", 7) == 0)
        {
            return env[i] + 7;
        }
    }
    return NULL;
}

int cd_builtin(const char *path, char **env)
{
    static char *prev_dir = NULL;
    char *new_path = NULL;
    char cwd[4096];

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd");
        return 1;
    }

    if (!path || ft_strcmp(path, "~") == 0)
    {
        new_path = get_path(env);
        if (!new_path)
        {
            return (ft_putstr_fd("Error getting home directory\n", 2), 1);
        }
        path = new_path;
    }
    else if (ft_strcmp(path, "-") == 0)
    {
        char *oldpwd = get_oldpwd(env);
        if (!oldpwd)
        {
            return (ft_putstr_fd("OLDPWD not set\n", 2), 1);
        }
        path = oldpwd;
        printf("%s\n", path);
    }

    if (chdir(path) == -1)
    {
        perror("cd");
        free(new_path);
        return 1;
    }

    update_env_var(env, "OLDPWD", cwd);

    if (prev_dir)
    {
        free(prev_dir);
    }

    prev_dir = ft_strdup(cwd);

    free(new_path);
    return 0;
}