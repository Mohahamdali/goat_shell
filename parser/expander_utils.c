#include "../minishell.h"

void	handle_quotes(char *content, int *i, char *quote_type)
{
	if (!*quote_type)
		*quote_type = content[*i];
	else if (*quote_type == content[*i])
		*quote_type = '\0';
	(*i)++;
}

void	expand_exit_status(char **expanded, int *i, int *x, t_env *env, t_garbage *g_c)
{
	char	*tmp_str;

	tmp_str = ft_itoa(env->exit_status, g_c);
	ft_strcpy(*expanded + *x, tmp_str);
	*x += ft_strlen(tmp_str);
	free(tmp_str);
	tmp_str = NULL;
	(*i)++;
}

void	dollar_underscore(char *content, char **expanded, int *i, int *x)
{
	if (content[*i] == '$')
	{
		(*expanded)[(*x)++] = '$';
		(*expanded)[(*x)++] = '$';
		(*i)++;
	}
	else if (content[*i] == '_')
	{
		(*expanded)[(*x)++] = '$';
		(*expanded)[(*x)++] = '_';
		(*i)++;
	}
}

bool	is_only_vars(char *content)
{
    int		i;
    char	quote_type;

    i = 0;
    quote_type = '\0';
    while (content[i])
    {
        if ((content[i] == '\'' || content[i] == '\"') && (!quote_type || quote_type == content[i]))
        {
            handle_quotes(content, &i, &quote_type);
            continue ;
        }
        if (quote_type == '\0' && (content[i] == ' ' || content[i] == '\t'))
            return (false);
        else if (quote_type != '\0' && content[i] != '$')
            return (false);
        i++;
    }
    return (true);
}

char	*replace_env_key(char *key, t_env *env, t_garbage *g_c)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (!ft_strcmp(key, tmp->key))
			return (g_strdup(g_c, tmp->content));
		tmp = tmp->next;
	}
	return (g_strdup(g_c, ""));
}
