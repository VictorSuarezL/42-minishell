#include <minishell.h>

void	read_heredoc(char *linea, char *delimiterstr, char **env,
		int archivo)
{
	while (1)
	{
		linea = readline("> ");
		if (linea == NULL)
		{
			ft_printf("here-document at line delimited by eof (wanted `%s')\n",
				delimiterstr);
			break ;
		}
		linea[ft_strcspn(linea, "\n")] = 0;
		if (ft_strcmp(linea, delimiterstr) == 0)
		{
			free(linea);
			break ;
		}
		expand_heredoc(linea, env);
		ft_putstr_fd(linea, archivo);
		ft_putchar_fd('\n', archivo);
		free(linea);
	}
}

void	abrir_archivo_y_proceso_hijo(char *linea, char *delimiterstr,
		char **env, char *nombrearchivo)
{
	int	archivo;

	archivo = open(nombrearchivo, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (archivo == -1)
	{
		perror("Error al crear el archivo");
		exit(1);
	}
	signal(SIGINT, 0);
	read_heredoc(linea, delimiterstr, env, archivo);
	close(archivo);
	exit(0);
}

void	extract_delimiter(char *delimiter, char *delimiterstr,
		size_t *lendelimiter)
{
	char	*delimiterend;

	delimiterend = delimiter;
	while (*delimiterend != ' ' && *delimiterend != '\0' && *delimiterend != '|'
		&& *delimiterend != '>')
		delimiterend++;
	*lendelimiter = delimiterend - delimiter;
	ft_strncpy(delimiterstr, delimiter, *lendelimiter);
	delimiterstr[*lendelimiter] = '\0';
}

void	move_delimiter(char **delimiter, char *heredocstart)
{
	*delimiter = heredocstart + 2;
	while (**delimiter == ' ')
		(*delimiter)++;
}

int	handlehredocprocess(char *heredocstart, char *input, char **env)
{
	char	*delimiter;
	char	nombrearchivo[256];
	char	delimiterstr[256];
	size_t	lendelimiter;
	pid_t	pid;

	move_delimiter(&delimiter, heredocstart);
	extract_delimiter(delimiter, delimiterstr, &lendelimiter);
	built_heredoc_file(nombrearchivo, 0);
	pid = fork();
	if (pid == -1)
		return (perror("Error al crear el proceso hijo"), 1);
	else if (pid == 0)
	{
		abrir_archivo_y_proceso_hijo(NULL, delimiterstr, env, nombrearchivo);
	}
	else
	{
		if (child_done(pid) || built_new_file(heredocstart, input,
				delimiter + lendelimiter, nombrearchivo))
			return (1);
	}
	return (0);
}
