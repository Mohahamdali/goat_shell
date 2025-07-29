#include "../minishell.h"

static int	count_words(const char *s)
{
	int	count;
	int	in_string;

	count = 0;
	in_string = 0;
	while (*s)
	{
		if (*s != ' ' && *s != '\t' && !in_string)
		{
			in_string = 1;
			count++;
		}
		else if (*s == ' ' || *s == '\t')
			in_string = 0;
		s++;
	}
	return (count);
}

static char	*alloc_word(const char *s, int len, t_garbage *g_c)
{
	int		i;
	char	*word;

	i = 0;
	word = (char *)g_malloc(g_c, len + 1);
	if (!word)
		return (NULL);
	while (i < len)
	{
		word[i] = s[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

// static void	free_all(char **result, int i)
// {
// 	while (i >= 0)
// 		free(result[i--]);
// 	free(result);
// 	result = NULL;
// }

static	char	**split_body(char **result, const char *s, t_garbage *g_c)
{
	int	i;
	int	j;
	int	start;

	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[i] != ' ' && s[i] != '\t')
		{
			start = i;
			while (s[i] && s[i] != ' ' && s[i] != '\t')
				i++;
			result[j] = alloc_word(s + start, i - start, g_c);
			if (!result[j++])
			{
				// free_all(result, j - 2);
				return (NULL);
			}
		}
		else
			i++;
	}
	result[j] = NULL;
	return (result);
}

char	**my_split(char *s, t_garbage *g_c)
{
	char	**result;

	if (!s)
		return (NULL);
	result = g_malloc(g_c, (count_words(s) + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	result = split_body(result, s, g_c);
	return (result);
}