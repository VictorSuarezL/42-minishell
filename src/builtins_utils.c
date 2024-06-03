#include <minishell.h>

// Function to skip spaces in the input string
int	skip_spaces(char *input, int i)
{
	while (input[i] == ' ')
		i++;
	return (i);
}

void	ft_swap(char **a, char **b)
{
	char	*temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

void	sort_strings(char **str)
{
	int	i;
	int	j;

	i = 0;
	while (str[i])
	{
		j = 0;
		while (str[j + 1])
		{
			if (ft_strcmp(str[j], str[j + 1]) > 0)
				ft_swap(&str[j], &str[j + 1]);
			j++;
		}
		i++;
	}
}

int	empty_string(char *str)
{
	return (str == NULL || *str == '\0');
}

void	print_str(char **str_array)
{
	if (str_array == NULL)
		return ;
	while (*str_array)
	{
		ft_printf("declare -x ");
		ft_printf("%s\n", *str_array);
		str_array++;
	}
}