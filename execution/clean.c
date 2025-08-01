/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhamdali <mhamdali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 12:23:52 by mhamdali          #+#    #+#             */
/*   Updated: 2025/08/01 19:51:29 by mhamdali         ###   ########.fr       */
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

