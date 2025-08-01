/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhamdali <mhamdali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 18:51:36 by mhamdali          #+#    #+#             */
/*   Updated: 2025/07/31 20:51:51 by mhamdali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	setup_child_pipes(int prev_fd, int pipe_fd[2])
{
	if (prev_fd != -1)
	{
		if (dup2(prev_fd, 0) == -1)
			perror("dup2 stdin");
		close(prev_fd);
	}
	if (pipe_fd)
	{
		close(pipe_fd[0]);
		if (dup2(pipe_fd[1], 1) == -1)
			perror("dup2 stdout");
		close(pipe_fd[1]);
	}
}

static void	handle_pipe_cleanup(int *prev_fd, t_command **cmd, int pipe_fd[2])
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if ((*cmd)->next)
		*prev_fd = pipe_fd[0];
	*cmd = (*cmd)->next;
	if (*cmd && pipe_fd[1])
		close(pipe_fd[1]);
}

void	in_child_process(t_command *cmd, int *pipe_fd, int prev_fd)
{
	int		*pipe_fd_to_pass;

	if (cmd->next)
		pipe_fd_to_pass = pipe_fd;
	else
		pipe_fd_to_pass = NULL;
	setup_child_pipes(prev_fd, pipe_fd_to_pass);
}

int	fork_and_execute(t_command *cmd, pid_t *pids, t_gc_manager *gc)
{
	int		i;
	int		pipe_fd[2];
	int		prev_fd;
	pid_t	child;
	char	*path;

	i = 0;
	prev_fd = -1;
	path = get_env_var(cmd->ori_env, "PATH");
	while (cmd)
	{
		if (cmd->next && pipe(pipe_fd) == -1)
			return (perror("pipe"), kill_all_children(pids, i), -1);
		child = fork();
		if (child == -1)
			return (perror("fork"), kill_all_children(pids, i), -1);
		if (child == 0)
		{
			in_child_process(cmd, pipe_fd, prev_fd);
			execute_child_process(path, cmd, gc);
		}
		pids[i++] = child;
		handle_pipe_cleanup(&prev_fd, &cmd, pipe_fd);
	}
	return (0);
}

void	execution_pipes(t_command *cmd, t_gc_manager *gc)
{
	pid_t	*pids;
	int		n_cmd;

	n_cmd = count_cmd(cmd);
	if (n_cmd <= 0)
		return ;
	pids = malloc(n_cmd * sizeof(pid_t));
	if (!pids)
		return ;
	if (handle_all_heredocs(cmd) == -1)
	{
		dup2(cmd->save_out, 1);
		dup2(cmd->save_in, 0);
		return ;
	}
	if (fork_and_execute(cmd, pids, gc) == -1)
	{
		kill_all_children(pids, n_cmd);
		free(pids);
		return ;
	}
	wait_for_children(pids, n_cmd);
	free(pids);
}
