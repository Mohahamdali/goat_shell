/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_two.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhamdali <mhamdali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 13:20:09 by mhamdali          #+#    #+#             */
/*   Updated: 2025/07/29 03:18:50 by mhamdali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	env_built(t_command *cmd)
{
	t_env	*tmp;

	if (!cmd->env)
		return (0);
	if (cmd->args[1])
	{
		if (cmd->args[1][0] == '-')
			return (0);
		printf("env: %s: No such file or directory\n", cmd->args[1]);
		return (1);
	}
	tmp = cmd->env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, "OLDPWD") != 0)
			printf("%s=%s\n", tmp->key, tmp->content);
		tmp = tmp->next;
	}
	return (0);
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
		if (ft_strncmp(current->key, key, ft_strlen(key)) == 0
			&& ft_strlen(current->key) == ft_strlen(key))
		{
			if (!prev_node)
				*env = next;
			else
				prev_node->next = next;
			free(current->key);
			free(current->content);
			free(current);
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
