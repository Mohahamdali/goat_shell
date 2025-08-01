/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhamdali <mhamdali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 03:33:18 by mhamdali          #+#    #+#             */
/*   Updated: 2025/07/31 20:51:58 by mhamdali         ###   ########.fr       */
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

void	execute_child_process(char *path, t_command *cmd, t_gc_manager *gc)
{
	int	check;

	cmd->ori_env = convert_linked_to_char(cmd->env, gc->cmd_gc);
	check = apply_redirections(cmd);
	if (is_builtins(cmd) && check != -1)
		execution_builtins(cmd, &cmd->env, 1, gc);
	else if (check != -1)
		external_command_pipe(path, cmd, gc);
	clean_save_in(cmd);
	cleanup_grb_cltr(gc ->cmd_gc);
	cleanup_grb_cltr(gc ->env_gc);
	exit(0);
}

void	external_command_pipe(char *path, t_command *command, t_gc_manager *gc)
{
	if (command->args_fo_exp && \
		(!command->args || !command->args[0] || command->args[0][0] == '\0') &&
		command->args_fo_exp[0] == true)
	{
		return ;
	}
	if (ft_strchr(command->args[0], '/'))
		execute_path_command(command->args[0], command, gc);
	if (!path)
	{
		handle_local_path(command, gc);
	}
	search_in_path(path, command, gc);
	write(2, command->args[0], ft_strlen(command->args[0]));
	write(2, ": command not found", 20);
	write(2, "\n", 1);
	clean_save_in(command);
	cleanup_grb_cltr(gc->cmd_gc);
	cleanup_grb_cltr(gc->env_gc);
	exit(127);
}
