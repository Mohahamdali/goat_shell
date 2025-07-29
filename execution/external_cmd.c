/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhamdali <mhamdali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 23:35:10 by mhamdali          #+#    #+#             */
/*   Updated: 2025/07/29 08:11:29 by mhamdali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_local_path(t_command *command, t_gc_manager *gc)
{
	char	*local_path;

	local_path = ft_strjoin("/", command->args[0], gc -> cmd_gc);
	if (!local_path)
	{
		perror("malloc");
		return ;
	}
	if (access(local_path, F_OK) == 0)
		execute_path_command(local_path, command, gc);
	else
	{
		write(2, "minishell: ", 11);
		write(2, command->args[0], ft_strlen(command->args[0]));
		write(2, ": No such file or directory\n", 29);
		clean_save_in(command);
		cleanup_grb_cltr(gc ->cmd_gc);
		cleanup_grb_cltr(gc ->env_gc);
		exit(127);
	}
}

void	search_in_path(char *path, t_command *command, t_gc_manager *gc)
{
	char	**src;
	char	*fullpath;
	char	*test;
	int		i;

	src = ft_split(path, ':', gc ->cmd_gc);
	i = 0;
	while (src && src[i])
	{
		fullpath = ft_strjoin(src[i], "/", gc->cmd_gc);
		test = ft_strjoin(fullpath, command->args[0], gc->cmd_gc);
		if (access(test, F_OK) == 0)
			execute_path_command(test, command, gc);
		i++;
	}
}

static void	handle_child_process(char *path, t_command *command,
		t_gc_manager *gc)
{
	if (ft_strchr(command->args[0], '/'))
		execute_path_command(command->args[0], command, gc);
	path = get_env_var(command->ori_env, "PATH");
	if (!path)
	{
		handle_local_path(command, gc);
		return ;
	}
	search_in_path(path, command, gc);
	write(2, command->args[0], ft_strlen(command->args[0]));
	write(2, ": command not found", 20);
	write(2, "\n", 1);
	clean_save_in(command);
	cleanup_grb_cltr(gc ->cmd_gc);
	cleanup_grb_cltr(gc ->env_gc);
	exit(127);
}

static void	handle_parent_process(pid_t child)
{
	int	status;

	waitpid(child, &status, 0);
	if (WIFEXITED(status))
		last_exit_status(WEXITSTATUS(status), 1);
	else if (WIFSIGNALED(status))
		last_exit_status(128 + WTERMSIG(status), 1);
	else
		last_exit_status(255, 1);
}

void	external_command(char *path, t_command *command, t_gc_manager *gc)
{
	pid_t	child;

	if (command->args_fo_exp && (!command->args || !command->args[0]
			|| command->args[0][0] == '\0')
		&& command->args_fo_exp[0] == true)
		return ;
	child = fork();
	if (child < 0)
		return ;
	if (child == 0)
		handle_child_process(path, command, gc);
	else
		handle_parent_process(child);
}
