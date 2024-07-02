#include <minishell.h>

static int	word_count(char const *s, char c)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s[i])
	{
		while (s[i] == c && s[i])
		{
			i++;
		}
		if (s[i] != c && s[i])
		{
			j++;
		}
		while (s[i] != c && s[i])
		{
			i++;
		}
	}
	return (j);
}

static int	str_len(char const *s, int c, int i)
{
	int	len;

	len = 0;
	while (s[i] && s[i] != c)
	{
		len++;
		i++;
	}
	return (len);
}

static char	**free_all(char **s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		free(s[i++]);
	}
	free(s);
	return (NULL);
}

static int	skip_prefix(char const *s, char c)
{
	int	i;

	i = 0;
	while ((s[i] == c && s[i]) && i < 1)
	{
		i++;
	}
	return (i);
}

char	**ft_split_m(char const *s)
{
    char	**array;
    int		start;
    int		len;
    int		i;
    int		array_num;
    char	c;

    array_num = -1;
    i = 0;
    array = (char **)malloc((word_count(s, ' ') + 1) * sizeof(char *));
    if (!s || array == 0)
        return (0);
    while (s[i])
    {
        len = 0;
		c = ' ';
		if (s[i] == '"')
		{
			c = '"';
			i++;
		}
		else if (s[i] == '\'')
		{
			c = '\'';
			i++;
		}
		else if (s[i] == ' ')
		{
			i += skip_prefix(&s[i], c);
		}
        start = i;
        while (s[i] && s[i] != c)
            i++;
        len = i - start;
        if (len)
        {
            array[++array_num] = ft_substr(s, start, len);
            if (array_num > 0 && array[array_num][0] == ' ')
            {
                free(array[array_num]);
                array_num--;
            }
        }
        if (len && !array[array_num])
            return (free_all(array));
        if (s[i])
            i++;
        while (s[i] && s[i] == ' ')
            i++;
    }
    array[++array_num] = NULL;
    return (array);
}