#include <minishell.h>

void	ft_handle_sigint(int signum)
{
	(void)signum;
	if (g_signal == 1)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		write(1, "\033[K\n", 5);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (g_signal == 2)
	{
		write(1, "\033[K\n", 5);
		rl_on_new_line();
		rl_replace_line("", 0);
		g_signal = 4;
	}
	else if (g_signal == 3)
	{
		write(1, "\033[K\n", 5);
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	ft_handle_sigquit(int signum)
{
	(void)signum;
	if (g_signal == 1)
		;
	else if (g_signal == 2)
		;
	else if (g_signal == 3)
	{
		write(1, "Quit (core dumped)\n", sizeof(char) * 19);
		exit(0);
	}
}

void	suppress_output(void)
{
	struct termios	termios_p;

	if (isatty(0))
	{
		if (tcgetattr(0, &termios_p) != 0)
			perror("Minishell: tcgetattr");
		termios_p.c_lflag &= ~ECHOCTL;
		if (tcsetattr(0, 0, &termios_p) != 0)
			perror("Minishell: tcsetattr");
	}
}
