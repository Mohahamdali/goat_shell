/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhamdali <mhamdali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 16:50:23 by mhamdali          #+#    #+#             */
/*   Updated: 2025/07/29 08:10:26 by mhamdali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_env(t_env *env)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (!current->content)
			printf("declare -x %s\n", current->key);
		else if (ft_strncmp(current->key, "_", ft_strlen("_")) == 0 \
			&& ft_strlen (current -> key) == ft_strlen("_"))
		{
			current = current->next;
			continue ;
		}
		else
			printf("declare -x %s=\"%s\"\n", current->key, current->content);
		current = current->next;
	}
}

int	update_env_content(t_env *env, char *content, int append, t_garbage *gc)
{
	char	*new_content;

	if (content == NULL)
		return (1);
	if (append && env->content)
	{
		new_content = ft_strjoin(env->content, content, gc);
		if (!new_content)
			return (1);
		env->content = new_content;
	}
	else
	{
		env->content = g_strdup(gc, content);
		if (!env->content)
			return (1);
	}
	return (1);
}

int	change_env_already(t_env *env, char *key, char *content, t_garbage *gc)
{
	while (env)
	{
		if (ft_strlen(env->key) == ft_strlen(key) && \
			ft_strncmp(env->key, key, ft_strlen(key)) == 0)
		{
			return (update_env_content(env, content, gc ->append, gc));
		}
		env = env->next;
	}
	return (0);
}
