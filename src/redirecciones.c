#include <minishell.h>

void	actualizar_redireccion(char *inicio_redirecciones,
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

void	limpiar_redirecciones_restantes(char *inicio_redirecciones,
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

void	modificar_entrada(char *entrada, char *ultima_redireccion,
		char modo_redireccion)
{
	char	*inicio_redirecciones;

	inicio_redirecciones = ft_strchr(entrada, '>');
	if (inicio_redirecciones)
	{
		actualizar_redireccion(inicio_redirecciones, ultima_redireccion,
			modo_redireccion);
		limpiar_redirecciones_restantes(inicio_redirecciones,
			ultima_redireccion, modo_redireccion);
	}
}

int	crear_abrir_archivo(const char *archivo, char modo_redireccion)
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

void	ajustar_modo_y_pos(char **pos, char *modo_redireccion)
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

char	*obtener_archivo_y_actualizar(char *pos, char *entrada_copy,
		char **ultima_redireccion, char modo_redireccion)
{
	char	*token_start;
	size_t	token_length;
	char	*archivo;

	token_start = pos;
	while (*pos && *pos != ' ' && *pos != '>')
		pos++;
	token_length = pos - token_start;
	archivo = ft_strndup(token_start, token_length);
	if (!archivo)
	{
		ft_printf("malloc failed: Cannot allocate memory\n");
		free(entrada_copy);
		exit(EXIT_FAILURE);
	}
	if (crear_abrir_archivo(archivo, modo_redireccion) == -1)
	{
		free(archivo);
		free(entrada_copy);
		exit(EXIT_FAILURE);
	}
	free(*ultima_redireccion);
	*ultima_redireccion = archivo;
	return (pos);
}

char	*procesar_redireccion(char *pos, char *entrada_copy,
		char **ultima_redireccion, char *modo_redireccion)
{
	ajustar_modo_y_pos(&pos, modo_redireccion);
	return (obtener_archivo_y_actualizar(pos, entrada_copy, ultima_redireccion,
			*modo_redireccion));
}

void	procesar_todas_redirecciones(char *entrada_copy,
		char **ultima_redireccion, char *modo_redireccion)
{
	char	*pos;

	pos = entrada_copy;
	pos = ft_strchr(pos, '>');
	while (pos != NULL && (*(pos - 1) != '\\'))
	{
		pos = procesar_redireccion(pos, entrada_copy, ultima_redireccion,
				modo_redireccion);
		pos = ft_strchr(pos, '>');
	}
}

void	procesarredirecciones(char *entrada)
{
	char	*entrada_copy;
	char	*ultima_redireccion;
	char	modo_redireccion;

	entrada_copy = ft_strdup(entrada);
	if (!entrada_copy)
	{
		ft_printf("malloc failed: Cannot allocate memory\n");
		exit(EXIT_FAILURE);
	}
	ultima_redireccion = NULL;
	modo_redireccion = 'w';
	procesar_todas_redirecciones(entrada_copy, &ultima_redireccion,
		&modo_redireccion);
	if (ultima_redireccion)
		modificar_entrada(entrada, ultima_redireccion, modo_redireccion);
	free(entrada_copy);
	free(ultima_redireccion);
}
