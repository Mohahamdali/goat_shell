/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_cmd_u2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhamdali <mhamdali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 18:36:23 by mhamdali          #+#    #+#             */
/*   Updated: 2025/07/29 08:13:23 by mhamdali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	handle_file_not_found(char *path, t_command *cmd, t_gc_manager *gc)
{
	if (errno == ENOTDIR)
	{
		write(2, "minishell: ", 11);
		write(2, path, ft_strlen(path));
		write(2, ": Not a directory\n", 19);
		clean_save_in(cmd);
		cleanup_grb_cltr(gc->cmd_gc);
		cleanup_grb_cltr(gc->env_gc);
		exit(126);
	}
	else if (errno == ENOENT)
	{
		write(2, "minishell: ", 11);
		write(2, path, ft_strlen(path));
		write(2, ": No such file or directory\n", 29);
		clean_save_in(cmd);
		cleanup_grb_cltr(gc->cmd_gc);
		cleanup_grb_cltr(gc->env_gc);
		exit(127);
	}
}

static void	check_file_access(char *path, t_command *cmd, t_gc_manager *gc)
{
	if (access(path, F_OK) != 0)
		handle_file_not_found(path, cmd, gc);
}

static void	check_directory_and_permissions(char *path, t_command *cmd,
										t_gc_manager *gc)
{
	if (is_dire(path))
	{
		write(2, "minishell: ", 11);
		write(2, path, ft_strlen(path));
		write(2, ": is a directory\n", 17);
		clean_save_in(cmd);
		cleanup_grb_cltr(gc->cmd_gc);
		cleanup_grb_cltr(gc->env_gc);
		exit(126);
	}
	if (!is_executable(path))
	{
		write(2, "minishell: ", 11);
		write(2, path, ft_strlen(path));
		write(2, ": Permission denied\n", 20);
		clean_save_in(cmd);
		cleanup_grb_cltr(gc->cmd_gc);
		cleanup_grb_cltr(gc->env_gc);
		exit(126);
	}
}

void	execute_path_command(char *path, t_command *cmd, t_gc_manager *gc)
{
	check_file_access(path, cmd, gc);
	check_directory_and_permissions(path, cmd, gc);
	clean_save_in(cmd);
	execve(path, cmd->args, cmd->ori_env);
	perror("execve failed");
	clean_save_in(cmd);
	cleanup_grb_cltr(gc->cmd_gc);
	cleanup_grb_cltr(gc->env_gc);
	exit(1);
}
