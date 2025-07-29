/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhamdali <mhamdali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 02:40:33 by mhamdali          #+#    #+#             */
/*   Updated: 2025/07/29 02:51:49 by mhamdali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	cd_error_handler(char *oldpwd, char *pwd_env)
{
	if (oldpwd)
		free(oldpwd);
	if (pwd_env)
		free(pwd_env);
	return (1);
}

static char	*handle_no_args_cd(t_command *cmd, char *oldpwd, char *pwd_env)
{
	char	*target;

	target = get_env_var(cmd->ori_env, "HOME");
	if (!target)
	{
		write(2, "minishell: cd: HOME not set\n", 28);
		cd_error_handler(oldpwd, pwd_env);
		return (NULL);
	}
	return (target);
}

static char	*handle_empty_arg_cd(t_command *cmd, char *oldpwd, char *pwd_env)
{
	char	*target;

	if (cmd->args_fo_exp[1] == true)
	{
		target = get_env_var(cmd->ori_env, "HOME");
		if (!target)
		{
			write(2, "minishell: cd: HOME not set\n", 28);
			cd_error_handler(oldpwd, pwd_env);
			return (NULL);
		}
		return (target);
	}
	else
		return (NULL);
}

char	*get_cd_target(t_command *cmd, char *oldpwd, char *pwd_env)
{
	if (cmd->args && !cmd->args[1])
		return (handle_no_args_cd(cmd, oldpwd, pwd_env));
	if (cmd->args[1] && cmd->args[2])
	{
		write(2, "minishell: cd: too many arguments\n", 34);
		cd_error_handler(oldpwd, pwd_env);
		return (NULL);
	}
	if (cmd->args[1] && cmd->args[1][0] == '\0')
		return (handle_empty_arg_cd(cmd, oldpwd, pwd_env));
	return (cmd->args[1]);
}

void	update_pwd_env_cd_dotdot(char **pwd_env)
{
	int	i;
	int	len;

	if (!pwd_env || !(*pwd_env))
		return ;
	len = ft_strlen(*pwd_env);
	if (len == 1 && (*pwd_env)[0] == '/')
		return ;
	i = len - 1;
	while (i >= 0 && (*pwd_env)[i] != '/')
		i--;
	if (i == 0)
		(*pwd_env)[1] = '\0';
	else if (i > 0)
		(*pwd_env)[i] = '\0';
}
