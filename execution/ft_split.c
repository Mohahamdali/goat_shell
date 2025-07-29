/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhamdali <mhamdali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 12:23:22 by mhamdali          #+#    #+#             */
/*   Updated: 2025/07/29 08:02:41 by mhamdali         ###   ########.fr       */
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

char	**ft_split(char const *s, char c, t_garbage *gc)
{
	int		count;
	char	**result;
	size_t	i;
	size_t	len;

	if (!s)
		return (NULL);
	count = count_word(s, c);
	result = g_malloc(gc, (count + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	i = 0;
	while (*s && i < (size_t)count)
	{
		while (*s == c)
			s++;
		if (*s && (len = my_strlen(s, c)))
		{
			result[i] = g_malloc(gc, len + 1);
			if (!result[i])
				return (NULL);
			ft_strlcpy(result[i++], s, len + 1);
			s += len;
		}
	}
	result[i] = NULL;
	return (result);
}
