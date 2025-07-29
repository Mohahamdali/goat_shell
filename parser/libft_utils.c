#include "../minishell.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t		i;
	const char	*s;
	char		*d;

	if (!src || !dst)
		return (NULL);
	s = (const char *)src;
	d = (char *)dst;
	if (dst == src)
		return (dst);
	i = 0;
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dst);
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*trimmed(char *str, t_garbage *g_c)
{   
    int i = 0;
    int end;
    char *trimmed;

    if (!str)
		return(NULL);
    while (str[i] && (str[i] == ' ' || str[i] == '\t'))
        i++;
    if (str[i] == '\0')
        return(g_strdup(g_c, ""));
    end = ft_strlen(str) - 1;
    while (end >= 0 && (str[end] == ' ' || str[end] == '\t'))
        end--;
    trimmed = g_malloc(g_c, end - i + 2);
    if (!trimmed)
        return (NULL);
    end = end - i + 1;
    ft_memcpy(trimmed, str + i, end);
    trimmed[end] = '\0';
    return (trimmed);
}

char	*ft_strcpy(char *dst, const char *src)
{
	unsigned int	i;

	if (!src || !dst)
		return (NULL);
	if (dst == src)
		return (dst);
	i = 0;
	while (src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

char	*ft_strdup(const char *s)
{
	size_t	i;
	size_t	len;
	char	*p;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	p = (char *)malloc(len + 1);
	if (!p)
		return (NULL);
	i = 0;
	while (i < len)
	{
		p[i] = s[i];
		i++;
	}
	p[i] = '\0';
	return (p);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	size_t	s_len;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len)
		return (ft_strdup(""));
	if (len > s_len - start)
		len = s_len - start;
	sub = (char *)malloc(len + 1);
	if (!sub)
		return (NULL);
	ft_memcpy(sub, s + start, len);
	sub[len] = '\0';
	return (sub);
}

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*a1;
	unsigned char	*a2;

	a1 = (unsigned char *)s1;
	a2 = (unsigned char *)s2;
	while (n--)
	{
		if (*a1 != *a2)
			return (*a1 - *a2);
		a1++;
		a2++;
	}
	return (0);
}

int	ft_isalnumm(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_')
		return (1);
	else
		return (0);
}
