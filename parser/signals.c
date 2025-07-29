#include "../minishell.h"

int	v_signal[2];

void	sig_int(int sig)
{
	(void)sig;
	v_signal[0] = 1;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}
void	heredoc_sig_int(int sig)
{
	(void)sig;
	v_signal[1] = 1;
	write(1, "\n", 1);
	close(0);
}
void	init_signals(void)
{
	v_signal[0] = 0;
	signal(SIGINT, sig_int);
	signal(SIGQUIT, SIG_IGN);
}
void	init_heredoc_signals(void)
{
	v_signal[1] = 0;
	signal(SIGINT, heredoc_sig_int);
	signal(SIGQUIT, SIG_IGN);
}
