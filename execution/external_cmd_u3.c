/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_cmd_u3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhamdali <mhamdali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 03:56:44 by mhamdali          #+#    #+#             */
/*   Updated: 2025/07/29 03:48:30 by mhamdali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_linked_list(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env -> next;
	}
	return (count);
}

static char	*create_env_string(t_env *env, t_garbage *gc)
{
	int		len;
	char	*str;

	len = ft_strlen(env->key) + ft_strlen(env->content) + 2;
	str = g_malloc(gc, len);
	if (!str)
		return (NULL);
	ft_strlcpy(str, env->key, len);
	ft_strlcat(str, "=", len);
	ft_strlcat(str, env->content, len);
	return (str);
}

static int	is_valid_env_node(t_env *env)
{
	return (env && env->key && env->content);
}

char	**convert_linked_to_char(t_env *env, t_garbage *g_c)
{
	int		i;
	int		count;
	char	**buffer_env;

	count = count_linked_list(env);
	buffer_env = g_malloc(g_c, sizeof(char *) * (count + 1));
	if (!buffer_env)
		return (NULL);
	i = 0;
	while (env && i < count)
	{
		if (!is_valid_env_node(env))
		{
			env = env->next;
			continue ;
		}
		buffer_env[i] = create_env_string(env, g_c);
		if (!buffer_env[i])
			return (NULL);
		i++;
		env = env->next;
	}
	buffer_env[i] = NULL;
	return (buffer_env);
}
