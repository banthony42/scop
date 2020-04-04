/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abara <banthony@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/14 00:16:19 by abara             #+#    #+#             */
/*   Updated: 2019/12/14 03:20:24 by abara            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "obj.h"
#include "libft.h"
#include <stdio.h>

/*
**	This function count the number of word in line, delimited by space.
**	The number of word / arguments are compare with ac_set array.
**	If the number of word match with one of integer in ac_set,
**	the function return true, false otherwise.
*/

int	grammar_argc_check(char *line, const int ac_set[ACSET_MAX])
{
	char	*find;
	size_t	i;
	int		n;

	n = 0;
	if (!line)
		return (-1);
	if (!ac_set)
		return (1);
	find = line;
	while ((find = ft_strchr(find, ' ')))
	{
		find++;
		n++;
	}
	i = 0;
	while (i < ACSET_MAX)
		if (ac_set[i++] == n)
			return (1);
	printf("\n• Grammar error:[%s]\n Vertex has the wrong number of argument.\n", line);
	return (-1);
}
