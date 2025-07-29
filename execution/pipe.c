/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhamdali <mhamdali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 18:51:36 by mhamdali          #+#    #+#             */
/*   Updated: 2025/07/29 07:11:17 by mhamdali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_cmd(t_command *cmd)
{
	int	i;

	i = 0;
	while (cmd)
	{
		i++;
		cmd = cmd->next;
	}
	return (i);
}

void	setup_child_pipes(int **pipe_fd, int i, int n_cmd)
{
	int	j;

	if (i > 0)
	{
		if (dup2(pipe_fd[i - 1][0], 0) == -1)
			perror("dup2 stdin");
		close(pipe_fd[i - 1][0]);
		pipe_fd[i - 1][0] = -1;
	}
	if (i < n_cmd - 1)
	{
		if (dup2(pipe_fd[i][1], 1) == -1)
			perror("dup2 stdout");
		close(pipe_fd[i][1]);
		pipe_fd[i][1] = -1;
	}
	j = 0;
	while (j < n_cmd - 1)
	{
		if (pipe_fd[j][0] != -1)
			close(pipe_fd[j][0]);
		if (pipe_fd[j][1] != -1)
			close(pipe_fd[j][1]);
		j++;
	}
}

void	execute_child_process(char *path, t_command *cmd, \
    int **pipe_fd, t_gc_manager *gc)
{
    int check;

	cmd->ori_env = convert_linked_to_char(cmd->env, gc->cmd_gc);
	check = apply_redirections(cmd);
	if (is_builtins(cmd) && check != -1)
		execution_builtins(cmd, &cmd->env, 1, gc);
	else if (check != -1)
		external_command_pipe(path, cmd, gc);
	clean_save_in(cmd);
	exit(0);
}

int	fork_and_execute(t_command *cmd, int **pipe_fd, \
    pid_t *pids, t_gc_manager *gc)
{
	int			i;
	pid_t		child;
	char		*path;
	int			n_cmd;

	i = 0;
	n_cmd = count_cmd(cmd);
	path = get_env_var(cmd->ori_env, "PATH");
	while (cmd)
	{
		child = fork();
		if (child == -1)
			return (perror("fork"), kill_all_children(pids, i), -1);
		if (child == 0)
		{
			setup_child_pipes(pipe_fd, i, n_cmd);
			execute_child_process(path, cmd, pipe_fd, gc);
		}
		else
			pids[i] = child;
		i++;
		cmd = cmd->next;
	}
	return (0);
}

void	execution_pipes(t_command *cmd, t_gc_manager *gc)
{
	int		n_cmd;
	int		**pipe_fd;
	pid_t	*pids;

	n_cmd = count_cmd(cmd);
	if (n_cmd - 1 < 0)
		return ;
	pipe_fd = create_pipes(n_cmd);
	pids = malloc(n_cmd * sizeof(pid_t));
	if (!pipe_fd || !pids)
	{
		cleanup_resources(pipe_fd, pids, n_cmd);
		return ;
	}
	handle_all_heredocs(cmd);
	if (fork_and_execute(cmd, pipe_fd, pids, gc) == -1)
	{
		cleanup_resources(pipe_fd, pids, n_cmd);
		return ;
	}
	close_parent_pipes(pipe_fd, n_cmd);
	free_pipes(pipe_fd, n_cmd - 1);
	wait_for_children(pids, n_cmd);
	free(pids);
}
