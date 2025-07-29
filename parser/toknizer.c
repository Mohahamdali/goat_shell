#include "../minishell.h"

// void	clear_tokens(t_token *tokens)
// {
// 	t_token	*tmp;

// 	while (tokens)
// 	{
// 		tmp = tokens->next;
// 		free(tokens->content);
// 		tokens->content = NULL;
// 		free(tokens);
// 		tokens = tmp;
// 	}
// }

static int	token_plus(char *content, t_type type, t_token **tokens, t_garbage *g_c)
{
	t_token	*new_token;
	t_token	*tmp;

	new_token = g_malloc(g_c, sizeof(t_token));
	if (!new_token)
		return (1);
	new_token->content = g_strdup(g_c, content);
	if (!new_token->content)
		return (1);
	new_token->type = type;
	new_token->quoted = true;
	new_token->next = NULL;
	if (!*tokens)
	{
		*tokens = new_token;
		return (0);
	}
	tmp = *tokens;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_token;
	return (0);
}

static int	special_cases(char *line, int i)
{
	if (line[i] == '|' && line[i + 1] == '>')
		return (printf("Minishell: syntax error near unexpected token `|>'\n"), 1);
	if (line[i] == '|' && line[i + 1] == '<')
		return (printf("Minishell: syntax error near unexpected token `|<'\n"), 1);
	else if (line[i] == '>' && line[i + 1] == '|')
		return (printf("Minishell: syntax error near unexpected token `>|'\n"), 1);
	else if (line[i] == '<' && line[i + 1] == '|')
		return (printf("Minishell: syntax error near unexpected token `<|'\n"), 1);
	else if (line[i] == '>' && line[i + 1] == '<')
		return (printf("Minishell: syntax error near unexpected token `><'\n"), 1);
	else if (line[i] == '<' && line[i + 1] == '>')
		return (printf("Minishell: syntax error near unexpected token `<>'\n"), 1);
	else if (line[i] == '|' && line[i + 1] == '|')
		return (printf("Minishell: syntax error near unexpected token `||'\n"), 1);
	return (0);
}

int	handle_redirect_out(char *line, t_token **tokens, int *i, t_garbage *g_c)
{
	if (line[*i + 1] && line[*i + 1] == '>')
	{
		if (token_plus(">>", RED_APPEND, tokens, g_c))
			return (1);
		*i += 2;
	}
	else
	{
		if (token_plus(">", RED_OUT, tokens, g_c))
			return (1);
	}
	return (0);
}
int	handle_redirect_in(char *line, t_token **tokens, int *i, t_garbage *g_c)
{
	if (line[*i + 1] && line[*i + 1] == '<')
	{
		if (token_plus("<<", RED_HEREDOC, tokens, g_c))
			return (1);
		*i += 2;
	}
	else
	{
		if (token_plus("<", RED_IN, tokens, g_c))
			return (1);
	}
	return (0);
}

int	handle_operators(char *line, int i, t_token **tokens, t_garbage *g_c)
{
	if (special_cases(line, i))
			return (last_exit_status(2, 1), -1);
	if (line[i] == '|')
	{
		if(token_plus("|", PIPE, tokens, g_c))
			return (-1);
	}
	else if (line[i] == '>')
	{
		if (handle_redirect_out(line, tokens, &i, g_c))
			return (-1);
	}
	else if (line[i] == '<')
	{
		if (handle_redirect_in(line, tokens, &i, g_c))
			return (-1);
	}
	return (++i);
}
int	quotes_handler(char *line, bool *in_quotes, char *quote_type, int *i)
{
	while (line[*i])
	{
		if (!*in_quotes && (line[*i] == ' ' || line[*i] == '\t' || line[*i] == '|' || line[*i] == '>' || line[*i] == '<'))
			break ;
		if ((line[*i] == '\'' || line[*i] == '\"') && (!*in_quotes || line[*i] == *quote_type))
		{
			if (*in_quotes == false)
			{
				*in_quotes = true;
				*quote_type = line[*i];
			}
			else
				*in_quotes = false;
		}
		(*i)++;
	}
	if (*in_quotes == true)
	{
		write (2, "Minishell: Error Unclosed Quote(s) !\n", 38);
		return (1);
	}
	return (0);
}
static int	handle_string(char *line, int i, t_token **tokens, t_garbage *g_c)
{
	int		start;
	bool	in_quotes;
	char	quote_type;
	char	*content;

	start = i;
	in_quotes = false;
	quote_type = '\0';
	if (quotes_handler(line, &in_quotes, &quote_type, &i))
		return (-1);
	if (i > start)
	{
		content = g_substr(g_c, line, start, i - start);
		if (!content)
			return (-1);
		content[i - start] = '\0';
		if (token_plus(content, STRING, tokens, g_c))
			return (-1);
		// free(content);
		return (i);
	}
	return (-1);
}
void	add_env_to_tokens(t_token **tokens, t_env *env)
{
	t_token	*tmp;

	if (tokens && *tokens)
	{
		tmp = *tokens;
		while (tmp->next)
			tmp = tmp->next;
		tmp->env = env;	
	}	
}
t_token	*tokenizer(char *str, t_env *env, t_garbage *cmd_gc)
{
	int		i;
	char	*line;
	t_token	*tokens;

	i = 0;
	tokens = NULL;
	line = trimmed(str, cmd_gc);
	if (!line || !*line)
		return (NULL);
	while(line[i])
	{
		while (line[i] && (line[i] == ' ' || line[i] == '\t'))
			i++;
		if (line[i] == '|' || line[i] == '<' || line[i] == '>')
			i = handle_operators(line, i, &tokens, cmd_gc);
		else if (line[i])
			i = handle_string(line, i, &tokens, cmd_gc);
		if (i == -1)
			return (NULL);
		add_env_to_tokens(&tokens, env);
	}
	return (tokens);
}
