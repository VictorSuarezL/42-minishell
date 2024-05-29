#include <fcntl.h>
#include <minishell.h>
#include <stdio.h>
#include <string.h>

int	is_pipe_end(char *str)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(str);
	if (str[len - 1] == '|')
	{
		return (1);
	}
	else
	{
		return (0);
	}
}

int	quote_counter(char *str)
{
	int	i;
	int flag;
	int	flag_a;
	int	flag_b;

	i = 0;
	flag = 0;
	flag_a = 0;
	flag_b = 0;
	while (str[i])
	{
		if (str[i] == '\'' && flag == 0)	
		{
			flag = 1;
		}
		else if (str[i] == '\'' && flag == 1)
		{
			flag = 0;
		}
		if (str[i] == '\"' && flag == 0)	
		{
			flag = 1;
		}
		else if (str[i] == '\"' && flag == 1)
		{
			flag = 0;
		}
		i++;
	}
	if (flag_a || flag_b)
	{
		flag = 1;
	}
	
	return (flag);
}

int	is_metacharacter_end(char *str)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(str);
	if (str[len - 1] == '>' || str[len - 1] == '<')
	{
		return (1);
	}
	else
	{
		return (0);
	}
}

int	validator(char *str)
{
	if (quote_counter(str))
	{
		return (0);
	}
	else if (is_metacharacter_end(str))
	{
		return (0);
	}
	else if (is_pipe_end(str))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int	main(void)
{
	t_list	**lst;
	char	str[100] = "echo -E 'Hola $USER Esto '   es un ejemplo >> miarchivo.txt |";
	char	**args_split;
	int		i;
	t_list	*current;

	if (!validator(str))
	{
		ft_error("Wrong input!!!");
		return 0;
	}
	

	lst = malloc(sizeof(t_list));
	// char str[100] = "hola \"esto es un ejemplo\" con hola mundo 'como estas' hoy?";
	printf("ends in pipe? = %d\n", is_pipe_end(str));
	args_split = ft_split_m(str);
	i = 0;
	while (args_split[i] != NULL)
	{
		printf("%s\n", args_split[i]);
		i++;
	}
	i = 0;
	while (args_split[i] != NULL)
	{
		ft_lstadd_back(lst, ft_lstnew((void *)args_split[i]));
		i++;
	}
	current = *lst;
	while (current != NULL)
	{
		printf("%s\n", (char *)current->content);
		current = current->next;
	}
}
