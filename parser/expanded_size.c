#include "../minishell.h"

static bool	count_alphanum(t_env *env, char *content, int *size, int *i)
{
	t_env	*tmp;
	int		start;
	char	*new_key;

	start = *i;
	while (content[*i] && (ft_isalnumm(content[*i])))
		(*i)++;
	new_key = ft_substr(content, start, *i - start);
	if (!new_key)
		return (1);
	tmp = env;
	while (tmp)
	{
		if (!ft_strcmp(new_key, tmp->key))
		{
			*size += ft_strlen(tmp->content);
			break;
		}
		tmp = tmp->next;
	}
	free(new_key);
	new_key = NULL;
	return (0);
}

static int	count_expand_dollar(t_env *env, char *content, int *size, int *i)
{
	(*i)++;
	if (content[*i] == '?')
	{
		*size += 10;
		(*i)++;
	}
	else if (content[*i] == '$')
	{
		*size += 2;
		(*i)++;
	}
	else if (ft_isalnumm(content[*i]))
	{
		if (count_alphanum(env, content, size, i))
			return (1);
	}
	else if (content[*i])
		*size += 2;
	else
		*size+= 1;
	return (0);
}

int	expanded_size(char *content, t_env *env, int i)
{
	int		size;
	char	quote_type;

	size = 0;
	quote_type = '\0';
	while (content[i])
	{
		if ((content[i] == '\'' || content[i] == '\"') && (!quote_type || quote_type == content[i]))
		{
			handle_quotes(content, &i, &quote_type);
			continue;
		}
		if (content[i] == '$' && (quote_type == '\"' || quote_type == '\0'))
		{
			if (count_expand_dollar(env, content, &size, &i))
				return (-1);
		}
		else
		{
			size++;
			i++;
		}
	}
	return (size);
}

