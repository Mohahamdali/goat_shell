/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhamdali <mhamdali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 13:20:16 by mhamdali          #+#    #+#             */
/*   Updated: 2025/07/31 20:52:26 by mhamdali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	execute_cd(char *target, char *oldpwd, char *pwd_env)
{
	if (chdir(target) != 0)
	{
		cd_error_write(target);
		cd_error_handler(oldpwd, pwd_env);
		return (1);
	}
	return (0);
}

static int	setup_cd_vars(t_command *cmd, t_garbage *gc, char **oldpwd,
		char **pwd_env)
{
	*oldpwd = getcwd(NULL, 0);
	*pwd_env = g_strdup(gc, get_env_var(cmd->ori_env, "PWD"));
	if (!*oldpwd)
	{
		*oldpwd = g_strdup(gc, *pwd_env);
		if (!*oldpwd)
		{
			perror("cd: fallback PWD failed");
			return (1);
		}
		return (2);
	}
	return (0);
}

static void	update_oldpwd_env(t_command *cmd, char *oldpwd, t_garbage *gc)
{
	t_env	*oldpwd_node;

	oldpwd_node = find_env_var(cmd->env, "OLDPWD");
	if (oldpwd_node)
		update_env_content(oldpwd_node, oldpwd, 0, gc);
}

static void	update_pwd_env(t_command *cmd, char *target, int manual_pwd,
		t_garbage *gc)
{
	t_env	*pwd_node;
	char	*newpwd;
	char	*pwd_env;
	int		i;

	i = 1;
	if (manual_pwd)
	{
		pwd_env = g_strdup(gc, get_env_var(cmd->ori_env, "PWD"));
		update_pwd_env_cd_dotdot(&pwd_env);
	}
	pwd_node = find_env_var(cmd->env, "PWD");
	newpwd = getcwd(NULL, 0);
	if (!newpwd)
	{
		i = 0;
		newpwd = g_strdup(gc, target);
	}
	if (pwd_node)
		update_env_content(pwd_node, newpwd, 0, gc);
	if (newpwd && i && newpwd != target)
		free(newpwd);
}

int	cd_builtin(t_command *cmd, t_garbage *gc)
{
	char	*target;
	char	*oldpwd;
	char	*pwd_env;
	int		setup_result;
	int		manual_pwd;

	setup_result = setup_cd_vars(cmd, gc, &oldpwd, &pwd_env);
	if (setup_result == 1)
		return (1);
	manual_pwd = (setup_result == 2);
	update_oldpwd_env(cmd, oldpwd, gc);
	target = get_cd_target(cmd, oldpwd, pwd_env);
	if (!target)
		return (1);
	if (execute_cd(target, oldpwd, pwd_env) != 0)
		return (1);
	update_pwd_env(cmd, target, manual_pwd, gc);
	free(oldpwd);
	return (0);
}
