#include <minishell.h>

#include <stdio.h>
#include <string.h>
#include <fcntl.h>

// int main()
// {
// 	char	str[100] = "cat << ENF      ";
// 	char	*delimiter = NULL;
// 	if (have_heredoc(str, &delimiter))
// 	{
// 		// printf("%s", delimiter);
// 		launch_heredoc("ENF\n");
// 	}
// }

// int main()
// {
// 	t_list **lst;
// 	char str[100] = "echo    -E 'Hola $USER\nEsto   es un ejemplo' >> miarchivo.txt";
// 	char *saveptr;
// 	char *token;
// 	char	**args_split;

// 	token = ft_strtok(str, " ", &saveptr);
// 	while (token != NULL)
// 	{
// 		printf("%s\n", token);
// 		token = ft_strtok(NULL, "  ", &saveptr);

// 		// args_split = ft_split(str, ' ');

// 	}

// 	return 0;
// }

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


char	**ft_split_m(char const *s, char c)
{
	char	**array;
	int		start;
	int		len;
	int		i;
	int		array_num;

	array_num = -1;
	i = 0;
	array = (char **)malloc((word_count(s, c) + 1) * sizeof(char *));
	if (!s || array == 0)
		return (0);
	while (s[i])
	{
		len = 0;
		i += skip_prefix(&s[i], c);
		start = i;
		len = str_len(s, c, i);
		i += len;
		if (len)
		{
			array[++array_num] = ft_substr(s, start, len);
			if (array_num > 0 && array[array_num][0] == c)
			{
				free(array[array_num]);
				array_num--;
			}
		}
		if (len && !array[array_num])
			return (free_all(array));
	}
	return (array);
}
// {
// 	char	**array;
// 	int		start;
// 	int		len;
// 	int		i;
// 	int		array_num;

// 	array_num = -1;
// 	i = 0;
// 	array = (char **)malloc((word_count(s, c) + 1) * sizeof(char *));
// 	if (!s || array == 0)
// 		return (0);
// 	while (s[i])
// 	{
// 		len = 0;
// 		i += skip_prefix(&s[i], c);
// 		start = i;
// 		len = str_len(s, c, i);
// 		i += len;
// 		if (len)
// 			array[++array_num] = ft_substr(s, start, len);
// 		if (len && !array[array_num])
// 			return (free_all(array));
// 	}
// 	array[++array_num] = NULL;
// 	return (array);
// }


int main()
{
	t_list **lst;
	char str[100] = "echo    -E 'Hola $USER\nEsto     es un ejemplo' >> miarchivo.txt";
	char	**args_split;

	args_split = ft_split_m(str, ' ');

	int i = 0;
	while (args_split[i] != NULL) {
		printf("%s\n", args_split[i]);
		i++;
	}

}

// int main()
// {
// 	char str[100] = "Hello, World! This is a        test.";
// 	const char delim[2] = " ";
// 	char *token;

// 	// get the first token
// 	token = ft_strtok(str, delim);

// 	// walk through other tokens
// 	while (token != NULL)
// 	{
// 		printf("%s\n", token);
// 		token = ft_strtok(NULL, delim);
// 	}

// 	return 0;
// }

