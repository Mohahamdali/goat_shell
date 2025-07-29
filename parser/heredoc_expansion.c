#include "../minishell.h"

static size_t	new_del_size(char *del)
{
	int		i;
	size_t	len;
	char	quote_type;

	i = 0;
	len = 0;
	quote_type = '\0';
	while (del[i])
	{
		if ((del[i] == '\'' || del[i] == '\"') && (!quote_type || quote_type == del[i]))
		{
			if (!quote_type)
				quote_type = del[i];
			else if (quote_type == del[i])
				quote_type = '\0';
			i++;
			continue;
		}
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}
static void	init_variables(int *i, int *x, char *quote_type)
{
	*i = 0;
	*x = 0;
	*quote_type = '\0';
}
static int	alloc_new_delim(char **new_del, char *delim, t_garbage *g_c)
{
	size_t	len;

	len = new_del_size(delim);
	*new_del = g_malloc(g_c, len + 1);
	if (!*new_del)
		return (1);
	(*new_del)[len] = '\0';
	return (0);
}

static char	*remove_delimiter_quotes(char *delim, int x, t_garbage *g_c)
{
	int		i;
	char	quote_type;
	char	*new_del;

	init_variables(&i, &x, &quote_type);
	if (alloc_new_delim(&new_del, delim, g_c))
		return (NULL);
	while (delim[x])
	{
		if (delim[x] == '$')
		{
			if (delim[x + 1] == '\'' || delim[x + 1] == '\"')
			{
				x++;
				continue ;
			}
		}
		if ((delim[x] == '\'' || delim[x] == '\"') && (!quote_type || quote_type == delim[x]))
		{
			handle_quotes(delim, &x, &quote_type);
			continue;
		}
		new_del[i++] = delim[x++];
	}
	return (new_del);
}

static int	check_delim_quotes(t_redirect *t_redir, t_garbage *g_c)
{
	int	i;
	int	x;

	i = 0;
	x = 0;
	while (t_redir->delimiter[i])
	{
		if (t_redir->delimiter[i] == '\'' || t_redir->delimiter[i] == '\"')
		{
			t_redir->heredoc_exp = false;
			t_redir->delimiter = remove_delimiter_quotes(t_redir->delimiter, x, g_c);
			if (!t_redir->delimiter)
				return (1);
			break ;
		}
		i++;
	}
	return (0);
}
int	must_expand(t_command *cmd, t_garbage *g_c)
{
	t_command	*t_cmd;
	t_redirect	*t_redir;

	t_cmd = cmd;
	while (t_cmd)
	{
		t_redir = t_cmd->redirect;
		while (t_redir)
		{
			if (t_redir->delimiter)
				if (check_delim_quotes(t_redir, g_c))
					return (1);
			t_redir = t_redir->next;
		}
		t_cmd = t_cmd->next;
	}
	return (0);
}
