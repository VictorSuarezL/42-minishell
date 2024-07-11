#ifndef MINISHELL_H
# define MINISHELL_H

// BORRAR
# include <fcntl.h>
# include <libft.h>

// NO BORRAR
# include <ctype.h>
# include <dirent.h>
# include <errno.h>
# include <linux/limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

extern int					g_signal;

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

int							save_fork(void);
void						runcmd(t_cmd *cmd, char **env_copy,
								char **export_copy);
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
void						quote_manager(char *str, int i, int j);

// BUILTINS
int							cd_builtin(const char *path, char **env,
								char **export);
int							get_current_working_directory(char *cwd,
								size_t size);
char						*expand_path(const char *path, char **env);
char						*get_new_path(const char *path, char **env);
int							change_directory(const char *path, char *cwd,
								size_t size, char **env_vars[3]);
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
int							calculate_total_length(char **input);
char						*join_strings_with_spaces(char **input);
void						builtin_exec(t_execcmd *ecmd, char **env_copy,
								char **export_copy);
char						*display_prompt(void);
int							find_and_update(char **export, char *variable,
								char *key, char *value);
char						**allocate_new_export(int count);
int							add_new_variable(char ***export, char *variable,
								char *key, char *value);
int							is_escaped(char *token, char *pos);
int							is_valid_variable_name(const char *name);
void						process_variable_expand(const char **pos,
								char **envp, char *result, size_t new_size);
void						handle_single_quote(const char **pos,
								int *in_single_quotes, char *result);
void						handle_escaped_dollar(const char **pos,
								char *result);
void						handle_variable_expand(const char **pos,
								char **envp, char *result, size_t new_size);
void						handle_regular_char(const char **pos, char *result);
int							skip_command(char *input, int i);
int							is_echo_command(char *input, int *i);
int							handle_n_option(char *input, int i, int *line);
int							handle_escape_sequences(char *input, int i);
void						handle_output(char *input, int i);
void						echo_builtin(char *input);
void						exit_stat(char *buf, int *exit_code,
								char ***copy_en, char ***copy_exp);

int							exit_builtin(char *buf, int *should_exit);

int							env_builtin(char *input, char **env);
void						split_variable(char *variable, char **key,
								char **value);
int							find_and_update(char **export, char *variable,
								char *key, char *value);
char						**allocate_new_export(int count);
int							add_new_variable(char ***export, char *variable,
								char *key, char *value);
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
void						setup_executor(char *buf, char **env, char **export,
								int *exit_status);
void						setup_shell(char ***copy_exp, char ***copy_en,
								char **env);
void						process_each_token(char *token, char *result,
								int *wcard_pres, int *any_pat_found);
void						ft_handle_sigint(int signum);
void						ft_handle_sigquit(int signum);
void						suppress_output(void);

int							execute_builtin(char *input, char ***export,
								char ***env);
int							is_builtin_env(char *input);
int							is_builtin(char *input);
int							execute_cd(char *buf, char **env, char **export);

void						eliminarArchivos(void);

void						add_buf(char *result, char *token);
int							wildcard_result(int wildcard_present,
								int any_pattern_found);
int							expand_wildcards(char *buf);
int							match_pattern(const char *pattern, const char *str);
void						initialize_variables(char *result,
								int *pattern_found, int *any_pattern_found,
								int *wildcard_present);
void						read_directory(DIR *d, char *token, char *result,
								int *pattern_found);
int							process_token(char *token);
int							expand_token(char *token, char *result,
								int *pattern_found);
void						finalize_result(char *buf, char *result);

void						actualizar_redireccion(char *inicio_redirecciones,
								char *ultima_redireccion,
								char modo_redireccion);
void						limpiar_redirecciones_restantes(char *inicio_redir,
								char *ultima_redireccion,
								char modo_redireccion);
void						modificar_entrada(char *entrada,
								char *ultima_redireccion,
								char modo_redireccion);
int							crear_abrir_archivo(const char *archivo,
								char modo_redireccion);
void						ajustar_modo_y_pos(char **pos,
								char *modo_redireccion);
char						*obtener_archivo_y_actualizar(char *pos,
								char *entrada_copy, char **ultima_redireccion,
								char modo_redireccion);
char						*procesar_redireccion(char *pos, char *entrada_copy,
								char **ultima_redireccion,
								char *modo_redireccion);
void						procesar_todas_redirecciones(char *entrada_copy,
								char **ultima_redireccion,
								char *modo_redireccion);
void						procesarredirecciones(char *entrada);

char						*procesar_variable(char *pos, char **envp,
								char **res_ptr);
void						expand_heredoc(char *str, char **envp);
int							construir_nuevo_archivo(char *heredocstart,
								char *input, char *delimiterend,
								char *nombrearchivo);
void						construir_archivo_heredoc(char *nombrearchivo,
								int heredoccount);
int							hijo_done(pid_t pid);
void						lectura_heredoc(char *linea, char *delimiterstr,
								char **env, int archivo);
void						process_input(char *input, char ***copy_en,
								char ***copy_export, int *exit_status);
void						proceso_hijo(char *linea, char *delimiterstr,
								char **env, int archivo, char *nombrearchivo);
void						extraer_delimiter(char *delimiter,
								char *delimiterstr, size_t *lendelimiter);
void						avanza_delimiter(char **delimiter,
								char *heredocstart);
int							manejarprocesoheredoc(char *heredocstart,
								char *input, char **env);
int							procesarheredoc(char *input, char **env);
void						eliminararchivos(void);
void						int_to_str(int num, char *str);

void						process_variable_size(const char **str, char **envp,
								size_t *size);
void						process_single_character(const char **str,
								size_t *size);
void						process_single_quote(const char **str, size_t *size,
								int *in_single_quotes);
void						process_escape_sequence(const char **str,
								size_t *size);
size_t						calculate_size(const char *str, char **envp);
void						process_variable_expand(const char **pos,
								char **envp, char *result, size_t new_size);
void						handle_single_quote(const char **pos,
								int *in_single_quotes, char *result);
void						handle_escaped_dollar(const char **pos,
								char *result);
void						handle_variable_expand(const char **pos,
								char **envp, char *result, size_t new_size);
void						handle_regular_char(const char **pos, char *result);
void						expand(char *str, char **envp);

#endif
