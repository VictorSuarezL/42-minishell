#include <minishell.h>

int	pwd_builtin(char *input)
{
	char	cwd[4096];

	if ((input[3] != ' ' && input[3] != '\0'))
	{
		ft_printf("%s : Command not found\n", input);
		return (1);
	}
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		return (ft_printf("%s\n", cwd), 0);
	else
		return (perror("getcwd() error"), 1);
}
