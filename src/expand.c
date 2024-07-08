#include <minishell.h>

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