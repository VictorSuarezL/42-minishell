#include <minishell.h>

char	*process_variable(char *pos, char **envp, char **res_ptr)
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
			pos = process_variable(pos, envp, &res_ptr);
		else
			*res_ptr++ = *pos++;
	}
	*res_ptr = '\0';
	ft_strcpy(str, result);
}

int	built_new_file(char *heredocstart, char *input, char *delimiterend,
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

void	built_heredoc_file(char *nombrearchivo, int heredoccount)
{
	char	heredoccountstr[10];

	heredoccount++;
	g_signal = 2;
	ft_strcpy(nombrearchivo, "archivo_creado_");
	int_to_str(heredoccount, heredoccountstr);
	ft_strcat(nombrearchivo, heredoccountstr);
	ft_strcat(nombrearchivo, ".txt");
}

int	child_done(pid_t pid)
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
