#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>


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

// Constructores
struct cmd*
execcmd(void)
{
  struct execcmd *cmd;
  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = EXEC;
  return (struct cmd*)cmd;
}

int gettoken(char *p_str, char *end_str, char **q, char **eq)
{
    int i, start;
    // char *p_str;
    int ret;

    ret = 0;
    i = 0;
    // *p_str = s;
    while (p_str[i] && strchr(whitespace, p_str[i]))
        i++;
    printf("p_str: %s\n", p_str);
    if (q)
        *q = &p_str[i];
    
    // start = i;

    if (!p_str[i])
    {
        ret = 0;
    }
    else if (strchr("|();&", p_str[i]))
    {
        i++;
    }
    // // else if (s[i] == '>') 
    // // {
    // //     i++;
    // //     if (s[i] == '>') 
    // //     {
    // //         ret = '+';  // Indicar redirección de append
    // //         i++;
    // //     }
    // // } 
    else
    {
        ret = 'a';
        while (p_str[i] && !strchr(whitespace, p_str[i]) && !strchr(symbols, p_str[i]))
        {
            i++;
        }
    }
    if (eq)
    {
        *eq = &p_str[i];
    }
    while (p_str[i] && strchr(whitespace, p_str[i]))
    {
        i++;
    }

    p_str = &p_str[i];
    return ret;
}

struct cmd* nulterminate(struct cmd *cmd)
{
    struct execcmd *ecmd;

    int i = 0;

    if (cmd->type == EXEC)
    {
        ecmd = (struct execcmd*)cmd;

        while (ecmd->argv[i])
        {
            *ecmd->eargv[i] = 0;
            i++;
        }
        
    }
    return cmd;
}

struct cmd *parse_cmd(char *str)
{
    int i = 0;
    char *end_str;
    struct cmd *or;
    struct execcmd *cmd;
    struct cmd *ret;
    char *q, *eq;
    int tok;
    int argc = 0;


    end_str = &str[strlen(str)];
    ret = execcmd();
    cmd = (struct execcmd*)ret;
    argc = 0;

    while(str[i] && !strchr("|", str[i]))
    {
        while (str[i] && strchr(whitespace, str[i]))
            i++;
        // printf("str[i] = %c\n", str[i]);
        tok = gettoken(&str[i], end_str, &q, &eq);
        if (!tok)
        {
            printf("break!");
            break;
        }
        if (tok != 'a')
        {
            printf("syntax");
        }
        cmd->argv[argc] = q;
        cmd->eargv[argc] = eq;
        argc++;
        printf("argc: %d\n", argc);
        printf("q: %s\n", q);
        printf("eq: %s\n", eq);
        while (str[i] && !strchr(whitespace, str[i]))
        {
            i++;
        }
        if (str[i])
        {
            i++;
        }
        
    }
    cmd->argv[argc] = 0;
    cmd->eargv[argc] = 0;
    or = (struct cmd *)cmd;
    nulterminate(or);
    return ret;
}

int main(void)
{
    char line[100]="    echo -n esto | hola";

    parse_cmd(line);
}