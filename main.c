#include "minishell.h"

extern int	g_signal[2];

int	main_loop(const char *prompt, t_env *shared_env, char **env, int flag)
{
	char		*line;
	t_command	*cmd_list;
	t_gc_manager gc;
	gc.env_gc = gc_init();
	if (!gc.env_gc)
		return (1);
	while (1)
	{
		gc.cmd_gc = gc_init();
		if (!gc.cmd_gc || !gc.env_gc)
			return (1);
		g_signal[0] = 0;
		line = readline(prompt);
		if (!line)
			return (cleanup_grb_cltr(gc.cmd_gc), printf("exit\n"), 1);
		if (*line)
			add_history(line);
		cmd_list = parser(shared_env, line, env, gc.cmd_gc);
		free(line);
		if (!cmd_list)
		{
			cleanup_grb_cltr(gc.cmd_gc);
			continue ;
		}
		(void)flag;
		execution(cmd_list, &shared_env, flag,&gc);
		cleanup_grb_cltr(gc.cmd_gc);
	}
	cleanup_grb_cltr(gc.env_gc);
	return (0);
}

int	main(int ac, char **av, char**env)
{
	t_env		*shared_env;
	int			flag;
	t_garbage	*g_c;
	const char	*prompt;

	flag = 0;
	prompt = "<-MINI-SHELL-> ";
	(void)ac;
	(void)av;
	g_c = gc_init();
	if (!g_c)
		return (1);
	init_signals();
	shared_env = set_env(env, g_c);
	if (!shared_env)
		return (cleanup_grb_cltr(g_c), 1);
	if (main_loop(prompt, shared_env, env, flag))
	{
		//  rl_clear_history();
		cleanup_grb_cltr(g_c);
		return (1);
	}
	cleanup_grb_cltr(g_c);
	return (last_exit_status(0, 0));
}



