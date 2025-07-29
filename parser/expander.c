#include "../minishell.h"

static int	handle_alphanum(t_exp *exp, int *i, int *x, t_env *env, t_garbage *g_c)
{
	int	empty_expanded;

	empty_expanded = 0;

	if (exp->content[*i] >= '0' && exp->content[*i] <= '9')
		(*i)++;
	else
	{
		empty_expanded = expand_alphanumeric(exp, i, x, env, g_c);
		if (empty_expanded == -1)
			return (-1);
	}
	return (empty_expanded);
}
static int	check_expansion(t_exp *exp, int *i, int *x, t_env *env, t_garbage *g_c)
{
	exp->empty_exp = handle_alphanum(exp, i, x, env, g_c);
	if (exp->empty_exp == -1)
		return (1);
	else if (exp->empty_exp == 1)
		(*exp->token)->empty_expanded = exp->empty_exp;
	return (0);
}
static void	single_quote(t_exp *exp, int *i, int *x)
{
	(*exp->expanded)[(*x)++] = '$';
	(*exp->expanded)[(*x)++] = exp->content[(*i)++];
}
static void	two_double_quotes(t_exp *exp, int *i, int *x)
{
	(*exp->expanded)[(*x)++] = '$';
	(*exp->expanded)[(*x)++] = exp->content[(*i)++];
	(*exp->expanded)[(*x)++] = exp->content[(*i)++];
}
static int	expand_quotes(t_exp *exp, int *i, int *x)
{
	if (exp->content[*i] == '\"' && exp->content[*i + 1] && exp->content[*i + 1] == '\"')
	{
		if (exp->quote_type == '\"')
		{
			(*exp->expanded)[(*x)++] = '$';
			*i += 2;
		}
		else
			return (1);
	}
	else if (exp->content[*i] == '\'' && exp->content[*i + 1] && exp->content[*i + 1] == '\'')
	{
		if (exp->quote_type == '\"')
			two_double_quotes(exp, i, x);
		else
			return (1);
	}
	else if (exp->content[*i] == '\'')
		single_quote(exp, i, x);
	else if (exp->content[*i] == '\"' && exp->quote_type == '\"')
		(*exp->expanded)[(*x)++] = '$';
	else
		// return (1);
		(*exp->expanded)[(*x)++] = exp->content[(*i)++];
	return (0);
}
static int	expand_variables(t_exp *exp, int *i, int *x, t_env *env, t_garbage *g_c)
{
	(*i)++;
	if (exp->content[*i] == '?')
		expand_exit_status(exp->expanded, i, x, env, g_c);
	else if (exp->content[*i] == '$' || exp->content[*i] == '_')
		dollar_underscore(exp->content, exp->expanded, i, x);
	else if (exp->content[*i] == '\"' || exp->content[*i] == '\'') // must handle this case !!!
	{
		if (expand_quotes(exp, i, x))
			return (1);
	}
	else if (ft_isalnumm(exp->content[*i]))
	{
		if (check_expansion(exp, i, x, env, g_c))
			return (-1);
	}
	else if (exp->content[*i])
	{
		(*exp->expanded)[(*x)++] = '$';
		(*exp->expanded)[(*x)++] = exp->content[(*i)++];
	}
	else
		(*exp->expanded)[(*x)++] = '$';
	return (0);
}

static int	expander_main_loop(char *content, char **expanded, t_exp *exp, t_env *env, t_garbage *g_c)
{
	int		i;
	int		x;
	char	quote_type;

	init_vars(exp, &i, &x, &quote_type);
	while (content[i])
	{
		if ((content[i] == '\'' || content[i] == '\"') && (!quote_type || quote_type == content[i]))
		{
			handle_quotes(content, &i, &quote_type);
			exp->quote_type = quote_type;
			continue;
		}
		if (content[i] == '$' && (quote_type == '\"' || quote_type == '\0'))
		{
			exp->m = expand_variables(exp, &i, &x, env, g_c);
			if (exp->m == 1)
				continue ;
			else if (exp->m == -1)
				return (1);
		}
		else
			(*expanded)[x++] = content[i++];
	}
	(*expanded)[x] = '\0';
	return (0);
}
// dont forget $_ must be handled!!
char	*expander(t_token **token, char *content, t_env *env, t_garbage *g_c)
{
	char	*expanded;
	t_exp	exp;
	int		len;
	int		i;

	i = 0;
	exp.content = content;
	exp.expanded = &expanded;
	exp.token = token;
	exp.empty_exp = 0;
	len = expanded_size(content, env, i);
	if (len == -1)
		return (NULL);
	if (token && is_only_vars(content))
        (*token)->quoted = false;
	expanded = g_malloc(g_c, len + 1);
	if (!expanded)
		return (NULL);
	if (expander_main_loop(content, &expanded, &exp, env, g_c))
		return (NULL);
	// if (token)
	// 	free(content);
	return (expanded);
}

int		expand_tokens(t_token **token, t_env *env, t_garbage *cmd_gc)
{
	int		type;
	t_token	*tmp;

	type = 0;
	tmp = *token;
	while (tmp)
	{
		if (tmp->type == STRING && type != 4)
			tmp->content = expander(&tmp, tmp->content, env, cmd_gc);
		if (!tmp->content)
			return (1);
		type = tmp->type;
		tmp = tmp->next;
	}
	return (0);
}
