/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sub_command_path.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhamdali <mhamdali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 08:13:39 by mhamdali          #+#    #+#             */
/*   Updated: 2025/07/29 08:23:06 by mhamdali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_dire(char *path)
{
	struct stat	data;

	if (stat(path, &data) == 0)
		return (S_ISDIR(data.st_mode));
	return (0);
}

int	path_exists(const char *path)
{
	struct stat	st;

	return (stat(path, &st) == 0);
}

int	is_executable(const char *path)
{
	return (access(path, X_OK) == 0);
}
