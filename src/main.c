#include <fcntl.h>
#include <minishell.h>
#include <stdio.h>
#include <string.h>

# define EMPTY 0
# define CMD 1
# define ARG 2
# define TRUNC 3
# define APPEND 4
# define INPUT 5
# define PIPE 6
# define END 7

int in_quote(char *str, int i, char c)
{
	int j = 0;
	int flag = 0;
	while (str[j] && j < i)
	{
		if (str[j] == c && flag == 0)
		{
			flag = 1;
		}
		else if (str[j] == c && flag == 1)
		{
			flag = 0;
		}
		j++;
	}
	return flag;
}

int	ignore_sep(char *line, int i)
{
	if (line[i] && line[i] == '\\' && line[i + 1] && line[i + 1] == ';')
		return (1);
	else if (line[i] && line[i] == '\\' && line[i + 1] && line[i + 1] == '|')
		return (1);
	else if (line[i] && line[i] == '\\' && line[i + 1] && line[i + 1] == '>')
		return (1);
	else if (line[i] && line[i] == '\\' && line[i + 1] && line[i + 1] == '>'
				&& line[i + 2] && line[i + 2] == '>')
		return (1);
	return (0);
}

int syntax_checker(char *str)
{
	int i = 0;

	while (str[i])
	{
		if (in_quote(str, i, '"') || in_quote(str, i, '\''))
		{
			i++;
		}
		else if (ignore_sep(str, i))
		{
			i++;
		}
		else if (str[i] == ';')
		{
			return 0;
		}
		// else if (ft_strchr("<>|;", str[i]) && str[i+1] && ft_strchr("<>|;", str[i+1]))
		// {
		// 	if (str[i] != str[i + 1])
		// 	{
		// 		return 0;
		// 	}
		// }
		i++;
	}	
	return 1;
}

int	main(void)
{
	t_list	**tokens;
	char	str[100] = "echo -E 'Hola $USER Esto ' es un ejemplo  miarchivo.txt";
	char	**args_split;
	int		i;
	t_list	*current;

	if (!validator(str))
	{
		ft_error("Wrong input!!!");
		return 0;
	}
	if (!syntax_checker(str))
	{
		ft_error("Syntax checker!");
		return 0;
	}
	
	

	tokens = malloc(sizeof(t_list));
	// char str[100] = "hola \"esto es un ejemplo\" con hola mundo 'como estas' hoy?";
	// printf("ends in pipe? = %d\n", is_pipe_end(str));
	args_split = ft_split_m(str);
	// i = 0;
	// while (args_split[i] != NULL)
	// {
	// 	printf("%s\n", args_split[i]);
	// 	i++;
	// }
	i = 0;
	while (args_split[i] != NULL)
	{
		ft_lstadd_back(tokens, ft_lstnew((void *)args_split[i]));
		i++;
	}
	current = *tokens;
	while (current != NULL)
	{
		printf("%s\n", (char *)current->content);
		current = current->next;
	}
}
