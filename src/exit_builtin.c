#include <minishell.h>

void	exit_stat(char *buf, int *exit_code, char ***copy_en, char ***copy_exp)
{
	int	should_exit;

	should_exit = 0;
	if (ft_strncmp(buf, "exit", 4) == 0 && (buf[4] == ' ' || buf[4] == '\0'))
	{
		*exit_code = exit_builtin(buf, &should_exit);
		if (should_exit)
		{
			final_clean(*copy_en, *copy_exp);
			exit(*exit_code);
		}
	}
}

void	init_args(char *buf, char **arg1, char **arg2, char **command)
{
	*command = ft_strtok(buf, " ");
	*arg1 = ft_strtok(NULL, " ");
	*arg2 = ft_strtok(NULL, " ");
}

int	exit_builtin(char *input, int *should_exit)
{
	char	buf[4096];
	char	*command;
	char	*arg1;
	char	*arg2;

	ft_strncpy(buf, input, sizeof(buf) - 1);
	buf[sizeof(buf) - 1] = '\0';
	init_args(buf, &arg1, &arg2, &command);
	*should_exit = 0;
	if (arg1 == NULL)
	{
		ft_printf("exit\n");
		*should_exit = 1;
		return (0);
	}
	else if (arg2 == NULL)
	{
		ft_printf("exit\n");
		*should_exit = 1;
		return (ft_atoi(arg1));
	}
	else
		return (1);
	return (1);
}
