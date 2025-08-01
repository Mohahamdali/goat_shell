/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhamdali <mhamdali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 22:36:25 by mhamdali          #+#    #+#             */
/*   Updated: 2025/07/30 22:36:54 by mhamdali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	save_output_redi(t_redirect *redirect)
{
	int	fd;

	fd = open(redirect->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		write(2, "minishell: ", 11);
		write(2, redirect->filename, ft_strlen(redirect->filename));
		write(2, ": permission denied\n", 21);
		last_exit_status(1, 1);
		return (-1);
	}
	if (dup2(fd, 1) == -1)
	{
		perror("Error redirecting stdout");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	handle_input_redi(t_redirect *redirect)
{
	int	fd;
	int	save_fd_0;

	fd = open(redirect->filename, O_RDONLY);
	if (fd == -1)
	{
		write(2, "minishell: ", 11);
		write(2, redirect->filename, ft_strlen(redirect->filename));
		write(2, ": No such file or directory\n", 29);
		last_exit_status(1, 1);
		return (-1);
	}
	save_fd_0 = dup(0);
	if (dup2(fd, 0) == -1)
	{
		perror("Error redirecting stdinput");
		close(fd);
		return (-1);
	}
	close(fd);
	return (save_fd_0);
}

int	handle_output_redi(t_redirect *redirect)
{
	int	fd;

	fd = open(redirect->filename, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd == -1)
	{
		write(2, "minishell: ", 11);
		write(2, redirect->filename, ft_strlen(redirect->filename));
		write(2, ": permission denied\n", 21);
		last_exit_status(1, 1);
		return (-1);
	}
	if (dup2(fd, 1) == -1)
	{
		perror("Error redirecting stdout");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}
