/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhamdali <mhamdali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 03:33:07 by mhamdali          #+#    #+#             */
/*   Updated: 2025/07/30 22:40:05 by mhamdali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_heredoc_fd(t_redirect *redir)
{
	if (redir->heredoc_fd == -1)
		return (-1);
	if (dup2(redir->heredoc_fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		return (-1);
	}
	close(redir->heredoc_fd);
	redir->heredoc_fd = -1;
	return (0);
}

static int	process_redirect(t_redirect *redir)
{
	if (redir->type == 4)
		return (handle_heredoc_fd(redir));
	else if (redir->type == 2)
		return (handle_output_redi(redir));
	else if (redir->type == 1)
		return (handle_input_redi(redir));
	else if (redir->type == 3)
		return (save_output_redi(redir));
	return (0);
}

int	apply_redirections(t_command *cmd)
{
	t_redirect	*redir;

	redir = cmd->redirect;
	while (redir)
	{
		if (process_redirect(redir) == -1)
			return (-1);
		redir = redir->next;
	}
	return (0);
}
