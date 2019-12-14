/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abara <banthony@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/13 17:46:24 by abara             #+#    #+#             */
/*   Updated: 2019/12/14 03:16:50 by abara            ###   ########.fr       */
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
	void		**array_ptr[NB_VERTEX_ARRAY];
	t_vertex	vert = {0};
	t_vector	vec = {0};
	static int	vi[4] = {0};

	array_ptr[V] = (void**)&obj->vertex;
	array_ptr[VT] = (void**)&obj->textures;
	array_ptr[VN] = (void**)&obj->normals;
	array_ptr[VP] = (void**)&obj->vparam;
	if (ft_strnequ(line, g_ogrammar[V].grm, g_ogrammar[V].len))
	{
		sscanf(&line[g_ogrammar[index].len], "%f%f%f", &vert.x, &vert.y, &vert.z);
		ft_memcpy(*array_ptr[V] + (vi[V] * sizeof(t_vertex)), &vert, sizeof(vert));
		vi[V]++;
		return (true);
	}
	else if (ft_strnequ(line, g_ogrammar[index].grm, g_ogrammar[index].len))
	{
		if (index < 4)
		{
			sscanf(&line[g_ogrammar[index].len], "%f%f%f", &vec.x, &vec.y, &vec.z);
			ft_memcpy(((t_vector*)(*array_ptr[index])) + vi[index], &vec, sizeof(vec));
			vi[index]++;
			return (true);
		}
	}
	return (false);
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
		if (line[0] == 'v')
		{
			if (extract_vertex(index, line, obj))
				break ;
		}
		/*
		**	Si 'usemtl' / 's' / 'g' creation d'un nouveaux t_element
		**	Tant que 'f' ajouter la data au dernier element ajouté
		*/
		index++;
	}
	return (1);
}
