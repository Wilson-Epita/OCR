#include "network.h"

char	*ft_strfjoin(const char *s1, const char *s2)
{
	char	*new;

	if (!s1 || !s2 || !(new = ft_strjoin(s1, s2)))
		return (NULL);
	free((void*)s1);
	return (new);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*tmp;
	size_t	x;
	size_t	y;
	size_t	len_s1;
	size_t	len_s2;

	if (!s1 || !s2)
		return (NULL);
	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	if (!(tmp = ft_strnew(len_s1 + len_s2)))
		return (NULL);
	x = -1;
	y = -1;
	while (++x < len_s1)
		tmp[x] = s1[x];
	while (++y < len_s2)
		tmp[x++] = s2[y];
	return (tmp);
}

char	*ft_strsub(char const *s, unsigned int start, size_t len)
{
	char	*tmp;
	size_t	x;

	if (!s || !(tmp = ft_strnew(len)))
		return (NULL);
	x = -1;
	while (++x < len)
		tmp[x] = s[start++];
	return (tmp);
}

char	*ft_strcpy(char *dst, const char *src)
{
	int i;

	i = 0;
	while (src[i] != '\0')
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

size_t	ft_strlen(const char *s)
{
	size_t len;

	len = 0;
	while (s[len] != '\0')
		len++;
	return (len);
}

char		*ft_strnew(size_t size)
{
	char	*str;

	if ((str = (char*)malloc(sizeof(char) * (size + 1))) == 0)
		return (NULL);
	while ((int)size-- >= 0)
		*(str + size + 1) = '\0';
	return (str);
}

char	*ft_strdup(const char *s1)
{
	char	*dest;

	if ((dest = (char*)malloc(sizeof(char) * (ft_strlen(s1) + 1))) == 0)
		return (NULL);
	ft_strcpy(dest, s1);
	return (dest);
}

char	*ft_strchr(const char *str, int c)
{
	int		x;
	char	*tmp;
	int		len_str;

	x = -1;
	tmp = (char*)str;
	len_str = ft_strlen(tmp);
	while (++x < (len_str + 1))
		if (tmp[x] == (char)c)
			return (&tmp[x]);
	return (NULL);
}

void	ft_strdel(char **as)
{
	if (!as || !*as)
		return ;
	free(*as);
	*as = 0;
}
