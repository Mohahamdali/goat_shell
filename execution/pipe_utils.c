/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhamdali <mhamdali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 03:33:22 by mhamdali          #+#    #+#             */
/*   Updated: 2025/07/31 01:28:00 by mhamdali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	wait_for_children(pid_t *pids, int n_cmd)
{
	int	i;
	int	status;

	i = 0;
	while (i < n_cmd - 1)
	{
		waitpid(pids[i], NULL, 0);
		i++;
	}
	waitpid(pids[n_cmd - 1], &status, 0);
	if (WIFEXITED(status))
		last_exit_status(WEXITSTATUS(status), 1);
	else if (WIFSIGNALED(status))
		last_exit_status(128 + WTERMSIG(status), 1);
	else
		last_exit_status(1, 1);
}

void	kill_all_children(pid_t *pids, int count)
{
	int	i;

	i = 0;
	if (!pids)
		return ;
	while (i < count)
	{
		if (pids[i] > 0)
			kill(pids[i], SIGKILL);
		i++;
	}
	i = 0;
	while (i < count)
	{
		if (pids[i] > 0)
		{
			while (waitpid(pids[i], NULL, 0) == -1)
			{
				if (errno != EINTR)
					break ;
			}
		}
		i++;
	}
}
