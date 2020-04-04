/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parse_element.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abara <banthony@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/19 13:26:30 by abara             #+#    #+#             */
/*   Updated: 2019/12/19 13:28:47 by abara            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "obj.h"
#include <stdio.h>

static t_bool	add_new_mesh(t_obj *obj, t_mesh new)
{
	t_list *tmp;

	if (!(tmp = ft_lstnew(&new, sizeof(new))))
	{
		ft_putendlcol(SH_RED, "Error during mesh allocation.");
		return (false);
	}
	if (!obj->meshes && !obj->flag)
		obj->meshes = tmp;
	else if (obj->meshes && !obj->flag)
		ft_lstaddback(&obj->meshes, tmp);
	obj->flag = true;
	return (true);
}

/*
**	The first memcpy purpose is to keep existing data to modify it later.
**	Useless for now, since we don't do anything if data exist.
*/
t_bool	grouping_and_attributes_parse(t_oelm index, char *line, t_obj *obj)
{
	char	*str;
	t_mesh	new;

	if (!line || !obj || ft_strlen(line) <= get_grmentry(index).len) // FIXME: size compare useless ? already perform by strnequ before this call.
		return (false);
	ft_memset(&new, 0, sizeof(new));
	str = &line[get_grmentry(index).len];
	if (obj->meshes && obj->flag)
		ft_memcpy(&new, (ft_lstlast(obj->meshes))->content, sizeof(new));
	if (index == G_NAME && !new.gname)
		new.gname = ft_strdup(str);
	else if (index == USEMTL && !new.mtl)
		new.mtl = ft_strdup(str);
	if (obj->meshes && obj->flag)
		ft_memcpy(ft_lstlast(obj->meshes)->content, &new, sizeof(new));
	else
		return (add_new_mesh(obj, new));
	return (true);
}

/*
**	This function browse face argv using recursive way.
**
**	f v1//vn1 ...		=> size == 2
**	f v1/vt1/vn1 ...	=> size == 3
**	f v1 v2 v3 ...		=> '/' char not found
*/
static	t_bool	extract_face(char **face_ag, t_face *f, int i)
{
	char		**split;
	int			size;

	if (!f || !face_ag || (i - 1) < 0)
		return (false);
	if (!face_ag[i])
		return (true);
	if (ft_strchr(*face_ag, '/'))
	{
		if (!(split = ft_strsplit(face_ag[i], '/')))
			return (false);
		if ((size = ft_tablen(split)) == 2)
			sscanf(face_ag[i], "%d//%d", &f->v[i - 1][VERTEX], &f->v[i - 1][VERTEX_N]);
		else if (size == 3)
			sscanf(face_ag[i], "%d/%d/%d", &f->v[i - 1][VERTEX], &f->v[i - 1][VERTEX_T], &f->v[i - 1][VERTEX_N]);
		ft_freetab(split);
	}
	else
		sscanf(face_ag[i], "%d", &f->v[i - 1][VERTEX]);
	return (extract_face(face_ag, f, i + 1));
}

t_bool	faces_parse(t_oelm index, char *line, t_obj *obj)
{
	t_list	*tmp;
	t_mesh	*mesh;
	char	**face_ag;
	t_face	face;

	(void)index;
	if (!line || !obj)
		return (false);
	if (!(tmp = ft_lstlast(obj->meshes)))
		return (false);
	if (!(mesh = (t_mesh*)tmp->content))
		return (false);
	if (!(face_ag = ft_strsplit(line, ' ')))
		return (false);
	ft_memset(&face, 0, sizeof(face));
	extract_face(face_ag, &face, 1);
	ft_freetab(face_ag);
	if (!(tmp = ft_lstnew(&face, sizeof(face))))
		return (false);
	if (!mesh->faces)
		mesh->faces = tmp;
	else
		ft_lstaddback(&mesh->faces, tmp);
	return (true);
}
