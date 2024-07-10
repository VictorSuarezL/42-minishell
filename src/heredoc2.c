#include <minishell.h>

void	lectura_heredoc(char *linea, char *delimiterstr, char **env,
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

void	proceso_hijo(char *linea, char *delimiterstr, char **env, int archivo,
		char *nombrearchivo)
{
	archivo = open(nombrearchivo, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (archivo == -1)
	{
		perror("Error al crear el archivo");
		exit(1);
	}
	signal(SIGINT, 0);
	lectura_heredoc(linea, delimiterstr, env, archivo);
	close(archivo);
	exit(0);
}

void	extraer_delimiter(char *delimiter, char *delimiterstr,
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

void	avanza_delimiter(char **delimiter, char *heredocstart)
{
	*delimiter = heredocstart + 2;
	while (**delimiter == ' ')
		(*delimiter)++;
}

int	manejarprocesoheredoc(char *heredocstart, char *input, char **env)
{
	char	*delimiter;
	char	nombrearchivo[256];
	char	delimiterstr[256];
	size_t	lendelimiter;
	pid_t	pid;

	avanza_delimiter(&delimiter, heredocstart);
	extraer_delimiter(delimiter, delimiterstr, &lendelimiter);
	construir_archivo_heredoc(nombrearchivo, 0);
	pid = fork();
	if (pid == -1)
	{
		perror("Error al crear el proceso hijo");
		return (1);
	}
	else if (pid == 0)
	{
		proceso_hijo(NULL, delimiterstr, env, 0, nombrearchivo);
	}
	else
	{
		if (hijo_done(pid) || construir_nuevo_archivo(heredocstart, input,
				delimiter + lendelimiter, nombrearchivo))
			return (1);
	}
	return (0);
}
