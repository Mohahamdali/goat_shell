/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhamdali <mhamdali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:12:44 by mhamdali          #+#    #+#             */
/*   Updated: 2025/07/13 16:04:46 by mhamdali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	va_lid_n(char *str)
{
	int	i;

	i = 0;
	if (str[i] != '-' || str[1] != 'n')
		return (0);
	i = 2;
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

static int	write_args(t_command *cmd, int start_index)
{
	int	i;

	i = start_index;
	while (cmd->args[i])
	{
		if (write(1, cmd->args[i], ft_strlen(cmd->args[i])) == -1)
			return (1);
		if (cmd->args[i + 1])
		{
			if (write(1, " ", 1) == -1)
				return (1);
		}
		i++;
	}
	return (0);
}

int	buittin_echo(t_command *cmd)
{
	int	i;
	int	flag;

	i = 1;
	flag = 0;
	while (cmd->args[i] && va_lid_n(cmd->args[i]))
	{
		flag = 1;
		i++;
	}
	if (write_args(cmd, i) == 1)
		return (1);
	if (!flag)
	{
		if (write(1, "\n", 1) == -1)
			return (1);
	}
	return (0);
}
