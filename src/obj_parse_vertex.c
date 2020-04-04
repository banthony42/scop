/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parse_vertex.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abara <banthony@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/19 13:26:10 by abara             #+#    #+#             */
/*   Updated: 2019/12/19 13:26:58 by abara            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "obj.h"
#include <stdio.h>

// TODO make function to handle differen format fot geometric vertex
t_bool	geometric_vertex_parse(t_oelm index, char *line, t_obj *obj)
{
	static int	vi = 0;
	t_grm_entry	grm_entry;
	void		*dest;
	t_vertex	vert;

	if (!line || !obj)
		return (false);
	if (index != VERTEX)
		printf("\n• Extract error: Trying to extract geometric vertex on [%s] instruction.\nline:%s\n", get_grm_description(index), line);
	ft_memset(&vert, 0, sizeof(vert));
	grm_entry = get_grmentry(index);
	sscanf(&line[grm_entry.len], "%f%f%f", &vert.x, &vert.y, &vert.z);
	dest = obj->g_vertex + vi;
	ft_memcpy(dest, &vert, sizeof(vert));
	vi++;
	return (true);
}

/*
**	This function extract data for all vertex instructions.
**	There is one big array of vertex to fill, and a vertex_offset give us the starting point in the array for each type of vertex.
**	(obj->vertex = [VERTEX_T DATA][VERTEX_N DATA][VERTEX_P DATA] sequence of data)
**	A static variable memory, index the position of the array for the next memcpy.
*/
t_bool	vertex_parse(t_oelm index, char *line, t_obj *obj)
{
	static int	vi[NB_VERTEX_ARRAY] = {0};
	t_grm_entry	grm_entry;
	void		*dest;
	t_vector	vec;

	if (!line || !obj)
		return (false);
	if (index == 0 || index >= NB_VERTEX_ARRAY)
		printf("\n• Extract error: Trying to extract vertex on [%s] instruction.\nline:%s\n", get_grm_description(index), line);
	ft_memset(&vec, 0, sizeof(vec));
	grm_entry = get_grmentry(index);
	sscanf(&line[grm_entry.len], "%f%f%f", &vec.x, &vec.y, &vec.z);
	dest = (obj->vertex + obj->vertex_offset[index]) + vi[index];
	ft_memcpy(dest, &vec, sizeof(vec));
	vi[index]++;
	return (true);
}
