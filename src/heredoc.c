#include <minishell.h>

char	*procesar_variable(char *pos, char **envp, char **res_ptr)
{
	char	*start;
	char	var_name[1000];
	char	*var_ptr;
	char	*env_value;

	var_ptr = var_name;
	start = pos + 1;
	var_ptr = var_name;
	ft_memset(var_name, 0, sizeof(var_name));
	while (*start && (ft_isalnum(*start) || *start == '_'))
		*var_ptr++ = *start++;
	*var_ptr = '\0';
	env_value = get_env_value(var_name, envp);
	if (env_value)
	{
		while (*env_value)
			*(*res_ptr)++ = *env_value++;
	}
	return (start);
}

void	expand_heredoc(char *str, char **envp)
{
	char	result[4096];
	char	*res_ptr;
	char	*pos;

	res_ptr = result;
	pos = str;
	while (*pos)
	{
		if (*pos == '$')
			pos = procesar_variable(pos, envp, &res_ptr);
		else
			*res_ptr++ = *pos++;
	}
	*res_ptr = '\0';
	ft_strcpy(str, result);
}

int	construir_nuevo_archivo(char *heredocstart, char *input, char *delimiterend,
		char *nombrearchivo)
{
	size_t	lenbeforheredoc;
	size_t	lenafterdelimiter;
	size_t	nuevostringlen;
	char	nuevostring[4096];

	lenbeforheredoc = heredocstart - input;
	lenafterdelimiter = ft_strlen(delimiterend);
	nuevostringlen = lenbeforheredoc + ft_strlen("< ")
		+ ft_strlen(nombrearchivo) + lenafterdelimiter + 1;
	if (nuevostringlen > 4096)
	{
		ft_printf("El nuevo string excede el tamaño del buffer.\n");
		return (1);
	}
	ft_strncpy(nuevostring, input, lenbeforheredoc);
	nuevostring[lenbeforheredoc] = '\0';
	ft_strcat(nuevostring, "< ");
	ft_strcat(nuevostring, nombrearchivo);
	ft_strcat(nuevostring, delimiterend);
	ft_strncpy(input, nuevostring, 4096);
	input[4095] = '\0';
	heredocstart = input + lenbeforheredoc + ft_strlen("< ")
		+ ft_strlen(nombrearchivo);
	return (0);
}

void	construir_archivo_heredoc(char *nombrearchivo, int heredoccount)
{
	char	heredoccountstr[10];

	heredoccount++;
	g_signal = 2;
	ft_strcpy(nombrearchivo, "archivo_creado_");
	int_to_str(heredoccount, heredoccountstr);
	ft_strcat(nombrearchivo, heredoccountstr);
	ft_strcat(nombrearchivo, ".txt");
}

int	hijo_done(pid_t pid)
{
	int	status;

	signal(SIGINT, ft_handle_sigint);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		kill(pid, SIGINT);
		return (1);
	}
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		return (1);
	return (0);
}

void	lectura_heredoc(char *linea, char *delimiterstr, char **env,
		int archivo)
{
	while (1)
	{
		linea = readline("> ");
		if (linea == NULL)
		{
			ft_printf("warning: here-document at line delimited by end-of-file (wanted `%s')\n",
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

int	manejarProcesoHeredoc(char *heredocstart, char *input, char **env)
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

int	procesarHeredoc(char *input, char **env)
{
	char	*heredocstart;

	heredocstart = input;
	while ((heredocstart = ft_strstr(heredocstart, "<<")) != NULL)
	{
		if (manejarProcesoHeredoc(heredocstart, input, env))
			return (1);
		heredocstart += 2;
	}
	return (0);
}

void	eliminarArchivos(void)
{
	char	nombrearchivo[256];
	char	heredocCountStr[10];
	int		i;

	i = 1;
	while (1)
	{
		ft_strcpy(nombrearchivo, "archivo_creado_");
		int_to_str(i, heredocCountStr);
		ft_strcat(nombrearchivo, heredocCountStr);
		ft_strcat(nombrearchivo, ".txt");
		if (unlink(nombrearchivo) == 0)
			i++;
		else
			break ;
	}
}

void	int_to_str(int num, char *str)
{
	int i;
	int is_negative;

	i = 0;
	is_negative = 0;
	if (num == 0)
	{
		str[i++] = '0';
		str[i] = '\0';
		return ;
	}
	if (num < 0)
	{
		is_negative = 1;
		num = -num;
	}
	while (num != 0)
	{
		int rem = num % 10;
		str[i++] = rem + '0';
		num = num / 10;
	}
	if (is_negative)
		str[i++] = '-';
	str[i] = '\0';
	int start = 0;
	int end = i - 1;
	while (start < end)
	{
		char temp = str[start];
		str[start] = str[end];
		str[end] = temp;
		start++;
		end--;
	}
}