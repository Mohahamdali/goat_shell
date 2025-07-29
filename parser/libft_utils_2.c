#include "../minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	unsigned int	i;
	unsigned char	*a1;
	unsigned char	*a2;

	a1 = (unsigned char *)s1;
	a2 = (unsigned char *)s2;
	if (!a1 && !a2)
		return (0);
	i = 0;
	while (a1[i] || a2[i])
	{
		if (a1[i] != a2[i])
			return (a1[i] - a2[i]);
		i++;
	}
	return (0);
}

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char		*d;
	unsigned const char	*s;

	if (dst == src)
		return (dst);
	d = (unsigned char *)dst;
	s = (unsigned const char *)src;
	if (d < s)
	{
		while (len--)
			*d++ = *s++;
	}
	else if (d > s)
	{
		d += len - 1;
		s += len - 1;
		while (len--)
			*(d--) = *(s--);
	}
	return (dst);
}

static int	nbr_of_digits(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
	{
		len++;
		if (n < 0)
			n = -n;
	}
	while (n > 0)
	{
		n = n / 10;
		len++;
	}
	return (len);
}

static	char	*alloc(int n)
{
	int		len;
	char	*str;

	if (n == 0)
		return (ft_strdup("0"));
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	len = nbr_of_digits(n);
	str = (char *)malloc(len + 1);
	if (!str)
		return (NULL);
	str[len] = '\0';
	return (str);
}

char	*ft_itoa(int n, t_garbage *g_c)
{
	char	*str;
	int		len;
	int		i;
	int		sign;

	len = nbr_of_digits(n);
	str = alloc(n);
	if (!str)
		return (cleanup_grb_cltr(g_c), exit(1), NULL);
	if (n < 0)
	{
		sign = -1;
		n = -n;
	}
	else
		sign = 1;
	i = len - 1;
	while (n > 0)
	{
		str[i--] = (n % 10) + '0';
		n /= 10;
	}
	if (sign == -1)
		str[0] = '-';
	return (str);
}
