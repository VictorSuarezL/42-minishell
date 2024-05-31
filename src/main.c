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

int	main(void)
{
	t_list	**tokens;
	char	str[100] = "echo         -E 'Hola $USER Esto '   es un ejemplo >> miarchivo.txt";
	// char	str[100] = 'echo "Hola $USER Esto "';
	char	**args_split;
	int		i;
	t_list	*current;

	if (!validator(str))
	{
		ft_error("Wrong input!!!");
		return 0;
	}
	

	tokens = malloc(sizeof(t_list));
	// char str[100] = "hola \"esto es un ejemplo\" con hola mundo 'como estas' hoy?";
	// printf("ends in pipe? = %d\n", is_pipe_end(str));
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
