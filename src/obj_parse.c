/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abara <banthony@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/13 17:46:24 by abara             #+#    #+#             */
/*   Updated: 2019/12/17 16:19:29 by abara            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "obj.h"
#include "libft.h"
#include <stdio.h>

#include <stdlib.h>
/*
**	This array define the content of an .obj file.
**	'grm'  : obj file format instruction.
**	'len'  : size of the grm field.
**	'comp' : Composition / charset for this instruction. (NULL when no restrict)
*/

static const	t_grm_entry g_ogrammar[NB_OELM] =
{
	[V] = {
		.grm = "v ",
		.len = sizeof("v ") - 1,
		.comp = "v -.0123456789",
		.acset = {3, 4, 6},
	},
	[VT] = {
		.grm = "vt ",
		.len = sizeof("vt ") - 1,
		.comp = "vt -.0123456789",
		.acset = {2, 3},
	},
	[VN] = {
		.grm = "vn ",
		.len = sizeof("vn ") - 1,
		.comp = "vn -.0123456789",
		.acset = {3},
	},
	[VP] = {
		.grm = "vp ",
		.len = sizeof("vp ") - 1,
		.comp = "vp -.0123456789",
		.acset = {3},
	},
	[FACE] = {
		.grm = "f ",
		.len = sizeof("f ") - 1,
		.comp = "f -/0123456789",
		.acset = {3, 4},
	},
	[MTLLIB] = {
		.grm = "mtllib ",
		.len = sizeof("mtllib ") - 1,
		.comp = NULL,
		.acset = {1},
	},
	[USEMTL] = {
		.grm = "usemtl ",
		.len = sizeof("usemtl ") - 1,
		.comp = NULL,
		.acset = {1},
	},
	[NAME] = {
		.grm = "o ",
		.len = sizeof("o ") - 1,
		.comp = NULL,
		.acset = {1},
	},
	[G_NAME] = {
		.grm = "g ",
		.len = sizeof("g ") - 1,
		.comp = NULL,
		.acset = {1},
	},
	[SG] = {
		.grm = "s ",
		.len = sizeof("s ") - 1,
		.comp = "s 0123456789off",
		.acset = {1},
	},
};

static int		special_entry(t_oelm index, t_obj *obj, char *line)
{
	char	*content;
	size_t	size;

	content = &line[g_ogrammar[index].len];
	// Add 1 to the size to also copy the '\0'
	size = ft_strlen(content) + 1;
	if (index == MTLLIB)
	{
		if (!obj->mtllib)
			obj->mtllib = ft_lstnew(content, size);
		else
			ft_lstadd(&obj->mtllib, ft_lstnew(content, size));
	}
	else if (index == NAME)
	{
		obj->name = ft_strnew(size);
		ft_strncpy(obj->name, content, size);
	}
	return (1);
}

int preprocess_obj(char *line, void *data)
{
	t_obj		*obj;
	t_oelm		index;
	size_t		len;

	if (!line || !(obj = (t_obj*)data))
		return (-1);
	len = ft_strlen(line);
	if (line[0] == '#' || len < OBJ_MIN_LINE_LEN)
		return (1);
	index = V;
	while (index < NB_OELM)
	{
		if (ft_strnequ(line, g_ogrammar[index].grm, g_ogrammar[index].len))
		{
			if (!g_ogrammar[index].comp)
				return (special_entry(index, obj, line));
			else if (strspn(line, g_ogrammar[index].comp) == len)
			{
				obj->size[index]++;
				return (grammar_argc_check(line, g_ogrammar[index].acset));
			}
			printf("\n• Grammar error:[%s]\n  %s grammar can only contain:[%s]\n\n",
				   line, get_grm_description(index), g_ogrammar[index].comp);
			return (-1);
		}
		index++;
	}
	printf("\n• Unknow grammar entry:[%s]\n", line);
	return (-1);
}

static t_bool	extract_vertex(t_oelm index, char *line, t_obj *obj)
{
	t_vertex	vert = {0};
	t_vector	vec = {0};
	static int	vi[4] = {0};
	void		*dest;

	if (!obj || !line)
		return (false);
	if (index == V)
	{
		// TODO make function to handle differen format fot geometric vertex
		sscanf(&line[g_ogrammar[index].len], "%f%f%f", &vert.x, &vert.y, &vert.z);
		dest = obj->g_vertex + vi[V];
		ft_memcpy(dest, &vert, sizeof(vert));
		vi[V]++;
		return (true);
	}
	else if (index < 4)
	{
		sscanf(&line[g_ogrammar[index].len], "%f%f%f", &vec.x, &vec.y, &vec.z);
		dest = (obj->vertex + obj->vertex_off[index]) + vi[index];
		ft_memcpy(dest, &vec, sizeof(vec));
		vi[index]++;
		return (true);
	}
	return (false);
}

static t_bool	create_newelem(t_oelm index, char *line, t_obj *obj)
{
	t_element	new;
	char		*str;

	if (!obj || !line)
		return (false);
	if (ft_strlen(line) <= g_ogrammar[index].len)
		return (false);
	str = &line[g_ogrammar[index].len];
 	ft_memset(&new, 0, sizeof(t_element));
	// Copy current element to modify it
	if (obj->elements && obj->flag)
		ft_memcpy(&new, (ft_lstlast(obj->elements))->content, sizeof(t_element));
	if (index == G_NAME)
	{
		ft_strdel(&new.gname);
		new.gname = ft_strdup(str);
	}
	else if (index == USEMTL)
	{
		ft_strdel(&new.mtl);
		new.mtl = ft_strdup(str);
	}

	// Create new element (the first)
	if (!obj->elements && !obj->flag)
	{
		obj->elements = ft_lstnew(&new, sizeof(t_element));
		obj->flag = true;
	}
	// Create new element (not the first)
	else if (obj->elements && !obj->flag)
	{
		ft_lstaddback(&obj->elements, ft_lstnew(&new, sizeof(t_element)));
		obj->flag = true;
	}
	// Modify current element
	else if (obj->elements && obj->flag)
		ft_memcpy(ft_lstlast(obj->elements)->content, &new, sizeof(t_element));
	return (true);
}

// f 1/2/3 4/5/6 7/8/9
// f 1//3 4//6 7//9
// f 1 2 3
static t_bool	add_to_element(char *line, t_obj *obj)
{
	t_face		face;
	t_list		*last;
	t_element	*element;
	char		**face_args;
	int			i;

	if (!obj || !line)
		return (false);
	obj->flag = false;
	last = ft_lstlast(obj->elements);
	element = (t_element*)last->content;
	face_args = ft_strsplit(line, ' ');
	i = 1;
	ft_memset(&face, 0, sizeof(face));
	char **sep;
	int nsep;
	while(face_args[i])
	{
		if (ft_strchr(face_args[i], '/'))
		{
			sep = ft_strsplit(face_args[i], '/');
			// geometric vertex + normal vertex
			if ((nsep = ft_tablen(sep)) == 2)
				sscanf(face_args[i], "%d//%d", &face.v[i - 1][V], &face.v[i - 1][VN]);
			// g. vertex + texture vertex + n. vertex
			else if (nsep == 3)
				sscanf(face_args[i], "%d/%d/%d", &face.v[i - 1][V], &face.v[i - 1][VT], &face.v[i - 1][VN]);
			ft_freetab(sep);
		}
		// Only g. vertex
		else
			sscanf(face_args[i], "%d", &face.v[i - 1][V]);
		i++;
	}
	ft_freetab(face_args);
	if (!element->faces)
		element->faces = ft_lstnew(&face, sizeof(t_face));
	else
		ft_lstaddback(&element->faces, ft_lstnew(&face, sizeof(t_face)));
	return (true);
}

int	extract_data_obj(char *line, void *data)
{
	t_obj		*obj;
	size_t		len;
	t_oelm		index;

	if (!line || !(obj = (t_obj*)data))
		return (-1);
	len = ft_strlen(line);
	if (line[0] == '#' || len < OBJ_MIN_LINE_LEN)
		return (1);
	index = V;
	while (index < NB_OELM)
	{
		if (ft_strnequ(line, g_ogrammar[index].grm, g_ogrammar[index].len))
		{
			if (index == V || index == VT || index == VN || index == VP)
				if (extract_vertex(index, line, obj))
					break ;
			if (index == G_NAME || index == USEMTL)
				if (create_newelem(index, line, obj))
					break;
			if (index == FACE)
				if (add_to_element(line, obj))
					break ;
		}
		index++;
	}
	return (1);
}
