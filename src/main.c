/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abara <banthony@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/12 10:50:21 by abara             #+#    #+#             */
/*   Updated: 2019/12/13 17:43:08 by abara            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "obj.h"
#include <stdlib.h>

static void scop_usage(char *bin)
{
	ft_putstr(bin);
	ft_putendl(" [.obj][.mtl]");
}

int main(int ac, char **av)
{
	t_obj obj;

	if (ac >= 2)
	{
		ft_memset(&obj, 0, sizeof(obj));
		ft_putendl("Scop pocessing ...");
		if (load_obj(ac, av, &obj))
		{
			ft_putendlcol(SH_GREEN, "OBJ loaded:");
			display_stat_obj(obj);
		}
		free_obj(&obj);
		return (EXIT_SUCCESS);
	}
	scop_usage(av[0]);
	return (EXIT_FAILURE);
}
