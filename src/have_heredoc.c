//#include <minishell.h>
#include <stdio.h>

int	have_heredoc(char *str, int *pos)
{
	int	i;

	i = 0;
	while (str[i] && str[i + 1])
	{
		if ((str[i] == '<') && (str[i + 1] == '<'))
			*pos = i;
		i++;
	}
	if (*pos)
		return (1);
	return (0);
}

//int main()
//{
//	char str[100] = "grep -i `patron` <<  END       << END";
//	int pos = 0;

//	printf("%d\n", have_heredoc(str, &pos));
//	printf("%d\n", pos);
//}