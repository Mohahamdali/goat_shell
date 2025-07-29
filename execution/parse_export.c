/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhamdali <mhamdali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 19:28:29 by mhamdali          #+#    #+#             */
/*   Updated: 2025/07/29 08:24:58 by mhamdali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	parse_export(char *arg)
{
	int	i;

	i = 1;
	if (!arg)
		return (1);
	if (!ft_isalpha(arg[0]) && arg[0] != '_')
		return (1);
	while (arg[i] && arg[i] != '=')
	{
		if (arg[i] == '+')
		{
			if (arg[i + 1] == '=')
				return (0);
			else
				return (1);
		}
		if (!ft_isalnum(arg[i]))
			return (1);
		i++;
	}
	return (0);
}

static void	swap_env_nodes(t_env *current)
{
	char	*temp_key;
	char	*temp_content;

	temp_key = current->key;
	current->key = current->next->key;
	current->next->key = temp_key;
	temp_content = current->content;
	current->content = current->next->content;
	current->next->content = temp_content;
}

void	sort_export(t_env *env)
{
	t_env	*current;
	int		swapped;

	if (!env)
		return ;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		current = env;
		while (current->next)
		{
			if (ft_strcmp(current->key, current->next->key) > 0)
			{
				swap_env_nodes(current);
				swapped = 1;
			}
			current = current->next;
		}
	}
}
