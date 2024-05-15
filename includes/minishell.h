/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francfer <francfer@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 20:01:49 by vsanz-su          #+#    #+#             */
/*   Updated: 2024/05/15 16:18:12 by francfer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// BORRAR
# include <fcntl.h>
# include <libft.h>

// NO BORRAR
# include <signal.h>
# include <stdio.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>

int	    ft_strcmp(const char *s1, const char *s2);
char	*get_delimiter(char **delimiter);
int	    have_heredoc(char *str, char **delimiter);
void	create_file(char *info);
void	launch_heredoc(char *delimiter);
void    echo_builtin(char *input);
int		cd_builtin(const char *path, char **env);
int		pwd_builtin(void);
int 	exit_builtin(void);
int		env_builtin(char **env);
char	*display_prompt(void);
int		unset_builtin(char **env, char *variable);
void	free_double(char **str);
size_t	count_env_vars(char **env);
char	**copy_env(char **env);

#endif
