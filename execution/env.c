/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhamdali <mhamdali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 15:08:24 by mhamdali          #+#    #+#             */
/*   Updated: 2025/07/29 03:21:44 by mhamdali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*find_env_var(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strncmp(env->key, key, ft_strlen(key) + 1) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

void built_pwd(t_command *cmd)
{
    char *s;
    int need_free = 0;

    (void)cmd;
    s = getcwd(NULL, 0);
    if (!s)
    {
        s = get_env_var(cmd->ori_env, "PWD");
        if (!s)
        {
            last_exit_status(1, 1);
            return;
        }
    }
    else
        need_free = 1;
    if (printf("%s\n", s) < 0)
    {
        if (need_free)
            free(s);
        last_exit_status(1, 1);
        return;
    }
    if (need_free)
        free(s);
    last_exit_status(0, 1);
}

static int	handle_env_args(t_command *cmd)
{
	int	i;

	i = 1;
	while (cmd->args[i])
	{
		if (cmd->args[i][0] == '-')
			return (0);
		else if (ft_strcmp(cmd->args[i], "env") != 0)
		{
			printf("env: %s: No such file or directory\n", cmd->args[1]);
			return (1);
		}
		i++;
	}
	return (-1);
}

static void	print_env_list(t_env *env)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (!tmp->content)
		{
			tmp = tmp->next;
			continue ;
		}
		printf("%s=%s\n", tmp->key, tmp->content);
		tmp = tmp->next;
	}
}

int	env_built(t_command *cmd)
{
	int	arg_result;

	if (!cmd->env)
		return (0);
	arg_result = handle_env_args(cmd);
	if (arg_result != -1)
		return (arg_result);
	print_env_list(cmd->env);
	return (0);
}
