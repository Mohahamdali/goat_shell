#include "../minishell.h"

int	expand_alphanumeric(t_exp *exp, int *i, int *x, t_env *env, t_garbage *g_c)
{
	int		start;
	char	*replaced;
	char	*sub;
	int		empty_expanded;

	empty_expanded = 0;
	start = *i;
	while (exp->content[*i] && ft_isalnumm(exp->content[*i])) // must handle _
		(*i)++;
	sub = g_substr(g_c, exp->content, start, *i - start);
	if (!sub)
		return (-1);
	replaced = replace_env_key(sub, env, g_c);
	if (!replaced)
		return (-1);
	if (replaced[0] == '\0')
		empty_expanded = 1;
	ft_strcpy((*exp->expanded) + *x, replaced);
	*x += ft_strlen(replaced);
	return (empty_expanded);
}

void	init_vars(t_exp *exp, int *i, int *x, char *quote_type)
{
	*i = 0;
	*x = 0;
	*quote_type = '\0';
	if (exp)
	{
		exp->quote_type = *quote_type;
		exp->m = 0;
	}
}
