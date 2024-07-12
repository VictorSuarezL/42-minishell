#include <minishell.h>

void	update_redir(char *inicio_redirecciones,
		char *ultima_redireccion, char modo_redireccion)
{
	char	redir_simbolo[3];

	if (modo_redireccion == 'a')
		ft_strcpy(redir_simbolo, ">>");
	else
		ft_strcpy(redir_simbolo, "> ");
	ft_strcpy(inicio_redirecciones, redir_simbolo);
	ft_strcpy(inicio_redirecciones + ft_strlen(redir_simbolo),
		ultima_redireccion);
}

void	clean_redirs_left(char *inicio_redirecciones,
		char *ultima_redireccion, char modo_redireccion)
{
	char	*pos_eliminar;
	char	redir_simbolo[3];

	if (modo_redireccion == 'a')
		ft_strcpy(redir_simbolo, ">>");
	else
		ft_strcpy(redir_simbolo, "> ");
	pos_eliminar = inicio_redirecciones + ft_strlen(ultima_redireccion)
		+ ft_strlen(redir_simbolo);
	while (*pos_eliminar)
	{
		if (*pos_eliminar == '>')
		{
			while (*pos_eliminar == ' ' || *pos_eliminar == '>')
				pos_eliminar++;
			if (*pos_eliminar)
				ft_strcpy(inicio_redirecciones + ft_strlen(redir_simbolo),
					pos_eliminar);
			else
				*(inicio_redirecciones + ft_strlen(redir_simbolo)) = '\0';
			break ;
		}
		pos_eliminar++;
	}
}

void	modify_entry(char *entrada, char *ultima_redireccion,
		char modo_redireccion)
{
	char	*inicio_redirecciones;

	inicio_redirecciones = ft_strchr(entrada, '>');
	if (inicio_redirecciones)
	{
		update_redir(inicio_redirecciones, ultima_redireccion,
			modo_redireccion);
		clean_redirs_left(inicio_redirecciones,
			ultima_redireccion, modo_redireccion);
	}
}

int	create_open_file(const char *archivo, char modo_redireccion)
{
	int	flags;
	int	archivo_fd;

	if (modo_redireccion == 'w')
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else
		flags = O_WRONLY | O_CREAT | O_APPEND;
	archivo_fd = open(archivo, flags, 0644);
	if (archivo_fd == -1)
	{
		perror("open");
		return (-1);
	}
	close(archivo_fd);
	return (0);
}

void	adjust_mode_pos(char **pos, char *modo_redireccion)
{
	if (*(*pos + 1) == '>')
	{
		*modo_redireccion = 'a';
		(*pos)++;
	}
	else
		*modo_redireccion = 'w';
	(*pos)++;
	while (**pos == ' ')
		(*pos)++;
}
