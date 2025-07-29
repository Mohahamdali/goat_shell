#include "../minishell.h"

int	fill_list_args(t_command **list, t_token *token, t_flag *fg, bool found_cmd, t_garbage *g_c)
{
	if (found_cmd)
		fg->found_command = true;
	(*list)->args[fg->i] = g_strdup(g_c, token->content);
	if (!(*list)->args[fg->i])
		return (1);
	(*list)->args_fo_exp[fg->i] = token->empty_expanded;
	fg->i++;
	return (0);
}
int	handle_redirect_filename(t_command **list, t_token *token, bool *at_redirect, t_garbage *g_c)
{
	if ((*list)->redirect->type == RED_HEREDOC)
	{
		(*list)->redirect->delimiter = g_strdup(g_c, token->content);
		if (!(*list)->redirect->delimiter)
			return (1);
	}
	else
	{
		(*list)->redirect->filename = g_strdup(g_c, token->content);
		if (!(*list)->redirect->filename)
			return (1);
		if (token->empty_expanded)
			return (write(2, "Minishell: ambiguous redirect\n", 31), last_exit_status(1, 1), 1);
	}
	*at_redirect = false;
	return (0);
}

int	init_redirect(t_command **list, t_token *token, bool *at_redirect, t_garbage *g_c)
{
	if ((*list)->redirect->type != 0)
	{
		(*list)->redirect->next = redirect_alloc(g_c);
		if (!(*list)->redirect->next)
			return (1);
		(*list)->redirect = (*list)->redirect->next;
	}
	(*list)->redirect->type = token->type;
	*at_redirect = true;
	return (0);
}
int	handle_multispace_tokens(t_token *token, t_command **list, t_flag *fg, int args_nb, t_garbage *g_c)
{
	// int		j;
	int		x;
	char	**words;

	words = my_split(token->content, g_c);
	if (!words)
		return (1);
	if (!fg->found_command)
		fg->found_command = true;
	x = 0;
	while (words[x] && fg->i + x < args_nb * 2) // * 2 for x++ and i++ in the same time
	{
		(*list)->args[fg->i] = g_strdup(g_c, words[x++]);
		if (!(*list)->args[fg->i])
			return (1);
		(*list)->args_fo_exp[fg->i] = token->empty_expanded;
		(fg->i)++;
	}
    // j = 0;
    // while (words[j])
    //     g_free_one_ptr(g_c, words[j++]);
    // g_free_one_ptr(g_c, words);
	// words = NULL;
	return (0);
}
int	handle_string_token(t_command **list, t_token **tmp, int args_nb, t_flag *fg, t_garbage *g_c)
{
	if ((*tmp)->type == STRING)
	{
		if ((*list)->args && (*list)->args[0] && !ft_strcmp((*list)->args[0], "export"))
		{
			if (fill_list_args(list, *tmp, fg, false, g_c))
				return (1);
		}
		else if (!(*tmp)->quoted && count_strings((*tmp)->content))
		{
			if (fg->at_redirect == true)
				return (write(2, "Minishell: ambiguous redirect\n", 31), last_exit_status(1, 1), 1);
			else if (handle_multispace_tokens(*tmp, list, fg, args_nb, g_c))
				return (1);
		}
		else if (!fg->found_command && fg->at_redirect == false)
			fill_list_args(list, *tmp, fg, true, g_c);
		else if (fg->i < args_nb && fg->at_redirect == false)
			fill_list_args(list, *tmp, fg, false, g_c);
		else if (fg->at_redirect == true)
			if (handle_redirect_filename(list, *tmp, &fg->at_redirect, g_c))
				return (1);
	}
	else if ((*tmp)->type == RED_IN || (*tmp)->type == RED_OUT || (*tmp)->type == RED_APPEND || (*tmp)->type == RED_HEREDOC)
		if (init_redirect(list, *tmp, &fg->at_redirect, g_c))
			return (1);
	*tmp = (*tmp)->next;
	return (0);
}
int	handle_pipe(t_command **list, t_token **tmp, t_redirect **redirect_head, t_garbage *g_c)
{
	(*list)->next = create_command(g_c);
	if (!(*list)->next)
		return (1);\
	(*list)->redirect = *redirect_head;
	*tmp = (*tmp)->next;
	*list = (*list)->next;
	return (0);
}
int	init_head_list(t_command **list, t_command **list_head, t_garbage *g_c)
{
	*list_head = create_command(g_c);
	if(!*list_head)
		return (1);
	*list = *list_head;
	return (0);
}
int count_strings(char *content)
{
	int	i;
	int	in_word;
	int	words_nb;
	
	i = 0;
	in_word = 0;
	words_nb = 0;
	while (content[i])
	{
		if (content[i] == ' ' || content[i] == '\t')
		{
			in_word = 0;
		}
		else if (!in_word)
		{
			in_word = 1;
			words_nb++;
		}
		i++;
	}
	if (words_nb > 1)
		return (words_nb);
	else
		return (0);
}
