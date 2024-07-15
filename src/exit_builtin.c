#include <minishell.h>

void	init_args(char *buf, char **arg1, char **arg2, char **command)
{
	*command = ft_strtok(buf, " ");
	*arg1 = ft_strtok(NULL, " ");
	*arg2 = ft_strtok(NULL, " ");
}

int	is_digit_str(char *str)
{
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

int	handle_exit_no_args(int *should_exit)
{
	ft_printf("exit\n");
	*should_exit = 1;
	return (0);
}

int	handle_exit_with_invalid_arg(char *arg1)
{
	ft_printf("exit\n");
	ft_printf("exit: %s: numeric argument required\n", arg1);
	return (2);
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
		return (handle_exit_no_args(should_exit));
	else if (arg2 == NULL)
	{
		if (!is_digit_str(arg1))
			return (handle_exit_with_invalid_arg(arg1));
		ft_printf("exit\n");
		*should_exit = 1;
		return (ft_atoi(arg1));
	}
	else
	{
		return (1);
	}
	return (1);
}
