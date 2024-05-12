#include <minishell.h>

int is_pipe_end(char *str)
{
    int i = 0;
    int len;

    len = ft_strlen(str);

    if (str[len-1] == '|')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
