/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhamdali <mhamdali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 03:33:07 by mhamdali          #+#    #+#             */
/*   Updated: 2025/07/19 03:33:08 by mhamdali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int save_output_redi(t_redirect *redirect)
{
    int fd = open(redirect->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1)
    {
         write(2, "minishell: ", 11);
        write(2, redirect->filename, ft_strlen(redirect->filename));
         write(2, ": permission denied\n", 21);
        return (-1);
    }
    if (dup2(fd, 1) == -1)
    {
        perror("Error redirecting stdout");
        close(fd);
        return -1;
    }
    close(fd);
    return 0;
}

int handle_input_redi(t_redirect *redirect)
{
    int fd = open(redirect->filename, O_RDONLY);
    if (fd == -1)
    {
        write(2, "minishell: ", 11);
        write(2, redirect->filename, ft_strlen(redirect->filename));
        write(2, ": No such file or directory\n", 29);
        return (-1);
    }
    int save_fd_0 = dup(0);
    if (dup2(fd, 0) == -1)
    {
        perror("Error redirecting stdinput");
        close(fd);
        return -1;
    }
    close(fd);
    return save_fd_0;
}

int handle_output_redi(t_redirect *redirect)
{
    int fd = open(redirect->filename, O_CREAT | O_TRUNC | O_WRONLY, 0644);
    if (fd == -1)
    {
        write(2, "minishell: ", 11);
        write(2, redirect->filename, ft_strlen(redirect->filename));
        write(2, ": permission denied\n", 21);
        return (-1);
    }
   
    if (dup2(fd, 1) == -1)
    {
        perror("Error redirecting stdout");
        close(fd);
        return -1;
    }
    close(fd);
    return 0;
}

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