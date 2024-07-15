#include <minishell.h>

int	g_signal;

int	main(int args, char **argv, char **env)
{
	char	**copy_export;
	char	**copy_en;
	char	*input;
	int		exit_status;

	exit_status = 0;
	(void)args;
	argv++;
	setup_shell(&copy_export, &copy_en, env);
	while (1)
	{
		g_signal = 1;
		input = display_prompt();
		if (input == NULL)
		{
			ft_printf("exit\n");
			break ;
		}
		process_input(input, &copy_en, &copy_export, &exit_status);
	}
	final_clean(copy_export, copy_en);
	return (exit_status);
}
