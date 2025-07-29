#include "../minishell.h"

void	free_default_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
		free(env[i++]);
	free(env);
}

void	clear_env(t_env *env_head)
{
	t_env	*tmp;

	while (env_head)
	{
		tmp = env_head->next;
		free(env_head->key);
		env_head->key = NULL;
		if (env_head->content)
		{
			free(env_head->content);
			env_head->content = NULL;
		}
		free(env_head);
		env_head = tmp;
	}
}

char	**create_default_env(t_garbage *g_c)
{
	char	**env;

	env = g_malloc(g_c, sizeof(char *) * 6);
	if (!env)
		return (NULL);
	env[0] = g_strdup(g_c, "PWD=/home/aachata/minishell");
	env[1] = g_strdup(g_c, "SHLVL=1");
	env[3] = g_strdup(g_c, "OLDPWD");
	env[2] = g_strdup(g_c, "_=/usr/bin/env");
	env[4] = g_strdup (g_c, "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/snap/bin");
	env[5] = NULL;
	if (!env[0] || !env[1] || !env[2] || !env[3] || !env[4])
		return (NULL);

	return (env);
}
