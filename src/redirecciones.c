#include <minishell.h>

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
