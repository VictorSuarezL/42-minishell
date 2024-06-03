#include <minishell.h>

int	pwd_builtin(void)
{
	char	cwd[4096];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		return (ft_printf("%s\n", cwd), 0);
	else
		return (perror("getcwd() error"), 1);
}
