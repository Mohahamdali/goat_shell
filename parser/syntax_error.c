#include "../minishell.h"

static const char *type_to_sign(t_token *token)
{
	if (token->type == RED_IN)
		return ("<");
	else if (token->type == RED_OUT)
		return (">");
	else if (token->type == RED_APPEND)
		return (">>");
	else if (token->type == RED_HEREDOC)
		return ("<<");
	else if (token->type == PIPE)
		return ("|");
	return ("");
}
static int	pipe_errors(t_token *tmp, bool start)
{
	if (tmp->next && tmp->next->type == PIPE)
	{
		write(2, "Minishell: syntax error near unexpected token `|'\n", 51);
		return (1);
	}
	else if (start)
	{
		write(2, "Minishell: syntax error near unexpected token `|'\n", 51);
		return (1);
	}
	else if (!tmp->next)
	{
		write(2, "Minishell: syntax error near unexpected token `newline'\n", 57);
		return (1);
	}
	return (0);
}
static int	redirect_errors(t_token *tmp)
{
	if (!tmp->next || tmp->next->type != STRING)
	{
		if (tmp->next && tmp->next->type != STRING)
			printf("Minishell: syntax error near unexpected token `%s'\n", type_to_sign(tmp->next));
		else
			write(2, "Minishell: syntax error near unexpected token `newline'\n", 57);
		return (1);
	}
	return (0);
}
bool	syntax_error(t_token *tokens)
{
	bool	start;
	t_token	*tmp;

	if (!tokens)
		return (NULL);
	tmp = tokens;
	start = true;
	while (tmp)
	{
		if (tmp->type == PIPE)
		{
			if (pipe_errors(tmp, start))
				return (true);
		}
		else if (tmp->type == RED_OUT || tmp->type == RED_IN || tmp->type == RED_APPEND || tmp->type == RED_HEREDOC)
		{
			if (redirect_errors(tmp))
				return (true);
		}
		start = false;
		tmp = tmp->next;
	}
	return (false);
}