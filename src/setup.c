#include <minishell.h>

void	final_clean(char **exp, char **env)
{
	eliminarArchivos();
	free_double(exp);
	free_double(env);
}

void	setup_executor(char *buf, char **env, char **export)
{
	g_signal = 3;
	signal(SIGQUIT, ft_handle_sigquit);
	signal(SIGINT, ft_handle_sigint);
	if (save_fork() == 0)    
		runcmd(parse_cmd(buf), env, export);
    wait(0);
	eliminarArchivos();
}

void	setup_shell(char ***copy_exp, char ***copy_en, char **env)
{
	*copy_exp = copy_env(env);
	*copy_en = copy_env(env);
	g_signal = 1;
	signal(SIGINT, ft_handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	suppress_output();
}