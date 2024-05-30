#include <fcntl.h>
#include <minishell.h>
#include <stdio.h>
#include <string.h>



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
