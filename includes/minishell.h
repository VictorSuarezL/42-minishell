
#ifndef MINISHELL_H
# define MINISHELL_H

// BORRAR
# include <fcntl.h>
# include <libft.h>

// NO BORRAR
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <unistd.h>

int		ft_strcmp(const char *s1, const char *s2);
char	*get_delimiter(char **delimiter);
int		have_heredoc(char *str, char **delimiter);
void	create_file(char *info);
void	launch_heredoc(char *delimiter);
char	**ft_split_m(char const *s);
int		validator(char *str);

#endif
