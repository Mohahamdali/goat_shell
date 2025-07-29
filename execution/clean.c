/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhamdali <mhamdali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 12:23:52 by mhamdali          #+#    #+#             */
/*   Updated: 2025/07/29 08:07:47 by mhamdali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	last_exit_status(int status, int set)
{
	static int	last_status = 0;

	if (set)
		last_status = status;
	return (last_status);
}

void	clean_save_in(t_command *cmd)
{
	if (cmd->save_in >= 4)
	{
		close(cmd->save_in);
		cmd->save_in = -1;
	}
	if (cmd->save_out >= 3)
	{
		close(cmd->save_out);
		cmd->save_out = -1;
	}
}

void	close_pipes(int **pipe_fd, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		close(pipe_fd[i][0]);
		close(pipe_fd[i][1]);
		i++;
	}
}

void	free_pipes(int **pipe_fd, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(pipe_fd[i]);
		i++;
	}
	free(pipe_fd);
}

void	cleanup_resources(int **pipe_fd, pid_t *pids, int n_cmd)
{
	if (pipe_fd)
	{
		close_pipes(pipe_fd, n_cmd - 1);
		free_pipes(pipe_fd, n_cmd - 1);
	}
	if (pids)
		free(pids);
}
