/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhamdali <mhamdali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 16:50:29 by mhamdali          #+#    #+#             */
/*   Updated: 2025/07/29 07:22:59 by mhamdali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	validate_and_get_key_content(char *arg, char **key,
		char **content, t_garbage *gc)
{
	if (parse_export(arg))
	{
		printf("minishell: export: `%s`: not a valid identifier\n", arg);
		return (0);
	}
	*key = determine_key(arg, gc);
	if (!*key)
		return (-1);
	*content = determine_content(arg, gc);
	if (*content == NULL && ft_strchr(arg, '=') != NULL)
		return (-1);
	return (1);
}

int	create_and_append_node(t_env **env, char *key, char *content,
		t_garbage *gc)
{
	t_env	*new_node;

	new_node = create_env_node(key, content, gc);
	if (!new_node)
		return (1);
	append_env_node(env, new_node);
	return (0);
}

static int	handle_no_args_export(t_command *cmd)
{
	sort_export(cmd->env);
	print_env(cmd->env);
	return (0);
}

static int	process_export_arg(t_command *cmd, t_env **env, char *arg,
		t_garbage *gc)
{
	char	*key;
	char	*content;
	int		validation_result;

	validation_result = validate_and_get_key_content(arg, &key, &content, gc);
	if (validation_result == -1)
		return (1);
	if (validation_result == 0)
		return (0);
	gc ->append = is_append_operation(arg);
	if (!change_env_already(cmd->env, key, content, gc))
	{
		if (create_and_append_node(env, key, content, gc))
			return (1);
	}
	return (0);
}

int	export(t_command *cmd, t_env **env, t_garbage *gc)
{
	int	i;
	int	result;

	if (!cmd->args[1])
		return (handle_no_args_export(cmd));
	i = 0;
	while (cmd->args && cmd->args[++i])
	{
		result = process_export_arg(cmd, env, cmd->args[i], gc);
		if (result == 1)
			return (1);
	}
	return (0);
}
