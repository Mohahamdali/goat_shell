/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhamdali <mhamdali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 17:26:45 by mhamdali          #+#    #+#             */
/*   Updated: 2025/07/29 03:23:12 by mhamdali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	cd_error_write(char *target)
{
	write(2, "minishell: cd: ", 15);
	write(2, target, ft_strlen(target));
	write(2, ": ", 2);
	write(2, strerror(errno), ft_strlen(strerror(errno)));
	write(2, "\n", 1);
}

void	print_exit_error(char *arg)
{
	write(2, "minishell: exit: ", 18);
	write(2, arg, ft_strlen(arg));
	write(2, ": numeric argument required\n", 29);
}

void	print_numeric_error(char *arg)
{
	write(2, "minishell: exit: ", ft_strlen("minishell: "));
	write(2, arg, ft_strlen(arg));
	write(2, ": numeric argument required\n", \
		ft_strlen(": numeric argument required\n"));
}
