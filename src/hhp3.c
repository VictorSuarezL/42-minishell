#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

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
    char *aux;
    int ret;
    aux = *ps;
    while (aux < es && strchr(whitespace, *aux))
        aux++;
    if (q)
        *q = aux;
    ret = *aux;

    if (*aux == 0) 
    {
        // No operation needed
    } 
    else if (strchr("|();&<", *aux)) 
    {
        aux++;
    } 
    else if (*aux == '>') 
    {
        aux++;
        if (*aux == '>') 
        {
            ret = '+';  // Indicate append redirection
            aux++;
        }
    }
    else if (*aux == '"' || *aux == '\'') 
    { // Añadir manejo de comillas
        char quote = *aux;
        aux++;
        ret = 'a'; // Argumento entre comillas
        while (aux < es && *aux != quote) 
        {
            aux++;
        }
        if (*aux == quote) 
        {
            aux++;
        } 
    }
    else 
    {
        ret = 'a';
        while (aux < es && !strchr(whitespace, *aux) && !strchr(symbols, *aux))
            aux++;
    }

    if (eq)
        *eq = aux;
    
    while (aux < es && strchr(whitespace, *aux))
        aux++;
    *ps = aux;
    return ret;
}


int peek(char **ps, char *es, char *toks)
{
    int i = 0;  // Índice para el acceso a través del string
    char *s = *ps;
    int length = es - s;  // Longitud máxima calculada basada en es

    // Salta caracteres de espacio en blanco
    while (i < length && strchr(" \t\r\n\v", s[i]))
        i++;
    
    *ps = &s[i];  // Usando &s[i] para la claridad

    // Verifica si el caracter actual está dentro de los toks buscados
    if (i < length && s[i] && strchr(toks, s[i]))
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

struct cmd*parseexec(char **p_str, char *end_str)
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
  while(!peek(p_str, end_str, "|)&;"))
  {
    tok=gettoken(p_str, end_str, &q, &eq);
    if(tok == 0)
      break;
    if(tok != 'a')
        printf("syntax");
    cmd->argv[argc] = q;
    cmd->eargv[argc] = eq;
    argc++;
    // if(argc >= MAXARGS)
    //   ft_error("too many args");
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


struct cmd*
nulterminate(struct cmd *cmd)
{
  int i;
//   struct backcmd *bcmd;
  struct execcmd *ecmd;
//   struct listcmd *lcmd;
//   struct pipecmd *pcmd;
//   struct redircmd *rcmd;
  if(cmd == 0)
    return 0;
  switch(cmd->type){
  case EXEC:
    ecmd = (struct execcmd*)cmd;
    for(i=0; ecmd->argv[i]; i++)
      *ecmd->eargv[i] = 0;
    break;
//   case REDIR:
//     rcmd = (struct redircmd*)cmd;
//     nulterminate(rcmd->cmd);
//     *rcmd->efile = 0;
//     break;
//   case PIPE:
//     pcmd = (struct pipecmd*)cmd;
//     nulterminate(pcmd->left);
//     nulterminate(pcmd->right);
//     break;
//   case LIST:
//     lcmd = (struct listcmd*)cmd;
//     nulterminate(lcmd->left);
//     nulterminate(lcmd->right);
//     break;
//   case BACK:
//     bcmd = (struct backcmd*)cmd;
//     nulterminate(bcmd->cmd);
//     break;
  }
  return cmd;
}

struct cmd *parsecmd(char *str)
{
    char *end_str;
    struct cmd *cmd;
    // len = strlen(str);
    // end_str = str + strlen(str);
    end_str = &str[strlen(str)];
    // printf("end_str: %s\n", end_str);
    cmd = parseline(&str, end_str);

    // Este bloque se asegura que todo el texto ha sido procesado y no queda nada más
    // peek(&s, es, "");
    // if (s != es)
    // {
    //     // ft_putendl_fd(s, STDERR_FILENO);
    //     ft_error("syntax error");
    // }
    // // printf("cool!");
    nulterminate(cmd);
    return cmd;
}

int main(void)
{
    char line[100] = "echo -n hola | hola";
    parsecmd(line);

    // char l[100] = "echo '-n mundo'";
    // parsecmd(l);
}