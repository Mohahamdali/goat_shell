#include "../minishell.h"

static char	*fill_content(char *env, t_env *new_env, int i, t_garbage *g_c)
{
	int content_len;

	if (env[i] == '=')
	{
		if (env[i + 1] == '\0')
			new_env->content = g_strdup(g_c, "");
		else
		{
			content_len = ft_strlen(env) - i - 1;
			new_env->content = g_malloc(g_c, content_len + 1);
			if (!new_env->content)
				return (NULL);
			ft_memcpy(new_env->content, env + i + 1, content_len);
			new_env->content[content_len] = '\0';
		}
	}
	else
		return (NULL);
	if (!new_env->content)
		return (NULL);
	return (new_env->content);
}

static int	fill_key(char **env, t_env *new_env, int *i, t_garbage *g_c)
{
	
	while ((*env)[*i] && (*env)[*i] != '=')
		(*i)++;
	new_env->key = g_malloc(g_c, *i + 1);
	if (!new_env->key)
		return (1);
	ft_memcpy(new_env->key, *env, *i);
	new_env->key[*i] = '\0';
	return (0);
}

static t_env	*prepare_env_node(char **env, t_garbage *g_c)
{
	int		i;
	t_env	*new_env;

	i = 0;
	new_env = g_malloc(g_c, sizeof(t_env));
	if (!new_env)
		return (NULL);
	if (fill_key(env, new_env, &i, g_c))
		return (NULL);
	new_env->content = fill_content(*env, new_env, i, g_c);
	if (!new_env->content)
		return (NULL);
	return (new_env);
}

static int	set_env_loop(char **env, t_env **env_head, t_garbage *g_c)
{
	t_env	*previous;
	t_env	*current;

	while (*env)
	{
		current = prepare_env_node(env, g_c);
		if (!current)
			return (1);
		current->next = NULL;
		current->exit_status = last_exit_status(0, 0);
		if (!*env_head)
			*env_head = current;
		else
			previous->next = current;
		previous = current;
		env++;
	}
	return (0);
}

t_env	*set_env(char **env, t_garbage *g_c)
{
	t_env	*env_head;

	env_head = NULL;
	if (!env || !*env)
		env = create_default_env(g_c);
	if (!env)
		return (NULL);
	if (set_env_loop(env, &env_head, g_c))
		return (NULL);
	return (env_head);
}
