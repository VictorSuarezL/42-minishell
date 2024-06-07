#include <minishell.h>


#define EXEC  1
#define REDIR 2
#define PIPE  3
#define LIST  4
#define BACK  5
#define MAXARGS 10

struct cmd
{
    int type;
};

struct execcmd {
  int type;
  char *argv[MAXARGS];
  char *eargv[MAXARGS];
};

char whitespace[] = " \t\r\n\v";
char symbols[] = "<|>&;()";

int peek(char **ps, char *es, char *toks)
{
    int i = 0;  // Índice para el acceso a través del string
    char *s = *ps;
    int length = es - s;  // Longitud máxima calculada basada en es

    // Salta caracteres de espacio en blanco
    while (i < length && ft_strchr(" \t\r\n\v", s[i]))
        i++;
    
    *ps = &s[i];  // Usando &s[i] para la claridad

    // Verifica si el caracter actual está dentro de los toks buscados
    if (i < length && s[i] && ft_strchr(toks, s[i]))
        return 1;
    else
        return 0;
}


int get_token(char **ps, char *es, char **q, char **eq)
{
    int i = 0;  // índice para manejar la posición relativa en el string
    int ret;
    char *s = *ps;
    int length = es - s;  // Cálculo de la longitud máxima basado en es

    // Salta espacios blancos iniciales
    while (i < length && ft_strchr(whitespace, s[i]))
        i++;
    
    if (q)
        *q = &s[i];  // Usando &s[i] para la claridad
    ret = s[i];

    // Chequeo de fin de string
    if (s[i] == 0) 
    {
        // No operation needed
    } 
    else if (ft_strchr("|();&<", s[i])) 
    {
        i++;
    } 
    // else if (s[i] == '>') 
    // {
    //     i++;
    //     if (s[i] == '>') 
    //     {
    //         ret = '+';  // Indicate append redirection
    //         i++;
    //     }
    // }
    // else if (s[i] == '"' || s[i] == '\'') 
    // {
    //     char quote = s[i];
    //     i++;
    //     ret = 'a'; // Argumento entre comillas
    //     while (i < length && s[i] != quote) 
    //     {
    //         i++;
    //     }
    //     if (s[i] == quote) 
    //     {
    //         i++;
    //     } 
    // }
    else 
    {
        ret = 'a';
        while (i < length && !ft_strchr(whitespace, s[i]) && !ft_strchr(symbols, s[i]))
            i++;
    }

    if (eq)
        *eq = &s[i];  // Usando &s[i] para la claridad
    
    // Saltar espacios blancos finales
    while (i < length && ft_strchr(whitespace, s[i]))
        i++;

    *ps = &s[i];  // Actualización del puntero de inicio
    return ret;
}

struct cmd *parse_exec(char **p_str, char *end_str)
{
    char *q;
    char *eq;
    int tok;
    struct execcmd *cmd;
    struct cmd *ret;

    while(!peek(p_str, end_str, "|)&;"))
    {
        tok = get_token(p_str, end_str, &q, &eq);
        if(tok == 0)
            break;
        if(tok != 'a')
            ft_error("syntax");
        // printf("q: %s", q);
        // printf("eq: %s", eq);
    }
    return ret;
}


struct cmd *parse_line(char **p_str, char *es)
{
    struct cmd *cmd;
    //Eventualmente parsear pipe
    cmd = parse_exec(p_str, es);
    return cmd;
}


struct cmd *parse_cmd(char *str)
{
    char *end_str;
    struct cmd *cmd;
    int i = 0;

    end_str = &str[ft_strlen(str)];
    // printf("%s", end_str);
    cmd = parse_line(&str, end_str);

    return cmd;
}

int main(void)
{
    char line[100]="echo > esto es un ejemplo";

    parse_cmd(line);
}