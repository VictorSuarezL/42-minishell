#include <errno.h>
#include <fcntl.h>
#include <minishell.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


#define BACK 5
#define MAXARGS 10



/* int execute_builtin(char *input, char ***export, char ***env) {
    char **split_input;
    if (strncmp(input, "echo", 4) == 0)
        return (echo_builtin(input), 0);
    if (strncmp(input, "exit", 4) == 0)
        return (exit_builtin());
    if (strncmp(input, "env", 3) == 0)
        return (env_builtin(input, *env));
    if (strncmp(input, "export", 6) == 0) {
        split_input = ft_split(input, ' ');
        int result = export_builtin(split_input, export, env);
        free_double(split_input);
        return result;
    }
    if (strncmp(input, "pwd", 3) == 0)
        return (pwd_builtin(input));
    if (strncmp(input, "unset", 5) == 0) {
        split_input = ft_split(input, ' ');
        int result = unset_builtin(export, split_input);
        unset_builtin(env, split_input);
        free_double(split_input);
        return result;
    }
    return (47);
}
int is_builtin_env(char *input)
{
    if (ft_strncmp(input, "export", 6) == 0 || ft_strncmp(input, "unset", 5) == 0 || ft_strncmp(input, "env", 3) == 0 )
        return (1);
    return (0);
}
int is_builtin(char *input)
{
    if (ft_strncmp(input, "echo", 4) == 0 || ft_strncmp(input, "exit", 4) == 0 || ft_strncmp(input, "env", 3) == 0
        || ft_strncmp(input, "export", 6) == 0 || ft_strncmp(input, "pwd", 3) == 0 || ft_strncmp(input, "unset", 5) == 0)
        return (1);
    return (0);
} */




// size_t	count_env_vars(char **env)
// {
// 	size_t	count;

// 	count = 0;
// 	while (env[count])
// 		count++;
// 	return (count);
// }

// char	**copy_env(char **env)
// {
// 	size_t	count;
// 	char	**copy;

// 	count = count_env_vars(env);
// 	copy = malloc((count + 1) * sizeof(char *));
// 	if (!copy)
// 		return (NULL);
// 	count = 0;
// 	while (env[count])
// 	{
// 		copy[count] = ft_strdup(env[count]);
// 		// if (!copy[count])
// 		// 	return (free_double(copy), NULL);
// 		count++;
// 	}
// 	copy[count] = NULL;
// 	return (copy);
// }


// int wait_status(void)
// {
// 	int status;
// 	int out_status = 0;

// 	wait(&status);
//     if (WIFEXITED(status) && WEXITSTATUS(status) != 0) 
// 		out_status = WEXITSTATUS(status);
// 	else if (WIFEXITED(status) && WIFSIGNALED(status) != 0) 
// 		out_status = WTERMSIG(status);
// 	else
// 		out_status = 0;
// 	return(out_status);
// }



// int	main(int argc, char *argv[], char **env)
// {
// 	char buf[4097];
// 	int exit_status  = 0;
// 	// int my_perror = 0;
// 	char	**export_env;
// 	// char	line[100] = "echo 'hola $HOME << > >  | mundo' estamos aqui 'aquí vamos $$$$$ otra vez' adfa";
// 	// char	line[100] = "ls -al | grep d";
// 	// char	line[100] = "ls -al | echo $?";
// 	// char	line[100] = "echo $HOME";
// 	// char	line[100] = "echa";
// 	// char	line[100] = "/bin/echo hola";
// 	char	line[100] = "echo 'hola echo mundo' esto es una prueba";
// 	// char	line[100] = "wc -l < b.txt < c.txt < a.txt";
// 	// char	line[100] = "cat /dev/random | head";
// 	// char	line[100] = "cat | cat | ls";
// 	// char	line[100] = "ls -al | grep d | wc -l";

// 	if (!validator(line))
// 	{
// 		printf("syntax error!\n");

// 	}
	
// 	// replace_qmark(line, exit_status);
// 	// printf("line = %s\n", line);

// 	// escape_special_chars(line);
// 	// printf("line: %s\n", line);
// 	// pop_slash(line);
// 	// printf("line poped: %s\n", line);

// 	// export_env = copy_env(env);
// 	// runcmd(parse_cmd(line), export_env, &my_perror);
// 	// SOLUCION PARA EL ERROR DEL HIJO:
	
// 	// Sustituir en line la cadena de texto "$?" por el valor de exit_status

// 	// char *aux = ft_atoi(exit_status);




	
// 	if(save_fork() == 0) 
// 	{
//       runcmd(parse_cmd(line), env);
//       exit(0); // Exit the child process after executing the command
//     }
// 	exit_status = wait_status();
// 	printf("Exit status = %d\n", exit_status);
// }
#include <minishell.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ctype.h>

size_t calculate_size(const char *str, char **envp) {
    size_t size = 0;
    int in_single_quotes = 0;

    while (*str) {
        if (*str == '\'') {
            in_single_quotes = !in_single_quotes;
            size++;
            str++;
        } else if (*str == '\\' && !in_single_quotes && *(str + 1) == '$') {
            size += 2; // Count both characters
            str += 2;
        } else if (*str == '$' && !in_single_quotes) {
            str++;
            const char *start = str;
            while (*str && (isalnum(*str) || *str == '_'))
                str++;
            char varname[str - start + 1];
            strncpy(varname, start, str - start);
            varname[str - start] = '\0';
            char *value = NULL;
            for (char **env = envp; *env != 0; env++) {
                char *thisEnv = *env;
                if (strncmp(thisEnv, varname, strlen(varname)) == 0 && thisEnv[strlen(varname)] == '=') {
                    value = thisEnv + strlen(varname) + 1;
                    break;
                }
            }
            if (value)
                size += strlen(value);
        } else {
            size++;
            str++;
        }
    }
    return size;
}

// Function to expand environment variables
void expand(char *str, char **envp) {
    size_t new_size = calculate_size(str, envp);
    char *result = malloc(new_size + 1);
    if (result == NULL) {
        fprintf(stderr, "Error: unable to allocate memory\n");
        return;
    }

    result[0] = '\0';
    const char *pos = str;
    int in_single_quotes = 0;

    while (*pos) {
        if (*pos == '\'') {
            in_single_quotes = !in_single_quotes;
            strncat(result, pos, 1);
            pos++;
        } else if (*pos == '\\' && !in_single_quotes && *(pos + 1) == '$') {
            strncat(result, pos, 2);
            pos += 2;
        } else if (*pos == '$' && !in_single_quotes) {
            pos++;
            const char *start = pos;
            while (*pos && (isalnum(*pos) || *pos == '_')) {
                pos++;
            }
            char varname[pos - start + 1];
            strncpy(varname, start, pos - start);
            varname[pos - start] = '\0';
            char *value = NULL;
            for (char **env = envp; *env != 0; env++) {
                char *thisEnv = *env;
                if (strncmp(thisEnv, varname, strlen(varname)) == 0 && thisEnv[strlen(varname)] == '=') {
                    value = thisEnv + strlen(varname) + 1;
                    break;
                }
            }
            if (value) {
                strncat(result, value, new_size - strlen(result));
            }
        } else {
            char temp[2] = {*pos, '\0'};
            strncat(result, temp, 1);
            pos++;
        }
    }

    strcpy(str, result);  // Copy the result back to the original buffer
    free(result);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void intToStr(int num, char *str) {
    char buffer[10]; // Asumiendo que num no es muy grande
    int i = 0, j, len;
    if (num == 0) {
        str[i++] = '0';
    } else {
        while (num > 0) {
            buffer[i++] = (num % 10) + '0';
            num /= 10;
        }
    }
    buffer[i] = '\0';
    len = i;
    for (j = 0; j < len; j++) {
        str[j] = buffer[len - j - 1];
    }
    str[len] = '\0';
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t ft_strnlen(const char *s, size_t maxlen)
{
    size_t len;
    
    len = 0;
    while (len < maxlen && s[len])
        len++;
    return len;
}

char *ft_strndup(const char *s, size_t n)
{
    char *result;
    size_t len;

    len  = ft_strnlen(s, n);
    result = (char *)malloc(len + 1);
    if (!result)
        return NULL;
    result[len] = '\0';
    return ((char *)ft_memcpy(result, s, len));
}

void actualizar_redireccion(char *inicio_redirecciones, char *ultima_redireccion, char modo_redireccion)
{
    char redir_simbolo[3];
    if (modo_redireccion == 'a')
        ft_strcpy(redir_simbolo, ">>");
    else
        ft_strcpy(redir_simbolo, "> ");
    ft_strcpy(inicio_redirecciones, redir_simbolo);
    ft_strcpy(inicio_redirecciones + ft_strlen(redir_simbolo), ultima_redireccion);
}

void limpiar_redirecciones_restantes(char *inicio_redirecciones, char *ultima_redireccion, char modo_redireccion)
{
    char *pos_eliminar;
    char redir_simbolo[3];
    if (modo_redireccion == 'a')
        ft_strcpy(redir_simbolo, ">>");
    else
        ft_strcpy(redir_simbolo, "> ");
    pos_eliminar = inicio_redirecciones + ft_strlen(ultima_redireccion) + ft_strlen(redir_simbolo);
    while (*pos_eliminar)
    {
        if (*pos_eliminar == '>')
        {
            while (*pos_eliminar == ' ' || *pos_eliminar == '>')
                pos_eliminar++;
            if (*pos_eliminar)
                ft_strcpy(inicio_redirecciones + ft_strlen(redir_simbolo), pos_eliminar);
            else
                *(inicio_redirecciones + ft_strlen(redir_simbolo)) = '\0';
            break;
        }
        pos_eliminar++;
    }
}

void modificar_entrada(char *entrada, char *ultima_redireccion, char modo_redireccion)
{
    char *inicio_redirecciones = ft_strchr(entrada, '>');
    if (inicio_redirecciones)
    {
        actualizar_redireccion(inicio_redirecciones, ultima_redireccion, modo_redireccion);
        limpiar_redirecciones_restantes(inicio_redirecciones, ultima_redireccion, modo_redireccion);
    }
}


int crear_abrir_archivo(const char *archivo, char modo_redireccion)
{
    int flags;
    if (modo_redireccion == 'w')
        flags = O_WRONLY | O_CREAT | O_TRUNC;
    else
        flags = O_WRONLY | O_CREAT | O_APPEND;

    int archivo_fd = open(archivo, flags, 0644);
    if (archivo_fd == -1)
    {
        perror("open");
        return (-1);
    }
    close(archivo_fd);
    return (0);
}

void ajustar_modo_y_pos(char **pos, char *modo_redireccion)
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

char *obtener_archivo_y_actualizar(char *pos, char *entrada_copy, char **ultima_redireccion, char modo_redireccion)
{
    char *token_start;
	size_t token_length;
	char *archivo;

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

char *procesar_redireccion(char *pos, char *entrada_copy, char **ultima_redireccion, char *modo_redireccion)
{
    ajustar_modo_y_pos(&pos, modo_redireccion);
    return obtener_archivo_y_actualizar(pos, entrada_copy, ultima_redireccion, *modo_redireccion);
}


void procesar_todas_redirecciones(char *entrada_copy, char **ultima_redireccion, char *modo_redireccion)
{
    char *pos = entrada_copy;
    while ((pos = ft_strchr(pos, '>')) != NULL)
        pos = procesar_redireccion(pos, entrada_copy, ultima_redireccion, modo_redireccion);
}

void procesarredirecciones(char *entrada)
{
    char *entrada_copy;
    char *ultima_redireccion;
    char modo_redireccion;

    entrada_copy = ft_strdup(entrada);
    if (!entrada_copy)
    {
        ft_printf("malloc failed: Cannot allocate memory\n");
        exit(EXIT_FAILURE);
    }
    ultima_redireccion = NULL;
    modo_redireccion = 'w';
    procesar_todas_redirecciones(entrada_copy, &ultima_redireccion, &modo_redireccion);
    if (ultima_redireccion)
        modificar_entrada(entrada, ultima_redireccion, modo_redireccion);
    free(entrada_copy);
    free(ultima_redireccion);
}


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
void ft_strcat(char *dest, const char *src)
{
    int i;
    int j;
    i = 0;
    j = 0;
    while (dest[i])
        i++;
    while (src[j])
        dest[i++] = src[j++];
    dest[i] = '\0';
}
// Función para verificar si una cadena coincide con un patrón de wildcard
int match_pattern(const char *pattern, const char *str)
{
    if (*pattern == '\0' && *str == '\0')
        return 1;
    if (*pattern == '*')
    {
        // Evitar bucles infinitos si hay múltiples '*' consecutivos
        while (*(pattern + 1) == '*')
            pattern++;
        // Avanzar el patrón y verificar si coincide con el resto de la cadena
        return match_pattern(pattern + 1, str) || (*str && match_pattern(pattern, str + 1));
    }
    if (*pattern == *str)
        return match_pattern(pattern + 1, str + 1);
    return 0;
}
// Declaración de funciones auxiliares
void initialize_variables(char *result, int *pattern_found, int *any_pattern_found, int *wildcard_present);
int process_token(char *token);
int expand_token(char *token, char *result, int *pattern_found);
void finalize_result(char *buf, char *result);
int wildcard_result(int wildcard_present, int any_pattern_found);
void    add_buf(char *result, char *token);
void    add_buf(char *result, char *token)
{
    ft_strcat(result, token);
    ft_strcat(result, " ");
}
int wildcard_result(int wildcard_present, int any_pattern_found)
{
    if (!wildcard_present)
        return 0;  // No hay wildcards
    else if (any_pattern_found)
        return 1;  // Hay wildcards y coincidencias
    else
        return -1; // Hay wildcards pero no coincidencias
}
int expand_wildcards(char *buf)
{
    char result[1000];
    char *token;
    int pattern_found;
    int any_pattern_found;
    int wildcard_present;
    // Inicialización de variables
    initialize_variables(result, &pattern_found, &any_pattern_found, &wildcard_present);
    token = ft_strtok(buf, " ");
    while (token != NULL)
    {
        if (process_token(token))
        {
            wildcard_present = 1;
            pattern_found = 0;
            if (!expand_token(token, result, &pattern_found))
                add_buf(result, token);
            else
                any_pattern_found = 1;
        } else {
            add_buf(result, token);
        }
        token = ft_strtok(NULL, " ");
    }
    finalize_result(buf, result);
    return (wildcard_result(wildcard_present, any_pattern_found));
}
void initialize_variables(char *result, int *pattern_found, int *any_pattern_found, int *wildcard_present)
{
    result[0] = '\0'; // Inicializa result a una cadena vacía
    *pattern_found = 0;
    *any_pattern_found = 0;
    *wildcard_present = 0;
}
int process_token(char *token)
{
    if (ft_strchr(token, '*') != NULL)
        return 1;
    return 0;
}
int expand_token(char *token, char *result, int *pattern_found)
{
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            // Ignorar '.' y '..'
            if (ft_strcmp(dir->d_name, ".") == 0 || ft_strcmp(dir->d_name, "..") == 0)
                continue;
            if (match_pattern(token, dir->d_name))
            {
                ft_strcat(result, dir->d_name);
                ft_strcat(result, " ");
                *pattern_found = 1;
            }
        }
        closedir(d);
    }
    return (*pattern_found);
}
void finalize_result(char *buf, char *result)
{
    size_t len;
    len  = ft_strlen(result);
    if (len > 0 && result[len - 1] == ' ')
        result[len - 1] = '\0';
    ft_strcpy(buf, result);
}

#include <minishell.h>
#include <string.h>
#include <ctype.h>

int execute_builtin(char *input, char ***export, char ***env)
{
    char **split_input;

    if (strncmp(input, "echo", 4) == 0)
        return (echo_builtin(input), 0);
    if (strncmp(input, "exit", 4) == 0)
        return (exit_builtin());
    if (strncmp(input, "env", 3) == 0)
        return (env_builtin(input, *env));
    if (strncmp(input, "export", 6) == 0) {
        split_input = ft_split(input, ' ');
        int result = export_builtin(split_input, export, env);
		free_double(split_input);
        return result;
    }
    if (strncmp(input, "pwd", 3) == 0)
        return (pwd_builtin(input));
    if (strncmp(input, "unset", 5) == 0) {
        split_input = ft_split(input, ' ');
        int result = unset_builtin(export, split_input);
		unset_builtin(env, split_input);
        free_double(split_input);
        return result;
    }
    return (47);
}

int	is_builtin_env(char *input)
{
	if (ft_strncmp(input, "export", 6) == 0 || ft_strncmp(input, "unset", 5) == 0 || ft_strncmp(input, "env", 3) == 0 || ft_strcmp(input, "exit") == 0 || ft_strncmp(input, "exit ", 5) == 0)
		return (1);
	return (0);
}

int	is_builtin(char *input)
{
	if (ft_strncmp(input, "echo", 4) == 0 || ft_strncmp(input, "exit", 4) == 0 || ft_strncmp(input, "env", 3) == 0
		|| ft_strncmp(input, "export", 6) == 0 || ft_strncmp(input, "pwd", 3) == 0 || ft_strncmp(input, "unset", 5) == 0)
		return (1);
	return (0);
}

int execute_cd(char *buf, char **env, char **export)
{
    const char  *path;
    int         result;
    if (((ft_strncmp(buf, "cd", 2) == 0 && (buf[2] == ' ' || buf[2] == '\0'))) && (!strstr(buf, "|") && (!strstr(buf, "<") && (!strstr(buf, ">")))))
    {
        if (buf[2] == ' ')
            path = buf + 3;
        else
            path = NULL;
        result = cd_builtin(path, env, export);
        return (result);
    }
    return (47);
}



void int_to_str(int num, char *str) {
    int i = 0;
    int is_negative = 0;

    // Handle 0 explicitly, otherwise empty string is printed
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    // Handle negative numbers
    if (num < 0) {
        is_negative = 1;
        num = -num;
    }

    // Process individual digits
    while (num != 0) {
        int rem = num % 10;
        str[i++] = rem + '0';
        num = num / 10;
    }

    // If number is negative, append '-'
    if (is_negative)
        str[i++] = '-';

    str[i] = '\0'; // Append null character

    // Reverse the string
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}


// Función para procesar una variable de entorno encontrada en la cadena
char *procesar_variable(char *pos, char **envp, char **res_ptr)
{
    char *start;
    char var_name[1000];
    char *var_ptr = var_name;
    char *env_value;

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
    return(start);
}

// Función principal para expandir variables de entorno en la cadena
void expand_heredoc(char *str, char **envp)
{
    char result[4096];;  // Buffer para la cadena resultante expandida
    char *res_ptr;
    char *pos;

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
    ft_strcpy(str, result);  // Copia la cadena resultante de nuevo en str
}

int g_signal;

void	ft_handle_sigint(int signum)
{
	(void)signum;
	if (g_signal == 1)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		write(1, "\033[K\n", 5);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (g_signal == 2)
	{
		write(1, "\033[K\n", 5);
		rl_on_new_line();
		rl_replace_line("", 0);
		g_signal = 4;
	}
	else if (g_signal == 3)
	{
		write(1, "\033[K\n", 5);
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	ft_handle_sigquit(int signum)
{
	(void)signum;
	if (g_signal == 1)
		;
	else if (g_signal == 2)
		;
	else if (g_signal == 3)
	{
		write(1, "Quit (core dumped)\n", sizeof(char) * 19);
		exit(0);
	}
}

int construir_nuevo_archivo(char *heredocStart, char *input, char *delimiterEnd, char *nombreArchivo)
{
    size_t  lenBeforeHeredoc;
    size_t  lenAfterDelimiter;
    size_t  nuevoStringLen;
    char    nuevoString[4096];

    lenBeforeHeredoc = heredocStart - input;
    lenAfterDelimiter = ft_strlen(delimiterEnd);
    nuevoStringLen = lenBeforeHeredoc + ft_strlen("< ") + ft_strlen(nombreArchivo) + lenAfterDelimiter + 1;
    if (nuevoStringLen > 4096)
    {
        ft_printf("El nuevo string excede el tamaño del buffer.\n");
        return (1);
    }
    strncpy(nuevoString, input, lenBeforeHeredoc);
    nuevoString[lenBeforeHeredoc] = '\0';
    ft_strcat(nuevoString, "< ");
    ft_strcat(nuevoString, nombreArchivo);
    ft_strcat(nuevoString, delimiterEnd);
    strncpy(input, nuevoString, 4096);
    input[4095] = '\0';
    heredocStart = input + lenBeforeHeredoc + ft_strlen("< ") + ft_strlen(nombreArchivo);
    return (0);
}

void    construir_archivo_heredoc(char *nombreArchivo, int heredocCount)
{
    char heredocCountStr[10];
    
    heredocCount++;
    g_signal = 2;
    ft_strcpy(nombreArchivo, "archivo_creado_");
    int_to_str(heredocCount, heredocCountStr);
    strcat(nombreArchivo, heredocCountStr);
    strcat(nombreArchivo, ".txt");
}

int hijo_done(pid_t pid)
{
    int status;

    signal(SIGINT, ft_handle_sigint);
    waitpid(pid, &status, 0);
    if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
    {
        kill(pid, SIGINT);
        return (1);  // Enviar SIGINT al hijo si se recibió en el padre
    }
    if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
        return (1);
    return (0);
}

void lectura_heredoc(char *linea, char *delimiterStr, char **env, int archivo)
{
    while (1)
    {
        linea = readline("> ");
        if (linea == NULL)
        {
            printf("warning: here-document at line delimited by end-of-file (wanted `%s')\n", delimiterStr);
            break;
        }
        linea[strcspn(linea, "\n")] = 0;
        if (ft_strcmp(linea, delimiterStr) == 0)
        {
            free(linea);
            break;
        }
        expand_heredoc(linea, env);
        ft_putstr_fd(linea, archivo);
        ft_putchar_fd('\n', archivo);
        free(linea);
    }
}

void proceso_hijo(char *linea, char *delimiterStr, char **env, int archivo, char *nombreArchivo)
{
    archivo = open(nombreArchivo, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (archivo == -1)
    {
        perror("Error al crear el archivo");
        exit(1);
    }
    signal(SIGINT, 0);  // Ignorar SIGINT en el hijo
    lectura_heredoc(linea, delimiterStr, env, archivo);
    close(archivo);
    exit(0);
}
void extraer_delimiter(char *delimiter, char *delimiterStr, size_t *lenDelimiter)
{
    char *delimiterEnd;

    delimiterEnd = delimiter;
    while (*delimiterEnd != ' ' && *delimiterEnd != '\0' && *delimiterEnd != '|' && *delimiterEnd != '>')
        delimiterEnd++;
    *lenDelimiter = delimiterEnd - delimiter;
    strncpy(delimiterStr, delimiter, *lenDelimiter);
    delimiterStr[*lenDelimiter] = '\0';
}

void avanza_delimiter(char **delimiter, char *heredocStart)
{
    // Inicializar el delimitador con heredocStart + 2
    *delimiter = heredocStart + 2;

    // Saltar cualquier espacio en blanco después de "<<"
    while (**delimiter == ' ') {
        (*delimiter)++;
    }
}

int manejarProcesoHeredoc(char *heredocStart, char *input, char **env)
{
    char *delimiter;
    char nombreArchivo[256];
    char delimiterStr[256];
    size_t lenDelimiter;
    pid_t pid;

    avanza_delimiter(&delimiter, heredocStart);
    extraer_delimiter(delimiter, delimiterStr, &lenDelimiter);
    construir_archivo_heredoc(nombreArchivo, 0);
    pid = fork();
    if (pid == -1)
    {
        perror("Error al crear el proceso hijo");
        return (1);
    } else if (pid == 0)
    {
        proceso_hijo(NULL, delimiterStr, env, 0, nombreArchivo);
    } else
    {
        if (hijo_done(pid) || construir_nuevo_archivo(heredocStart, input, delimiter + lenDelimiter, nombreArchivo))
            return (1);
    }
    return (0);
}

int procesarHeredoc(char *input, char **env)
{
    char *heredocStart;

    heredocStart = input;
    while ((heredocStart = strstr(heredocStart, "<<")) != NULL)
    {
        if (manejarProcesoHeredoc(heredocStart, input, env))
            return (1);
        heredocStart += 2;
    }
    return (0);
}

void eliminarArchivos(void)
{
    char nombreArchivo[256];
    int i = 1; // Comenzar con el primer archivo

    while (1) { // Bucle infinito hasta que no se puedan eliminar más archivos
        strcpy(nombreArchivo, "archivo_creado_");
        char heredocCountStr[10];
        int_to_str(i, heredocCountStr);
        strcat(nombreArchivo, heredocCountStr);
        strcat(nombreArchivo, ".txt");

        if (unlink(nombreArchivo) == 0) {
            i++; // Incrementar el contador para el próximo archivo
        } else {
            break; // Salir del bucle si no se puede eliminar el archivo
        }
    }
}

# include <termios.h>

void	suppress_output(void)
{
	struct termios	termios_p;

	if (isatty(0))
	{
		if (tcgetattr(0, &termios_p) != 0)
			perror("Minishell: tcgetattr");
		termios_p.c_lflag &= ~ECHOCTL;
		if (tcsetattr(0, 0, &termios_p) != 0)
			perror("Minishell: tcsetattr");
	}
}

#include <linux/limits.h>

void	final_clean(char **exp, char **env)
{
	eliminarArchivos();
	free_double(exp);
	free_double(env);
}

void	setup_executor(char *buf, char **env)
{
	g_signal = 3;
	signal(SIGQUIT, ft_handle_sigquit);
	signal(SIGINT, ft_handle_sigint);
	if (save_fork() == 0)    
		runcmd(parse_cmd(buf), env);
    wait(0);
	eliminarArchivos();
}

void	setup_shell(char ***copy_exp, char ***copy_en, char **env)
{
	*copy_exp = copy_env(env);
	*copy_en = copy_env(env);
	g_signal = 1;
	signal(SIGINT, ft_handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	suppress_output();
}

void process_commands(char *trimmed, char *buf, char ***copy_en, char ***copy_export)
{
    ft_strcpy(buf, trimmed);
    if (procesarHeredoc(buf, *copy_en) == 1)
    {
        eliminarArchivos();
        return;
    }
    expand(buf, *copy_en);
    procesarredirecciones(buf);
    expand_wildcards(buf);
    if (execute_cd(buf, *copy_en, *copy_export) == 0)
        return;
    if (is_builtin_env(buf) && (!strstr(buf, "|") || !strstr(buf, ">") || !strstr(buf, "<")))
    {
        execute_builtin(buf, copy_export, copy_en);
        if (ft_strcmp(buf, "exit") == 0 || ft_strncmp(buf, "exit ", 5) == 0)
            exit(0);
    }
    else
    {
        setup_executor(buf, *copy_en);
    }
}

void process_input(char *input, char ***copy_en, char ***copy_export)
{
    char buf[PATH_MAX];
    char *trimmed;

    if (ft_strlen(input) > PATH_MAX - 1)
    {
        free(input);
        return;
    }
    trimmed = ft_strtrim(input, " ");
    free(input);
    if (trimmed == NULL || trimmed[0] == '\0') 
    {
        free(trimmed);
        return;
    }
    if (!validator(trimmed))
    {
        ft_printf("syntax error near unexpected token `newline'\n");
        free(trimmed);
        return;
    }
    process_commands(trimmed, buf, copy_en, copy_export);
    free(trimmed);
}

int main(int args, char **argv, char **env)
{
    char **copy_export;
    char **copy_en;

    (void)args;
    argv++;
    setup_shell(&copy_export, &copy_en, env);
    while(1)
    {
        g_signal = 1;
        char *input = display_prompt();
        if (input == NULL)
        {
            exit_builtin();
            break ;
        }
        process_input(input, &copy_en, &copy_export);
    }
    final_clean(copy_export, copy_en);
}