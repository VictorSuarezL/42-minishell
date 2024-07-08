#ifndef MINISHELL_H
# define MINISHELL_H

// BORRAR
# include <fcntl.h>
# include <libft.h>
# include <string.h>

// NO BORRAR
# include <errno.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <sys/wait.h>
# include <unistd.h>
# include <linux/limits.h>
# include <termios.h>

extern	int 				g_signal;

typedef struct s_cmd		t_cmd;
typedef struct s_pipecmd	t_pipecmd;
typedef struct s_execcmd	t_execcmd;
typedef struct s_redircmd	t_redircmd;

typedef enum e_type
{
	EXEC,
	REDIR,
	PIPE
}							t_type;

typedef struct s_cmd
{
	int						type;
}							t_cmd;

typedef struct s_pipecmd
{
	int						type;
	t_cmd					*left;
	t_cmd					*right;
}							t_pipecmd;

typedef struct s_execcmd
{
	int						type;
	char					*argv[100];
	char					*eargv[100];
}							t_execcmd;

typedef struct s_redircmd
{
	int						type;
	t_cmd					*cmd;
	char					*file;
	char					*efile;
	int						mode;
	int						fd;
}							t_redircmd;

// int		ft_strcmp(const char *s1, const char *s2);
// char	*get_delimiter(char **delimiter);
// int		have_heredoc(char *str, char **delimiter);
// void	create_file(char *info);
// void	launch_heredoc(char *delimiter);
// char	**ft_split_m(char const *s);
// int		validator(char *str);
int							save_fork(void);
void						runcmd(t_cmd *cmd, char **env_copy, char **export_copy);
int							wait_status(void);
void						wait_pipe(void);
void						remove_quotes(t_execcmd *ecmd);

int							validator(char *str);
// SEARCH_PATH.C
void						ft_perror(char *msg);
void						free_all(char **str);
char						*find_path(char *command, char **export_env);
t_cmd						*pipe_cmd(t_cmd *left, t_cmd *right);
t_cmd						*exec_cmd(void);

// TOKENIZE.C
int							peek(char **ps, char *es, char *toks);
int							get_token(char **p_str, char *end_str, char **q,
								char **end_q);

// PARSE.C
t_cmd						*parse_cmd(char *str);

// CONSTRUCTOR.C
t_cmd						*exec_cmd(void);
t_cmd						*pipe_cmd(t_cmd *left, t_cmd *right);
t_cmd						*redir_out_cmd(t_cmd *subcmd, char *file,
								char *efile, int mode);
t_cmd						*redir_in_cmd(t_cmd *subcmd, char *file,
								char *efile, int mode);

// ESCAPE.C
void						escape_d_chars(char *str, char *aux, int *i,
								int *j);
void						escape_s_chars(char *str, char *aux, int *i,
								int *j);
void						escape_special_chars(char *str);
void						pop_slash(char *str);
void						replace_qmark(char *line, int exit_status);

// UTILS_TOK
void						ft_perror(char *msg);
char						*skip_whitespace(char *str, char *end);
void quote_manager(char *str, int i, int j);

// BUILTINS
int							cd_builtin(const char *path, char **env,
								char **export);
char						*get_env_value(const char *name, char **envp);
int							find_env_var(char **env, const char *var);
void						allocate_new_env_var(char **env, int i,
								const char *var, const char *value);
void						free_and_allocate_env_var(char **env, int i,
								const char *var, const char *value);
void						update_existing_env_var(char **env, int i,
								const char *var, const char *value);
void						update_env_var(char **env, const char *var,
								const char *value);
char						*create_expanded_path(const char *home,
								const char *path);
char						*handle_virguline(const char *path, char **env);
int							is_empty_or_spaces(const char *str);

char						*display_prompt(void);

int							skip_command(char *input, int i);
int							is_echo_command(char *input, int *i);
int							handle_n_option(char *input, int i, int *line);
int							handle_escape_sequences(char *input, int i);
void						handle_output(char *input, int i);
void						echo_builtin(char *input);

int							exit_builtin(void);

int							env_builtin(char *input, char **env);
void						split_variable(char *variable, char **key,
								char **value);
int							find_and_update(char **export, char *variable, char *key, char *value);
char						**allocate_new_export(int count);
int							add_new_variable(char ***export, char *variable, char *key, char *value);
int							is_valid_variable_name(const char *name);
int							add_variable(char *variable, char ***export);
int							export_builtin(char **variables, char ***export,
								char ***env);

int							pwd_builtin(char *input);

void						free_double(char **str);
size_t						count_env_vars(char **env);
char						**copy_env(char **env);
int							should_remove_env_var(char *env_var, char **vars);
size_t						populate_new_env(char **new_env, char **old_env,
								char **vars);
int							unset_builtin(char ***env, char **vars);

int							skip_spaces(char *input, int i);
void						ft_swap(char **a, char **b);
void						sort_strings(char **str);
int							empty_string(char *str);
void						print_str(char **str_array);
void						copy_export(char **new_export, char **export,
								int count);

int							is_builtin_env(char *input);
int							is_builtin(char *input);
int							execute_builtin(char *input, char ***export,
								char ***env);

void						final_clean(char **exp, char **env);
void						setup_executor(char *buf, char **env, char **export);
void						setup_shell(char ***copy_exp, char ***copy_en, char **env);

void						ft_handle_sigint(int signum);
void						ft_handle_sigquit(int signum);
void						suppress_output(void);

void 						eliminarArchivos(void);

#endif
