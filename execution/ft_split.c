/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhamdali <mhamdali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 12:23:22 by mhamdali          #+#    #+#             */
/*   Updated: 2025/07/30 22:44:05 by mhamdali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_word(char const *s, char c)
{
	int	count;

	count = 0;
	while (*s)
	{
		while (*s && *s == c)
			s++;
		if (*s && *s != c)
			count++;
		while (*s && *s != c)
			s++;
	}
	return (count);
}

static size_t	my_strlen(const char *str, char c)
{
	size_t	len;

	len = 0;
	while (str[len] != '\0' && str[len] != c)
		len++;
	return (len);
}

static char	**allocate_result(int count, t_garbage *gc)
{
	char	**result;

	result = g_malloc(gc, (count + 1) * sizeof(char *));
	return (result);
}

static int	process_word(char **result, const char **s, char c, t_garbage *gc)
{
	size_t	len;

	len = my_strlen(*s, c);
	if (len > 0)
	{
		result[0] = g_malloc(gc, len + 1);
		if (!result[0])
			return (-1);
		ft_strlcpy(result[0], *s, len + 1);
		*s += len;
		return (1);
	}
	return (0);
}

char	**ft_split(char const *s, char c, t_garbage *gc)
{
	int		count;
	char	**result;
	int		i;

	if (!s)
		return (NULL);
	count = count_word(s, c);
	result = allocate_result(count, gc);
	if (!result)
		return (NULL);
	i = 0;
	while (*s && i < count)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			if (process_word(&result[i], &s, c, gc) == 1)
				i++;
			else if (process_word(&result[i], &s, c, gc) == -1)
				return (NULL);
		}
	}
	result[i] = NULL;
	return (result);
}
