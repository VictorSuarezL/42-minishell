
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

typedef struct s_cmd	t_cmd;
typedef struct s_pipecmd	t_pipecmd;
typedef struct s_execcmd	t_execcmd;
typedef struct s_redircmd	t_redircmd;

typedef enum e_type
{
	EXEC,
	REDIR,
	PIPE,
	LIST
}	t_type;

typedef struct s_cmd
{
	int	type;
}	t_cmd;

typedef struct s_pipecmd
{
	int		type;
	t_cmd	*left;
	t_cmd	*right;
}	t_pipecmd;

typedef struct s_execcmd
{
	int		type;
	char	*argv[100];
	char	*eargv[100];
}	t_execcmd;

typedef struct s_redircmd
{
	int		type;
	t_cmd	*cmd;
	char	*file;
	char	*efile;
	int		mode;
	int		fd;
}	t_redircmd;

// int		ft_strcmp(const char *s1, const char *s2);
// char	*get_delimiter(char **delimiter);
// int		have_heredoc(char *str, char **delimiter);
// void	create_file(char *info);
// void	launch_heredoc(char *delimiter);
// char	**ft_split_m(char const *s);
// int		validator(char *str);
int	save_fork(void);
void	runcmd(t_cmd *cmd, char **env_copy);
int wait_status(void);
void	wait_pipe(void);
void	remove_quotes(t_execcmd *ecmd);

int	validator(char *str);
// SEARCH_PATH.C
void ft_perror(char *msg);
void free_all(char **str);
char	*find_path(char *command, char **export_env);
t_cmd	*pipe_cmd(t_cmd *left, t_cmd *right);
// t_cmd	*redir_cmd(t_cmd *subcmd, char *file, char *efile, int mode, int fd);
t_cmd	*exec_cmd(void);

// TOKENIZE.C
int	peek(char **ps, char *es, char *toks);
int	get_token(char **p_str, char *end_str, char **q, char **end_q);

// PARSE.C
t_cmd	*parse_cmd(char *str);

// CONSTRUCTOR.C
t_cmd	*exec_cmd(void);
t_cmd	*pipe_cmd(t_cmd *left, t_cmd *right);
// t_cmd	*redir_cmd(t_cmd *subcmd, char *file, char *efile, int mode, int fd);
t_cmd	*redir_out_cmd(t_cmd *subcmd, char *file, char *efile, int mode);
t_cmd	*redir_in_cmd(t_cmd *subcmd, char *file, char *efile, int mode);

// ESCAPE.C
void escape_d_chars(char *str, char *aux, int *i, int *j); 
void escape_s_chars(char *str, char *aux, int *i, int *j);
void escape_special_chars(char *str); 
void pop_slash(char *str);
void replace_qmark(char *line, int exit_status);

// UTILS_TOK
void ft_perror(char *msg);
char	*skip_whitespace(char *str, char *end);




#endif
