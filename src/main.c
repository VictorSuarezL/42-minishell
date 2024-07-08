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

void expand_heredoc(char *str, char **envp);
char *get_env_value(const char *name, char **envp);
// Función para procesar el tamaño de la variable de entorno
void process_variable_size(const char **str, char **envp, size_t *size)
{
    const char *start = *str;
    char varname[256];
    size_t varname_len;
    char *value;
    
    while (**str && (ft_isalnum(**str) || **str == '_'))
        (*str)++;
    varname_len = *str - start;
    strncpy(varname, start, varname_len);
    varname[varname_len] = '\0';
    value = get_env_value(varname, envp);
    if (value)
        *size += ft_strlen(value);
}

// Función para procesar caracteres individuales en la cadena
void process_single_character(const char **str, size_t *size)
{
    (*size)++;
    (*str)++;
}

// Función para procesar comillas simples en la cadena
void process_single_quote(const char **str, size_t *size, int *in_single_quotes)
{
    *in_single_quotes = !*in_single_quotes;
    (*size)++;
    (*str)++;
}

// Función para procesar secuencias de escape en la cadena
void process_escape_sequence(const char **str, size_t *size)
{
    (*size) += 2;
    (*str) += 2;
}

// Función principal para calcular el tamaño necesario para la cadena expandida
size_t calculate_size(const char *str, char **envp)
{
    size_t size = 0;
    int in_single_quotes = 0;

    while (*str)
    {
        if (*str == '\'')
        {
            process_single_quote(&str, &size, &in_single_quotes);
        }
        else if (*str == '\\' && !in_single_quotes && *(str + 1) == '$')
        {
            process_escape_sequence(&str, &size);
        }
        else if (*str == '$' && !in_single_quotes)
        {
            str++;
            process_variable_size(&str, envp, &size);
        }
        else
        {
            process_single_character(&str, &size);
        }
    }
    return size;
}

// Función para procesar y expandir una variable de entorno
void process_variable_expand(const char **pos, char **envp, char *result, size_t new_size)
{
    char varname[256];
    size_t varname_len;
    char *value;
    const char *start;
	
	start = *pos;
    while (**pos && (ft_isalnum(**pos) || **pos == '_'))
        (*pos)++;
    varname_len = *pos - start;
    ft_strncpy(varname, start, varname_len);
    varname[varname_len] = '\0';
    value = get_env_value(varname, envp);
    if (value)
        ft_strncat(result, value, new_size - ft_strlen(result));
}

void handle_single_quote(const char **pos, int *in_single_quotes, char *result)
{
    *in_single_quotes = !(*in_single_quotes);
    ft_strncat(result, *pos, 1);
    (*pos)++;
}

void handle_escaped_dollar(const char **pos, char *result)
{
    strncat(result, *pos, 2);
    *pos += 2;
}

void handle_variable_expand(const char **pos, char **envp, char *result, size_t new_size)
{
    (*pos)++;
    process_variable_expand(pos, envp, result, new_size);
}

void handle_regular_char(const char **pos, char *result)
{
    char temp[2];

    temp[0] = **pos;
    temp[1] = '\0';
    strncat(result, temp, 1);
    (*pos)++;
}
void expand(char *str, char **envp)
{
    size_t new_size;
    char *result;
    const char *pos;
    int in_single_quotes;

    new_size = calculate_size(str, envp);
    result = malloc(new_size + 1);
    pos = str;
    in_single_quotes = 0;
    if (result == NULL)
    {
        ft_printf("Error: unable to allocate memory\n");
        exit (1);
    }
    result[0] = '\0';
    while (*pos)
    {
        if (*pos == '\'')
            handle_single_quote(&pos, &in_single_quotes, result);
        else if (*pos == '\\' && !in_single_quotes && *(pos + 1) == '$')
            handle_escaped_dollar(&pos, result);
        else if (*pos == '$' && !in_single_quotes)
            handle_variable_expand(&pos, envp, result, new_size);
        else
            handle_regular_char(&pos, result);
    }
    ft_strcpy(str, result);
    free(result);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void intToStr(int num, char *str)
{
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
    char *pos;
    
    pos = entrada_copy;
    pos = ft_strchr(pos, '>');
    while (pos != NULL && (*(pos - 1) != '\\'))
    {
        pos = procesar_redireccion(pos, entrada_copy, ultima_redireccion, modo_redireccion);
        pos = ft_strchr(pos, '>');
    }
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
    char    **split_input;
    int     result;

    if (ft_strncmp(input, "echo", 4) == 0)
        return (echo_builtin(input), 0);
    if (ft_strncmp(input, "exit", 4) == 0)
        return (exit_builtin());
    if (ft_strncmp(input, "env", 3) == 0)
        return (env_builtin(input, *env));
    if (ft_strncmp(input, "export", 6) == 0)
    {
        split_input = ft_split(input, ' ');
        result = export_builtin(split_input, export, env);
        return (free_double(split_input), result);
    }
    if (ft_strncmp(input, "pwd", 3) == 0)
        return (pwd_builtin(input));
    if (ft_strncmp(input, "unset", 5) == 0)
    {
        split_input = ft_split(input, ' ');
        result = unset_builtin(export, split_input);
		unset_builtin(env, split_input);
        return (free_double(split_input), result);
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

    if (((ft_strncmp(buf, "cd", 2) == 0 && (buf[2] == ' ' || buf[2] == '\0'))) && (!ft_strstr(buf, "|") && (!ft_strstr(buf, "<") && (!ft_strstr(buf, ">")))))
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



void int_to_str(int num, char *str)
{
    int i;
    int is_negative;

    i = 0;
    is_negative = 0;
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return;
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


char    *procesar_variable(char *pos, char **envp, char **res_ptr)
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


void    expand_heredoc(char *str, char **envp)
{
    char result[4096];
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
    ft_strcpy(str, result);
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
    ft_strncpy(nuevoString, input, lenBeforeHeredoc);
    nuevoString[lenBeforeHeredoc] = '\0';
    ft_strcat(nuevoString, "< ");
    ft_strcat(nuevoString, nombreArchivo);
    ft_strcat(nuevoString, delimiterEnd);
    ft_strncpy(input, nuevoString, 4096);
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
    ft_strcat(nombreArchivo, heredocCountStr);
    ft_strcat(nombreArchivo, ".txt");
}

int hijo_done(pid_t pid)
{
    int status;

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

void lectura_heredoc(char *linea, char *delimiterStr, char **env, int archivo)
{
    while (1)
    {
        linea = readline("> ");
        if (linea == NULL)
        {
            ft_printf("warning: here-document at line delimited by end-of-file (wanted `%s')\n", delimiterStr);
            break;
        }
        linea[ft_strcspn(linea, "\n")] = 0;
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
    signal(SIGINT, 0);
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
    ft_strncpy(delimiterStr, delimiter, *lenDelimiter);
    delimiterStr[*lenDelimiter] = '\0';
}

void avanza_delimiter(char **delimiter, char *heredocStart)
{
    *delimiter = heredocStart + 2;
    while (**delimiter == ' ')
        (*delimiter)++;
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
    while ((heredocStart = ft_strstr(heredocStart, "<<")) != NULL)
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
    char heredocCountStr[10];
    int i;

    i = 1;
    while (1)
    {
        ft_strcpy(nombreArchivo, "archivo_creado_");
        int_to_str(i, heredocCountStr);
        ft_strcat(nombreArchivo, heredocCountStr);
        ft_strcat(nombreArchivo, ".txt");
        if (unlink(nombreArchivo) == 0)
            i++;
        else
            break;
    }
}


void process_commands(char *trimmed, char *buf, char ***copy_en, char ***copy_export)
{
    int a;

    ft_strcpy(buf, trimmed);
    quote_manager(buf, 0, 0);
    if (procesarHeredoc(buf, *copy_en) == 1)
    {
        eliminarArchivos();
        return;
    }
    escape_special_chars(buf);
    expand(buf, *copy_en);
    procesarredirecciones(buf);
    expand_wildcards(buf);
    pop_slash(buf);
    a = execute_cd(buf, *copy_en, *copy_export);
    if (a == 0 || a == 1)
        return;
    if (is_builtin_env(buf) && (!ft_strstr(buf, "|") && !ft_strstr(buf, ">") && !ft_strstr(buf, "<")))
    {
        execute_builtin(buf, copy_export, copy_en);
        if (ft_strcmp(buf, "exit") == 0 || ft_strncmp(buf, "exit ", 5) == 0)
            exit(0);
    }
    else
        setup_executor(buf, *copy_en, *copy_export);
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
int g_signal;

int main(int args, char **argv, char **env)
{
    char    **copy_export;
    char    **copy_en;
    char    *input;
    
    (void)args;
    argv++;
    setup_shell(&copy_export, &copy_en, env);
    while(1)
    {
        g_signal = 1;
        input = display_prompt();
        if (input == NULL)
        {
            exit_builtin();
            break ;
        }
        process_input(input, &copy_en, &copy_export);
    }
    final_clean(copy_export, copy_en);
}
