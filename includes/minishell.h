
#ifndef MINISHELL_H
# define MINISHELL_H

// BORRAR
# include <fcntl.h>
# include <libft.h>
# include <string.h>

// NO BORRAR
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <unistd.h>
#include <errno.h>
#include <sys/wait.h>


#define EXEC 1
#define REDIR 2
#define PIPE 3
#define LIST 4

struct			cmd
{
	int			type;
};

struct			pipecmd
{
	int			type;
	struct cmd	*left;
	struct cmd	*right;
};

struct			execcmd
{
	int			type;
	char		*argv[100];
	char		*eargv[100];
};

struct			redircmd
{
	int			type;
	struct cmd	*cmd;
	char		*file;
	char		*efile;
	int			mode;
	int			fd;
};

// int		ft_strcmp(const char *s1, const char *s2);
// char	*get_delimiter(char **delimiter);
// int		have_heredoc(char *str, char **delimiter);
// void	create_file(char *info);
// void	launch_heredoc(char *delimiter);
// char	**ft_split_m(char const *s);
// int		validator(char *str);
int	save_fork(void);
void	runcmd(struct cmd *cmd, char **env_copy);
int wait_status(void);
void	wait_pipe(void);
void	remove_quotes(struct execcmd *ecmd);

int	validator(char *str);
// SEARCH_PATH.C
void ft_perror(char *msg);
void free_all(char **str);
char	*find_path(char *command, char **export_env);
struct cmd	*pipe_cmd(struct cmd *left, struct cmd *right);
// struct cmd	*redir_cmd(struct cmd *subcmd, char *file, char *efile, int mode, int fd);
struct cmd	*exec_cmd(void);

// TOKENIZE.C
int	peek(char **ps, char *es, char *toks);
int	get_token(char **p_str, char *end_str, char **q, char **end_q);

// PARSE.C
struct cmd	*parse_cmd(char *str);

// CONSTRUCTOR.C
struct cmd	*exec_cmd(void);
struct cmd	*pipe_cmd(struct cmd *left, struct cmd *right);
// struct cmd	*redir_cmd(struct cmd *subcmd, char *file, char *efile, int mode, int fd);
struct cmd	*redir_out_cmd(struct cmd *subcmd, char *file, char *efile, int mode);
struct cmd	*redir_in_cmd(struct cmd *subcmd, char *file, char *efile, int mode);

// ESCAPE.C
void escape_d_chars(char *str, char *aux, int *i, int *j); 
void escape_s_chars(char *str, char *aux, int *i, int *j);
void escape_special_chars(char *str); 
void pop_slash(char *str);
void replace_qmark(char *line, int exit_status);

// UTILS_TOK
void ft_perror(char *msg);
char	*skip_whitespace(char *str, char *end);



//BUILTINS
int cd_builtin(const char *path, char **env, char **export);
char *get_env_value(const char *name, char **envp);
int find_env_var(char **env, const char *var);
void allocate_new_env_var(char **env, int i, const char *var, const char *value);
void free_and_allocate_env_var(char **env, int i, const char *var, const char *value);
void update_existing_env_var(char **env, int i, const char *var, const char *value);
void update_env_var(char **env, const char *var, const char *value);
char *create_expanded_path(const char *home, const char *path);
char *handle_virguline(const char *path, char **env);
int is_empty_or_spaces(const char *str);

char	*display_prompt(void);

int	skip_command(char *input, int i);
int	is_echo_command(char *input, int *i);
int	handle_n_option(char *input, int i, int *line);
int	handle_escape_sequences(char *input, int i);
void	handle_output(char *input, int i);
void	echo_builtin(char *input);

int	exit_builtin(void);

int	env_builtin(char *input, char **env);
void	split_variable(char *variable, char **key, char **value);
int find_and_update(char **export, char *variable, char *key);
char	**allocate_new_export(char **export, int count);
int	add_new_variable(char ***export, char *variable, char *key, char *value, int count);
int is_valid_variable_name(const char *name);
int add_variable(char *variable, char ***export);
int	export_builtin(char **variables, char ***export, char ***env);

int	pwd_builtin(char *input);

void	free_double(char **str);
size_t	count_env_vars(char **env);
char	**copy_env(char **env);
int should_remove_env_var(char *env_var, char **vars);
size_t populate_new_env(char **new_env, char **old_env, char **vars);
int unset_builtin(char ***env, char **vars);

int	skip_spaces(char *input, int i);
void	ft_swap(char **a, char **b);
void	sort_strings(char **str);
int	empty_string(char *str);
void	print_str(char **str_array);
void	copy_export(char **new_export, char **export, int count);

int	is_builtin_env(char *input);
int	is_builtin(char *input);
int execute_builtin(char *input, char ***export, char ***env);
#endif
