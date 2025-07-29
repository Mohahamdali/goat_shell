/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhamdali <mhamdali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 16:33:20 by mhamdali          #+#    #+#             */
/*   Updated: 2025/07/29 07:36:06 by mhamdali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_cmd_equal(char *cmd, char *builtin_name)
{
	return (ft_strlen(cmd) == ft_strlen(builtin_name)) &&
		(ft_strncmp(cmd, builtin_name, ft_strlen(builtin_name)) == 0);
}

int	is_builtins(t_command *cmd)
{
	int		i;
	char	*builtins[] = {"echo", "cd", "pwd", "export", "unset", "env", 
		"exit", NULL};

	if (!cmd->args || !cmd->args[0])
		return (0);
	i = 0;
	while (builtins[i])
	{
		if (is_cmd_equal(cmd->args[0], builtins[i]) == 1)
			return (1);
		i++;
	}
	return (0);
}

void	execution_builtins(t_command *cmd, t_env **env, int flag, 
						t_gc_manager *gc)
{
	if (is_cmd_equal(cmd->args[0], "echo"))
		last_exit_status(buittin_echo(cmd), 1);
	else if (is_cmd_equal(cmd->args[0], "pwd"))
		built_pwd(cmd);
	else if (is_cmd_equal(cmd->args[0], "exit"))
		builtin_exit(cmd, flag, gc);
	else if (is_cmd_equal(cmd->args[0], "export"))
		last_exit_status(export(cmd, env, gc->env_gc), 1);
	else if (is_cmd_equal(cmd->args[0], "env"))
		last_exit_status(env_built(cmd), 1);
	else if (is_cmd_equal(cmd->args[0], "cd"))
		last_exit_status(cd_builtin(cmd, gc->env_gc), 1);
	else if (is_cmd_equal(cmd->args[0], "unset"))
		unset_all_env(env, &cmd->args[1]);
}

static int	handle_single_command(t_command *cmd, t_env **env, t_gc_manager *gc)
{
	int	check;

	if (handle_all_heredocs(cmd) == -1)
		return (-1);
	check = apply_redirections(cmd);
	if (is_builtins(cmd) && check != -1)
		execution_builtins(cmd, env, 0, gc);
	else if (check != -1)
		external_command(NULL, cmd, gc);
	dup2(cmd->save_out, 1);
	dup2(cmd->save_in, 0);
	return (0);
}

int	execution(t_command *cmd, t_env **env, int flag, t_gc_manager *gc)
{
	(void)flag;
	if (!cmd)
		return (-1);
	cmd->save_out = dup(1);
	cmd->save_in = dup(0);
	cmd->env = *env;
	if (!cmd->env)
		return (-1);
	cmd->ori_env = convert_linked_to_char(cmd->env, gc->cmd_gc);
	if (!cmd->ori_env)
		return (-1);
	if (cmd->next != NULL)
		execution_pipes(cmd, gc);
	else
	{
		if (handle_single_command(cmd, env, gc) == -1)
		{
			clean_save_in(cmd);
			return (-1);
		}
	}
	clean_save_in(cmd);
	return (0);
}
