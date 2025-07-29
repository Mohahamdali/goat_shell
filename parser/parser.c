#include "../minishell.h"

t_redirect	*redirect_alloc(t_garbage *g_c)
{
	t_redirect	*redirect;

	redirect = g_malloc(g_c, sizeof(t_redirect));
	if (!redirect)
		return (NULL);
	redirect->type = 0;
	redirect->filename = NULL;
	redirect->filename_fo_exp = false;
	redirect->delimiter = NULL;
	redirect->heredoc_exp = true;
	redirect->next = NULL;
	return (redirect);
}
t_command	*create_command(t_garbage *g_c)
{
	t_command	*command;

	command = g_malloc(g_c, sizeof(t_command));
	if (!command)
		return (NULL);
	command->env = NULL;
	command->ori_env = NULL;
	command->args = NULL;
	command->args_fo_exp = NULL;
	command->redirect = redirect_alloc(g_c);
	command->next = NULL;
	if (!command->redirect)
		return (NULL);
	return (command);
}

int	count_args(t_token *tokens)
{
	t_token	*tmp;
	int		n;

	n = 0;
	tmp = tokens;
	while (tmp)
	{
		if (tmp->type == PIPE)
			break ;
		else if (tmp->type == RED_IN || tmp->type == RED_OUT || tmp->type == RED_APPEND || tmp->type == RED_HEREDOC)
			tmp = tmp->next;
		else if (tmp->type == STRING)
		{
			if (!tmp->quoted && count_strings(tmp->content))
				n += count_strings(tmp->content);
			else
				n++;
		}
		tmp = tmp->next;
	}
	return(n);
}
void	init_fg(t_flag *fg)
{
	fg->i = 0;
	fg->found_command = false;
	fg->at_redirect = false;
}
int	init_args_nb(t_command **list, t_token *token, int *args_nb, t_redirect **head, t_garbage *g_c)
{
	*args_nb = count_args(token);
	if (*args_nb > 0)
	{
		(*list)->args = g_malloc(g_c, sizeof(char *) * (*args_nb + 1));
		if (!(*list)->args)
			return (1);
		(*list)->args_fo_exp = g_malloc(g_c, sizeof(int) * (*args_nb));
		if (!(*list)->args_fo_exp)
			return (1);
		(*list)->args[0] = NULL;
		(*list)->args[*args_nb] = NULL;
	}
	*head = (*list)->redirect;
	return (0);
}
t_command	*fill_cmd_list(t_token *tokens, t_garbage *g_c)
{
	t_command	*list_head;
	t_command	*list;
	t_token		*tmp;
	t_redirect	*redirect_head;
	int			args_nb;
	t_flag		fg;

	tmp = tokens;
	if(init_head_list(&list, &list_head, g_c))
		return (NULL);
	while (tmp)
	{
		if (init_args_nb(&list, tmp, &args_nb, &redirect_head, g_c))
			return (NULL);
		init_fg(&fg);
		while (tmp && tmp->type != PIPE)
			if (handle_string_token(&list, &tmp, args_nb, &fg, g_c))
				return (NULL);
		if (tmp && tmp->type == PIPE)
			if (handle_pipe(&list, &tmp, &redirect_head, g_c))
				return  (NULL);
	}
	if (list)
		list->redirect = redirect_head;
	return (list_head);
}

void	set_execution_env(t_command *cmd_list, t_env *env, char **envv)
{
	t_command	*cmd_tmp;
	t_env		*shared_env;

	cmd_list->ori_env = envv;
	cmd_list->env = env;
	cmd_tmp = cmd_list;
	shared_env = cmd_list->env;
	while (cmd_tmp)
	{
		cmd_tmp->env = shared_env;
		cmd_tmp = cmd_tmp->next;
	}
}

t_command	*parser(t_env *env, char *line, char **envv, t_garbage *cmd_gc)
{
	t_token		*token;
	t_command	*cmd_list;

	token = tokenizer(line, env, cmd_gc);
	if (!token)
		return (NULL);
	if (!token || syntax_error(token))
		return (last_exit_status(2, 1), NULL); //check exit status
	if (expand_tokens(&token, env, cmd_gc))
		return (NULL);
	cmd_list = fill_cmd_list(token, cmd_gc);
	if (!cmd_list)
		return (NULL);
	if (must_expand(cmd_list, cmd_gc))
		return (NULL); // free previous
	set_execution_env(cmd_list, env, envv);
	return (cmd_list);
}
