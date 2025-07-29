/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhamdali <mhamdali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:12:44 by mhamdali          #+#    #+#             */
/*   Updated: 2025/07/29 03:24:17 by mhamdali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_valid_number(char *str)
{
	int	i;
	int	j;

	j = ft_strlen(str) - 1;
	i = 0;
	while (str[i] && str[i] == ' ')
		i++;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[j] == ' ')
		j--;
	while (i <= j)
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static void	handle_exit_message(int flag)
{
	if (!flag)
		write(2, "exit\n", 5);
}

static void	handle_numeric_error(t_command *cmd, t_gc_manager *gc)
{
	print_exit_error(cmd ->args[1]);
	clean_save_in(cmd);
	cleanup_grb_cltr(gc ->cmd_gc);
	cleanup_grb_cltr(gc ->env_gc);
	exit(2);
}

static void	handle_too_many_args(int flag)
{
	handle_exit_message(flag);
	write(2, "minishell: exit: too many arguments\n", 36);
}

void	builtin_exit(t_command *cmd, int flag, t_gc_manager *gc)
{
	int	status;

	status = last_exit_status(0, 0);
	if (!cmd->args[1] || !cmd->args[1][0])
	{
		handle_exit_message(flag);
		clean_save_in(cmd);
		cleanup_grb_cltr(gc ->cmd_gc);
		cleanup_grb_cltr(gc ->env_gc);
		exit(status);
	}
	if (!is_valid_number(cmd->args[1]))
		handle_numeric_error(cmd, gc);
	if (cmd->args[2])
	{
		handle_too_many_args(flag);
		return ;
	}
	status = ft_atoi(cmd->args[1], cmd, gc);
	handle_exit_message(flag);
	clean_save_in(cmd);
	cleanup_grb_cltr(gc ->cmd_gc);
	cleanup_grb_cltr(gc ->env_gc);
	exit((unsigned char)status);
}
