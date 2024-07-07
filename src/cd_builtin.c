#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <minishell.h>

char *get_env_value(const char *name, char **envp)
{
    size_t name_len;
    int i;

    i = 0;
    name_len = ft_strlen(name);
    while(envp[i])
    {
        if (ft_strncmp(envp[i], name, name_len) == 0 && envp[i][name_len] == '=')
            return (ft_strdup(envp[i] + name_len + 1));  // Devuelve el valor después del '='
        i++;
    }
    return (NULL);
}

// Function to find the index of an environment variable
int find_env_var(char **env, const char *var)
{
    size_t var_len;
    int i;

    var_len = ft_strlen(var);
    i = 0;
    while (env[i] != NULL)
    {
        if (ft_strncmp(env[i], var, var_len) == 0 && env[i][var_len] == '=')
            break;
        i++;
    }
    return (i);
}


// Function to allocate memory for a new environment variable
void allocate_new_env_var(char **env, int i, const char *var, const char *value)
{
    size_t var_len;
    size_t value_len;

    var_len = ft_strlen(var);
    value_len = ft_strlen(value);
    env[i] = malloc(var_len + value_len + 2);
    if (env[i] == NULL)
    {
        perror("malloc");
        return;
    }
    ft_strcpy(env[i], var);
    env[i][var_len] = '=';
    ft_strcpy(env[i] + var_len + 1, value);
    env[i + 1] = NULL;
}

// Function to free existing memory and allocate new memory for an environment variable
void free_and_allocate_env_var(char **env, int i, const char *var, const char *value)
{
    size_t var_len;
    size_t value_len;

    var_len = ft_strlen(var);
    value_len = ft_strlen(value);
    free(env[i]);
    env[i] = malloc(var_len + value_len + 2);
    if (env[i] == NULL)
    {
        perror("malloc");
        return;
    }
    ft_strcpy(env[i], var);
    env[i][var_len] = '=';
    ft_strcpy(env[i] + var_len + 1, value);
}

// Function to update an existing environment variable
void update_existing_env_var(char **env, int i, const char *var, const char *value)
{
    free_and_allocate_env_var(env, i, var, value);
}

// Function to update an environment variable
void update_env_var(char **env, const char *var, const char *value)
{
    int i;

    i = find_env_var(env, var);
    if (env[i] != NULL)
        update_existing_env_var(env, i, var, value);
    else
        allocate_new_env_var(env, i, var, value);
}

char *create_expanded_path(const char *home, const char *path)
{
    size_t home_len;
    size_t path_len;
    char *expanded_path;

    home_len = ft_strlen(home);
    path_len = ft_strlen(path);
    expanded_path = malloc(home_len + path_len);
    if (!expanded_path)
    {
        perror("malloc");
        return (NULL);
    }
    ft_strcpy(expanded_path, home);
    if (path[1])
        ft_strcat(expanded_path, path + 1);
    return (expanded_path);
}

char *handle_virguline(const char *path, char **env)
{
    char *home; 
    char *expanded_path;

    home = get_env_value("HOME", env);
    if (!home)
    {
        ft_printf("Error getting home directory\n");
        return NULL;
    }
    expanded_path = create_expanded_path(home, path);
    free(home);
    return (expanded_path);
}

char *expand_path(const char *path, char **env)
{
    if (path[0] == '~')
        return (handle_virguline(path, env));
    return (ft_strdup(path));
}

// Function to check if a string is empty or contains only spaces
int is_empty_or_spaces(const char *str)
{
    // Check if the string is NULL or empty
    if (str == NULL || *str == '\0')
        return (1);
    // Iterate over each character in the string
    while (*str != '\0')
    {
        // Check if the character is not a space
        if (*str != ' ')
            return (0);
        str++;
    }
    // If all characters are spaces, return 1
    return (1);
}

int get_current_working_directory(char *cwd, size_t size)
{
    if (getcwd(cwd, size) == NULL)
    {
        perror("getcwd");
        return (1);
    }
    return (0);
}

char *get_new_path(const char *path, char **env)
{
    if (ft_strcmp(path, "~") == 0)
        return (get_env_value("HOME", env));
    else if (ft_strcmp(path, "-") == 0)
        return (get_env_value("OLDPWD", env));
    else
        return (expand_path(path, env));
}

void update_environment_vars(char **env_vars[3], const char *cwd)
{
    update_env_var(env_vars[0], "OLDPWD", cwd);
    update_env_var(env_vars[1], "OLDPWD", cwd);
}

void update_prev_dir(char **prev_dir, const char *cwd)
{
    if (*prev_dir)
        free(*prev_dir);
    *prev_dir = ft_strdup(cwd);
}

int change_directory(const char *path, char *cwd, size_t size, char **env_vars[3])
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

int cd_builtin(const char *path, char **env, char **export_vars)
{
    char cwd[4096];
    int result;
    char *prev_dir = NULL;
    char *new_path = NULL;
    char **env_vars[3] = {env, export_vars, &prev_dir};

    if (get_current_working_directory(cwd, sizeof(cwd)))
        return 1;
    if (!path || is_empty_or_spaces(path))
        path = "~";
    new_path = get_new_path(path, env);
    if (!new_path)
    {
        printf("Error getting directory\n");
        return 1;
    }
    result = change_directory(new_path, cwd, sizeof(cwd), env_vars);
    if (new_path != path)
        free(new_path);
    if (prev_dir)
        free(prev_dir);
    return result;
}

