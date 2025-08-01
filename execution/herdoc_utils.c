/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhamdali <mhamdali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 13:20:09 by mhamdali          #+#    #+#             */
/*   Updated: 2025/07/31 01:21:16 by mhamdali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_tmp_filename(void)
{
	char	*tmpfile;

	tmpfile = "/tmp/minishell_heredoc.tmp";
	if (access(tmpfile, F_OK) == 0)
		tmpfile = "/tmp/minishell_handle.tmp";
	return (tmpfile);
}

int	open_heredoc_file(char *tmpfile)
{
	int	fd;

	fd = open(tmpfile, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd == -1)
		perror("open");
	return (fd);
}

char	*get_env_var(char **env, const char *var_name)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(var_name);
	while (env && env[i])
	{
		if (ft_strncmp(env[i], var_name, len) == 0 && env[i][len] == '=')
		{
			return (env[i] + len + 1);
		}
		i++;
	}
	return (NULL);
}

int	handle_all_heredocs(t_command *cmd)
{
	t_redirect	*redir;
	int			fd;

	while (cmd)
	{
		redir = cmd->redirect;
		while (redir)
		{
			if (redir->type == 4)
			{
				fd = handle_heredoc(redir, cmd->env);
				if (fd == -1)
					return (-1);
				redir->heredoc_fd = fd;
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (0);
}
