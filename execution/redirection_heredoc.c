/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_heredoc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhamdali <mhamdali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 03:33:12 by mhamdali          #+#    #+#             */
/*   Updated: 2025/07/31 00:09:48 by mhamdali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern int	g_signal[2];

static int	write_expanded_line(char *line, t_env *env, int fd)
{
	t_garbage	*g_c;
	char		*expanded_line;

	g_c = gc_init();
	expanded_line = expander(NULL, line, env, g_c);
	if (!expanded_line)
		return (-1);
	write(fd, expanded_line, ft_strlen(expanded_line));
	write(fd, "\n", 1);
	cleanup_grb_cltr(g_c);
	return (0);
}

int	process_heredoc_line(t_redirect *redirect, t_env *env, char *line, int fd)
{
	if (ft_strcmp(line, redirect->delimiter) == 0)
		return (1);
	if (redirect->heredoc_exp == true)
	{
		if (write_expanded_line(line, env, fd) == -1)
			return (-1);
	}
	else
	{
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
	}
	return (0);
}

int	read_heredoc_input(t_redirect *redirect, t_env *env, int fd)
{
	char	*line;
	int		result;

	while (1)
	{
		line = readline("> ");
		if (!line || g_signal[1])
		{
			if (line)
				free(line);
			return (-1);
		}
		result = process_heredoc_line(redirect, env, line, fd);
		free(line);
		if (result == 1)
			break ;
		if (result == -1)
			return (-1);
	}
	return (0);
}

int	cleanup_and_reopen(char *tmpfile, int fd)
{
	close(fd);
	if (g_signal[1] == 1)
	{
		unlink(tmpfile);
		return (-1);
	}
	fd = open(tmpfile, O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		return (-1);
	}
	unlink(tmpfile);
	return (fd);
}

int	handle_heredoc(t_redirect *redirect, t_env *env)
{
	char	*tmpfile;
	int		fd;

	if (!redirect->delimiter)
		return (-1);
	tmpfile = get_tmp_filename();
	fd = open_heredoc_file(tmpfile);
	if (fd == -1)
		return (-1);
	init_heredoc_signals();
	g_signal[1] = 0;
	if (read_heredoc_input(redirect, env, fd) == -1)
	{
		close(fd);
		init_signals();
		return (-1);
	}
	init_signals();
	return (cleanup_and_reopen(tmpfile, fd));
}
