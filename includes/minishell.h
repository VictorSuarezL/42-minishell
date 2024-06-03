/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francfer <francfer@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 20:01:49 by vsanz-su          #+#    #+#             */
/*   Updated: 2024/06/03 21:01:28 by francfer         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

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

char *ft_strcpy(char *dest, const char *src);
char *get_oldpwd(char **env);

int		ft_strcmp(const char *s1, const char *s2);
char	*get_delimiter(char **delimiter);
int		have_heredoc(char *str, char **delimiter);
void	create_file(char *info);
void	launch_heredoc(char *delimiter);
void	echo_builtin(char *input);
int		cd_builtin(const char *path, char **env);
char	*get_path(char **env);
void	update_env_var(char **env, const char *var, const char *value);
char	*get_path(char **env);
int		pwd_builtin(void);
int		exit_builtin(void);
int		env_builtin(char *input, char **env);
char	*display_prompt(void);
int		unset_builtin(char **env, char **vars);
void	free_double(char **str);
size_t	count_env_vars(char **env);
char	**copy_env(char **env);
void	print_str(char **str);
int		add_variable(char *variable, char ***export);
int		export_builtin(char **variables, char ***export);
void	ft_swap(char **a, char **b);
void	sort_strings(char **cadenas);
size_t	get_block_size(void *ptr);
void	*my_malloc(size_t size);
void	my_free(void *ptr);
void	*ft_realloc(void *ptr, size_t new_size);
void	copy_export(char **new_export, char **export, int count);
int		skip_spaces(char *input, int i);
void	ft_swap(char **a, char **b);
void	sort_strings(char **str);
int		empty_string(char *str);
void	print_str(char **str_array);

#endif
