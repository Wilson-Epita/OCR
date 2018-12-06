#include "network.h"


int		ft_get_line(char **str, char **line, int fd, int ret)
{
	char	*tmp;
	int		len;

	len = 0;
	while (str[fd][len] != '\n' && str[fd][len])
		len++;
	if (str[fd][len] == '\n')
	{
		*line = ft_strsub(str[fd], 0, len);
		tmp = ft_strdup(str[fd] + len + 1);
		free(str[fd]);
		str[fd] = tmp;
		if (!&str[fd])
			ft_strdel(&str[fd]);
	}
	else if (!str[fd][len])
	{
		if (ret == BUFF_SIZE)
			return (get_next_line(fd, line));
		*line = ft_strdup(str[fd]);
		ft_strdel(&str[fd]);
	}
	return (1);
}

int				get_next_line(const int fd, char **line)
{
	static char	*str[FD + 1];
	char		buf[BUFF_SIZE + 1];
	int			ret;

	if (!line || fd < 0 || FD < 0 || BUFF_SIZE < 0)
		return (-1);
	while ((ret = read(fd, buf, BUFF_SIZE)))
	{
		if (ret == -1)
			return (-1);
		buf[ret] = '\0';
		!str[fd] ? str[fd] = ft_strnew(1) : 0;
		str[fd] = ft_strfjoin(str[fd], buf);
		if (ft_strchr(str[fd], '\n'))
			break ;
	}
	return ((!ret && (!str[fd] || !*str[fd])) ?
			0 : ft_get_line(str, line, fd, ret));
}
