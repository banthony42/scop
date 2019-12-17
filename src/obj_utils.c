/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abara <banthony@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/13 14:56:07 by abara             #+#    #+#             */
/*   Updated: 2019/12/17 16:14:33 by abara            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "obj.h"
#include "libft.h"
#include <stdio.h>

static const	char		*grm_description[NB_OELM + 1] =
{
	[V] = "Vertex",
	[VT] = "Texture Vertex",
	[VN] = "Vertex normal",
	[VP] = "Parameter space vertex",
	[FACE] = "Faces",
	[NB_OELM] = NULL,
};

char *get_grm_description(t_oelm grm)
{
	if (grm < V || grm >= NB_OELM)
		return (NULL);
	return ((char*)grm_description[grm]);
}

void	display_vertex(t_obj obj)
{
	/*	size_t		i;
	t_vertex	vert;
	t_vector	vec;

	i = 0;
	while (i < obj.size[V])
	{
		vert = obj.g_vertex[i];
		printf("vertex[%zu]: x:%f - y:%f - z:%f\n", i, vert.x, vert.y, vert.z);
		i++;
	}
	i = 0;
	while (i < obj.size[VT])
	{
		vec = obj.vertex[i];
		printf("texture[%zu]: x:%f - y:%f - z:%f\n", i, vec.x, vec.y, vec.z);
		i++;
	}
	i = 0;
	while (i < obj.size[VN])
	{
		vec = obj.vertex[obj.size[VT] + i];
		printf("normal[%zu]: x:%f - y:%f - z:%f\n", i, vec.x, vec.y, vec.z);
		i++;
	}
		  */	printf("\n\t* ELEMENTS:\n");
	t_list *tmp = obj.elements;
	t_list *faces;
	t_element *elem;
	t_face *face;
	int f = 0;
	int e = 0;
	while (tmp)
	{
		elem = (t_element*)tmp->content;
		faces = elem->faces;
		f = 0;
		printf("ELEM[%d]:\nName:\t%s\nS:\t%d\nmtl:\t%s\n", e, elem->gname,
			   elem->smoothing_grp, elem->mtl);
		while (faces)
		{
			face = (t_face*)faces->content;
			printf("Face[%d]:%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", f,
				   face->v[0][V], face->v[0][VT], face->v[0][VN],
				   face->v[1][V], face->v[1][VT], face->v[1][VN],
				   face->v[2][V], face->v[2][VT], face->v[2][VN],
				   face->v[3][V], face->v[3][VT], face->v[3][VN]);
			f++;
			faces = faces->next;
		}
		e++;
		tmp = tmp->next;
	}
	printf("\nNb elements:%zu\n", ft_lstlen(obj.elements));
}

void	display_stat_obj(t_obj obj)
{
	t_oelm	index;

	index = V;
	if (obj.name)
		printf("• Name :%s\n", obj.name);
	while(grm_description[index])
	{
		printf("• %s:%zu\n", grm_description[index], obj.size[index]);
		index++;
	}
	if (obj.mtllib)
		printf("• MTL files references:\n");
	while (obj.mtllib)
	{
		printf("\t%s\n", (char*)obj.mtllib->content);
		obj.mtllib = obj.mtllib->next;
	}
}

static	void free_mtllib(void *data, size_t size)
{
	(void)size;
	ft_memdel(&data);
}

static	void free_faces(void *data, size_t size)
{
	(void)size;
	ft_memdel(&data);
}

static	void free_elements(void *data, size_t size)
{
	(void)size;
	t_element *elem = (t_element*)data;
	ft_strdel(&elem->gname);
	ft_strdel(&elem->mtl);
	ft_lstdel(&elem->faces, free_faces);
	ft_memdel(&data);
}

void	free_obj(t_obj *obj)
{
	if (!obj)
		return ;
	ft_memdel((void**)&obj->vertex);
	ft_memdel((void**)&obj->g_vertex);
	ft_lstdel(&obj->mtllib, free_mtllib);
	ft_lstdel(&obj->elements, free_elements);
	ft_strdel(&obj->name);
	obj = NULL;
}
