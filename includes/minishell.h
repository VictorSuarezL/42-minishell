/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francfer <francfer@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 20:01:49 by vsanz-su          #+#    #+#             */
/*   Updated: 2024/05/13 08:10:04 by francfer         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef MINISHELL_H
# define MINISHELL_H

// BORRAR
# include <fcntl.h>
# include <libft.h>

// NO BORRAR
# include <signal.h>
# include <stdio.h>
# include <unistd.h>
# include <readline/history.h>
# include <readline/readline.h>

int	    ft_strcmp(const char *s1, const char *s2);
char	*get_delimiter(char **delimiter);
int	    have_heredoc(char *str, char **delimiter);
void	create_file(char *info);
void	launch_heredoc(char *delimiter);
void    echo_builtin(char *input);
void    cd_builtin(const char *path);
void    pwd_builtin(void);
void    exit_builtin(int status);
void    env_builtin(char **env);

#endif
