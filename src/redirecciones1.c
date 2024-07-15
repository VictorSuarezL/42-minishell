#include <minishell.h>

void	update_redir(char *start_redirs, char *last_redir, char redir_mode)
{
	char	redir_simbolo[3];

	if (redir_mode == 'a')
		ft_strcpy(redir_simbolo, ">>");
	else
		ft_strcpy(redir_simbolo, "> ");
	ft_strcpy(start_redirs, redir_simbolo);
	ft_strcpy(start_redirs + ft_strlen(redir_simbolo), last_redir);
}

void	clean_redirs_left(char *start_redirs, char *last_redir, char redir_mode)
{
	char	*pos_eliminar;
	char	redir_simbolo[3];

	if (redir_mode == 'a')
		ft_strcpy(redir_simbolo, ">>");
	else
		ft_strcpy(redir_simbolo, "> ");
	pos_eliminar = start_redirs + ft_strlen(last_redir)
		+ ft_strlen(redir_simbolo);
	while (*pos_eliminar)
	{
		if (*pos_eliminar == '>')
		{
			while (*pos_eliminar == ' ' || *pos_eliminar == '>')
				pos_eliminar++;
			if (*pos_eliminar)
				ft_strcpy(start_redirs + ft_strlen(redir_simbolo),
					pos_eliminar);
			else
				*(start_redirs + ft_strlen(redir_simbolo)) = '\0';
			break ;
		}
		pos_eliminar++;
	}
}

void	modify_entry(char *entrada, char *last_redir, char redir_mode)
{
	char	*start_redirs;

	start_redirs = ft_strchr(entrada, '>');
	if (start_redirs)
	{
		update_redir(start_redirs, last_redir, redir_mode);
		clean_redirs_left(start_redirs, last_redir, redir_mode);
	}
}

int	create_open_file(const char *archivo, char redir_mode)
{
	int	flags;
	int	archivo_fd;

	if (redir_mode == 'w')
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else
		flags = O_WRONLY | O_CREAT | O_APPEND;
	if (access(archivo, F_OK) == 0)
	{
		if (access(archivo, W_OK) != 0)
			return (2);
	}
	archivo_fd = open(archivo, flags, 0644);
	if (archivo_fd == -1)
	{
		perror("open");
		return (-1);
	}
	close(archivo_fd);
	return (0);
}

void	adjust_mode_pos(char **pos, char *redir_mode)
{
	if (*(*pos + 1) == '>')
	{
		*redir_mode = 'a';
		(*pos)++;
	}
	else
		*redir_mode = 'w';
	(*pos)++;
	while (**pos == ' ')
		(*pos)++;
}
