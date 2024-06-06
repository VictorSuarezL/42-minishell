#include <minishell.h>

struct cmd
{
    int type;
};

int peek(char **p_str, char *end_str, char *chars)
{
    char *aux;
    aux = *p_str;

    while (aux < end_str && ft_strchr(" \t\r\n\v", aux))
    {
        aux++;

    }
    
}

struct cmd *parseexec(char **p_str, char *end_str)
{
    char *q;
    char *eq;
    int tok;
    
    while (!peek(p_str, &eq, &q, &end_str))
    {

    }
    
}


struct cmd *parseline(char **p_str, char *es)
{
    struct cmd *cmd;
    //Eventualmente parsear pipe
    cmd = parseexec(p_str, es);

}


struct cmd *parsecmd(char *str)
{
    char *end_str;
    struct cmd *cmd;
    int i = 0;

    end_str = &str[ft_strlen(str)];
    cmd = parseline(&str, end_str);

    return cmd;
}

int main(void)
{
    char line[100]="echo > esto es un ejemplo";

    parsecmd(line);
}