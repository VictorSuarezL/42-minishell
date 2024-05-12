#include <minishell>


/**
 * Flag the number of single and double quotes in a given string.
 * 
 * @param str The string to count quotes in.
 * @return The number of quotes found in the string.
 */
int quote_counter(char *str)
{
    int i = 0;
    int flag = 0;

    while (str[i])
    {
        if (str[i] == '\"' || str[i] == '\'')
        {
            flag = 1;
        }
        i++;
    }
    return flag;
}

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


