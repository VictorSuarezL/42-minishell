#include <minishell.h>

char	*get_file_update(char *pos, char *entrada_copy,
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
	if(save_fork() == 0)
	{
		if (create_open_file(archivo, modo_redireccion) == -1)
		{
			free(archivo);
			free(entrada_copy);
			exit(EXIT_FAILURE);
		}
	}
	wait_status();
	// if (create_open_file(archivo, modo_redireccion) == -1)
	// {
	// 	free(archivo);
	// 	free(entrada_copy);
	// 	exit(EXIT_FAILURE);
	// }
	free(*ultima_redireccion);
	*ultima_redireccion = archivo;
	return (pos);
}

char	*process_redir(char *pos, char *entrada_copy,
		char **ultima_redireccion, char *modo_redireccion)
{
	adjust_mode_pos(&pos, modo_redireccion);
	return (get_file_update(pos, entrada_copy, ultima_redireccion,
			*modo_redireccion));
}

void	process_all_redirs(char *entrada_copy,
		char **ultima_redireccion, char *modo_redireccion)
{
	char	*pos;

	pos = entrada_copy;
	pos = ft_strchr(pos, '>');
	while (pos != NULL && (*(pos - 1) != '\\'))
	{
		pos = process_redir(pos, entrada_copy, ultima_redireccion,
				modo_redireccion);
		pos = ft_strchr(pos, '>');
	}
}

void	processredirs(char *entrada)
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
	process_all_redirs(entrada_copy, &ultima_redireccion,
		&modo_redireccion);
	if (ultima_redireccion)
		modify_entry(entrada, ultima_redireccion, modo_redireccion);
	free(entrada_copy);
	free(ultima_redireccion);
}
