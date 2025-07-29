/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_two.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhamdali <mhamdali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 13:20:09 by mhamdali          #+#    #+#             */
/*   Updated: 2025/07/13 15:14:06 by mhamdali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	keys_match(char *key1, char *key2)
{
	if (ft_strncmp(key1, key2, ft_strlen(key2)) == 0
		&& ft_strlen(key1) == ft_strlen(key2))
		return (1);
	return (0);
}

static void	free_env_node(t_env *node)
{
	free(node->key);
	free(node->content);
	free(node);
}

void	unset(t_env **env, char *key)
{
	t_env	*current;
	t_env	*prev_node;
	t_env	*next;

	current = *env;
	prev_node = NULL;
	while (current)
	{
		next = current->next;
		if (keys_match(current->key, key))
		{
			if (!prev_node)
				*env = next;
			else
				prev_node->next = next;
			free_env_node(current);
		}
		else
			prev_node = current;
		current = next;
	}
}

void	unset_all_env(t_env **env, char **keys)
{
	int	i;

	if (!env || !keys)
	{
		last_exit_status(1, 1);
		return ;
	}
	i = 0;
	while (keys[i])
	{
		unset(env, keys[i]);
		i++;
	}
	last_exit_status(0, 1);
}
