#include <minishell.h>

char	*get_file_update(char *pos, char *entry_copy,
		char **last_redir, char redir_mode)
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
		free(entry_copy);
		exit(EXIT_FAILURE);
	}
	if (create_open_file(archivo, redir_mode) == -1)
	{
		free(archivo);
		free(entry_copy);
		exit(EXIT_FAILURE);
	}
	
	// if (create_open_file(archivo, redir_mode) == -1)
	// {
	// 	free(archivo);
	// 	free(entry_copy);
	// 	exit(EXIT_FAILURE);
	// }
	free(*last_redir);
	*last_redir = archivo;
	return (pos);
}

char	*process_redir(char *pos, char *entry_copy,
		char **last_redir, char *redir_mode)
{
	adjust_mode_pos(&pos, redir_mode);
	return (get_file_update(pos, entry_copy, last_redir,
			*redir_mode));
}

void	process_all_redirs(char *entry_copy,
		char **last_redir, char *redir_mode)
{
	char	*pos;

	pos = entry_copy;
	pos = ft_strchr(pos, '>');
	while (pos != NULL && (*(pos - 1) != '\\'))
	{
		pos = process_redir(pos, entry_copy, last_redir,
				redir_mode);
		pos = ft_strchr(pos, '>');
	}
}

void	processredirs(char *entrada)
{
	char	*entry_copy;
	char	*last_redir;
	char	redir_mode;

	entry_copy = ft_strdup(entrada);
	if (!entry_copy)
	{
		ft_printf("malloc failed: Cannot allocate memory\n");
		exit(EXIT_FAILURE);
	}
	last_redir = NULL;
	redir_mode = 'w';
	process_all_redirs(entry_copy, &last_redir,
		&redir_mode);
	if (last_redir)
		modify_entry(entrada, last_redir, redir_mode);
	free(entry_copy);
	free(last_redir);
}
