#include <minishell.h>

#define MAXARGS 10

#define EXEC  1
#define REDIR 2
#define PIPE  3
#define LIST  4
#define BACK  5
#define MAXARGS 10

struct execcmd {
  int type;
  char *argv[MAXARGS];
  char *eargv[MAXARGS];
};
char whitespace[] = " \t\r\n\v";
char symbols[] = "<|>&;()";

struct cmd
{
    int type;
};

int gettoken(char **ps, char *es, char **q, char **eq)
{
    char *s;
    int ret;
    s = *ps;
    while (s < es && strchr(whitespace, *s))
        s++;
    if (q)
        *q = s;
    ret = *s;

    if (*s == 0) 
    {
        // No operation needed
    } 
    // else if (*s == '|' || *s == '(' || *s == ')' || *s == ';' || *s == '&' || *s == '<') 
    else if (ft_strchr("|();&<", *s)) 
    {
        s++;
    } 
    else if (*s == '>') 
    {
        s++;
        if (*s == '>') 
        {
            ret = '+';  // Indicate append redirection
            s++;
        }
    }
    else if (*s == '"' || *s == '\'') 
    { // Añadir manejo de comillas
        char quote = *s;
        s++;
        ret = 'a'; // Argumento entre comillas
        while (s < es && *s != quote) 
        {
            s++;
        }
        if (*s == quote) 
        {
            s++;
        } 
    }
    else 
    {
        ret = 'a';
        while (s < es && !strchr(whitespace, *s) && !strchr(symbols, *s))
            s++;
    }

    if (eq)
        *eq = s;
    
    while (s < es && strchr(whitespace, *s))
        s++;
    *ps = s;
    return ret;
}


int peek(char **ps, char *es, char *toks)
{
    char *s;
    s = *ps;
    while (s < es && ft_strchr(" \t\r\n\v", *s))
    {
        s++;
    }
    *ps = s;
    if (*s && ft_strchr(toks, *s))
        return 1;
    else
        return 0;
}

// int peek(char **ps, char *es, char *toks)
// {
//   char *s;
//   s = *ps;
//   while(s < es && strchr(whitespace, *s))
//     s++;
//   *ps = s;
//   return *s && strchr(toks, *s);
// }

struct cmd*execcmd(void)
{
  struct execcmd *cmd;
  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = EXEC;
  return (struct cmd*)cmd;
}

struct cmd*parseexec(char **ps, char *es)
{
  char *q, *eq;
  int tok; 
  int argc;
  struct execcmd *cmd;
  struct cmd *ret;
//   if(peek(ps, es, "("))
//     return parseblock(ps, es);
  ret = execcmd();
  cmd = (struct execcmd*)ret;
  argc = 0;
//   ret = parseredirs(ret, ps, es);
  while(!peek(ps, es, "|)&;"))
  {
    tok=gettoken(ps, es, &q, &eq);
    if(tok == 0)
      break;
    if(tok != 'a')
      ft_error("syntax");
    cmd->argv[argc] = q;
    cmd->eargv[argc] = eq;
    argc++;
    if(argc >= MAXARGS)
      ft_error("too many args");
    // ret = parseredirs(ret, ps, es);
  }
  cmd->argv[argc] = 0;
  cmd->eargv[argc] = 0;
  printf("argv: %s\n", *cmd->argv);
  printf("eargv: %s\n", *cmd->eargv);
  return ret;
}

/**
 * Parses a command line and returns a linked list of commands.
 * 
 * Se encarga de gestionar los & o las ;
 *
 * @param ps The command line string to parse.
 * @param es The end of the command line string.
 * @return A pointer to the head of the linked list of commands.
 */
struct cmd *parseline(char **ps, char *es)
{
    struct cmd *cmd;
    // cmd = parsepipe(ps, es);
    cmd = parseexec(ps, es);
    // Handles background commands, each time it finds an '&', it calls gettoken
    // while(peek(ps, es, "&")){
    //     gettoken(ps, es, 0, 0);
    //     cmd = backcmd(cmd);
    // }
    
    // if(peek(ps, es, ";")){
    //     gettoken(ps, es, 0, 0);
    //     cmd = listcmd(cmd, parseline(ps, es));
    // }
    
    return cmd;
}

struct cmd *parsecmd(char *s)
{
    char *es;
    struct cmd *cmd;
    es = s + ft_strlen(s);

    cmd = parseline(&s, es);

    // Este bloque se asegura que todo el texto ha sido procesado y no queda nada más
    peek(&s, es, "");
    if (s != es)
    {
        // ft_putendl_fd(s, STDERR_FILENO);
        ft_error("syntax error");
    }
    // printf("cool!");
    return cmd;
}

int main(void)
{
    char line[100] = "echo '>'";
    parsecmd(line);

    char l[100] = "echo '-n mundo'";
    parsecmd(l);
}