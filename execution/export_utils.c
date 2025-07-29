/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhamdali <mhamdali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 16:35:52 by mhamdali          #+#    #+#             */
/*   Updated: 2025/07/29 03:25:01 by mhamdali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_append_operation(char *arg)
{
	int	i;

	i = 0;
	while (arg[i] && arg[i] != '=')
		i++;
	return (i > 0 && arg[i - 1] == '+');
}

t_env	*create_env_node(char *key, char *content, t_garbage *gc)
{
	t_env	*new_node;

	new_node = g_malloc(gc, sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = g_strdup(gc, key);
	if (!new_node->key)
		return (NULL);
	if (content != NULL)
	{
		new_node->content = g_strdup(gc, content);
		if (!new_node->content)
		{
			return (NULL);
		}
	}
	else
		new_node->content = NULL;
	new_node->next = NULL;
	return (new_node);
}

char	*determine_key(char *argv, t_garbage *gc)
{
	int		i;
	int		j;
	char	*key;

	i = 0;
	j = 0;
	while (argv[i] && argv[i] != '=')
		i++;
	if (i > 0 && argv[i - 1] == '+')
		i--;
	key = g_malloc(gc, i + 1);
	if (!key)
		return (NULL);
	while (j < i)
	{
		key[j] = argv[j];
		j++;
	}
	key[i] = '\0';
	return (key);
}

char	*determine_content(char *args, t_garbage *gc)
{
	char	*content;
	char	*val_dup;

	content = ft_strchr(args, '=');
	if (!content)
		return (NULL);
	val_dup = g_strdup(gc, content + 1);
	return (val_dup);
}

void	append_env_node(t_env **head, t_env *new_node)
{
	t_env	*current;

	if (!new_node)
		return ;
	if (!*head)
	{
		*head = new_node;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_node;
}
