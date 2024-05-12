#include <minishell.h>

#include <stdio.h>
#include <string.h>
#include <fcntl.h>

int main()
{
	char	str[100] = "cat << ENF      ";
	char	*delimiter = NULL;
	if (have_heredoc(str, &delimiter))
	{
		// printf("%s", delimiter);
		launch_heredoc("ENF\n");
	}
}

