/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abara <banthony@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/12 12:08:47 by abara             #+#    #+#             */
/*   Updated: 2019/12/13 17:52:37 by abara            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "obj.h"
#include "libft.h"
#include <fcntl.h>
#include <stdlib.h>

t_bool	parse_obj(char *data, off_t size, t_obj *obj)
{
	char	**lines;
	char	*find;
	int		ret;
	if (!data || !obj || !size)
		return (false);
	if (ft_strnstr(data, "\r\n", size))
	{
		ft_putendlcol(SH_YELLOW, "Windows newline will be replace by unix newline.");
		find = data;
		while ((find = ft_strchr(find, '\r')))
			find[0] = ' ';
	}
	lines = ft_strsep(data, '\n');
	//ft_printtab(lines, ft_putstr, "\n.");
	ret = ft_tabiterif(lines, preprocess_obj, obj);
	free(lines);
	return(ret > 0);
}

static int	open_obj(char *path, off_t *size)
{
	struct stat file;
	int fd;

	if (!path || stat(path, &file) < 0)
	{
		ft_putendl("Error with file path name.");
		return (-1);
	}
	ft_putstr(path);
	if (!S_ISREG(file.st_mode))
	{
		ft_putendl(": is not a file.");
		return (-1);
	}
	if (file.st_size == 0)
	{
		ft_putendl(": The file is empty, or corrupted.");
		return (-1);
	}
	if ((fd = open(path, O_RDWR)) < 0)
	{
		ft_putendl(": open has failed.");
		return (-1);
	}
	ft_putendl(": open.");
	*size = file.st_size;
	return (fd);
}

t_bool load_obj(int ac, char **av, t_obj *obj)
{
	int		fd;
	int		ret;
	off_t	size;
	char	*data;
	t_bool	status;

	data = NULL;
	status = false;
	if (ac >= 2 && obj)
		if ((fd = open_obj(av[1], &size)))
		{
			if (!(data = (char*)ft_memalloc(size + 1)))
				ft_putendl("Error during allocation.");
			else if ((ret = read(fd, data, size)) != size)
				ft_putendl("Partial read");
			else
				status = parse_obj(data, size, obj);
		}
	if (fd > 0)
		close(fd);
	if (data)
		ft_memdel((void**)&data);
	return (status);
}
