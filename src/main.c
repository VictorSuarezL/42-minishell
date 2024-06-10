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

struct cmd *parse_cmd(char *str)
{
    int i = 0;
    char *end_str;
    struct execcmd *cmd;
    struct cmd *ret;
    char *q, *eq;

    end_str = &str[ft_strlen(str)];
    ret = execcmd();


    while (str[i] && !ft_strchr("|", str[i]))
    {
        while ()
        {
            /* code */
        }
        
        


        

    }
    
}

int main(void)
{
    char line[100]="echo -n esto";

    parse_cmd(line);
}