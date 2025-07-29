/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_heredoc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhamdali <mhamdali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 03:33:12 by mhamdali          #+#    #+#             */
/*   Updated: 2025/07/29 04:11:45 by mhamdali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern int	v_signal;

char	*get_tmp_filename(void)
{
	char	*tmpfile;

	tmpfile = "/tmp/minishell_heredoc.tmp";
	if (access(tmpfile, F_OK) == 0)
		tmpfile = "/tmp/minishell_handle.tmp";
	return (tmpfile);
}

int	open_heredoc_file(char *tmpfile)
{
	int	fd;

	fd = open(tmpfile, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd == -1)
		perror("open");
	return (fd);
}

int	process_heredoc_line(char *line, t_redirect *redirect, t_env *env, int fd)
{
	t_garbage	*g_c = gc_init();
	char	*expanded_line;

	if (ft_strcmp(line, redirect->delimiter) == 0)
		return (1);
	if (redirect->heredoc_exp == true)
	{
		expanded_line = expander(NULL, line, env, g_c);
		if (!expanded_line)
			return (-1);
		write(fd, expanded_line, ft_strlen(expanded_line));
		write(fd, "\n", 1);
		free(expanded_line);
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
		if (!line || v_signal)
		{
			if (line)
				free(line);
			break ;
		}
		result = process_heredoc_line(line, redirect, env, fd);
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
	if (v_signal == 1)
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
	v_signal = 0;
	if (read_heredoc_input(redirect, env, fd) == -1)
	{
		close(fd);
		init_signals();
		return (-1);
	}
	init_signals();
	return (cleanup_and_reopen(tmpfile, fd));
}