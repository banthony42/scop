/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abara <banthony@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/12 10:50:21 by abara             #+#    #+#             */
/*   Updated: 2019/12/14 02:17:01 by abara            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "obj.h"
#include <stdlib.h>

static void scop_usage(char *bin)
{
	ft_putstr(bin);
	ft_putendl(" [.obj]");
	ft_putendl("Make sure .mtl and .obj files are in the same folder.");
}

int main(int ac, char **av)
{
	t_obj obj;

	if (ac == 2)
	{
		ft_memset(&obj, 0, sizeof(obj));
		if (load_obj(ac, av, &obj))
		{
			ft_putendlcol(SH_GREEN, "• OBJ successfully loaded.");
			display_vertex(obj);
		}
		else
			ft_putendlcol(SH_RED, "• OBJ loading has failed.");
		free_obj(&obj);
		return (EXIT_SUCCESS);
	}
	scop_usage(av[0]);
	return (EXIT_FAILURE);
}
