/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abara <banthony@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/13 14:56:07 by abara             #+#    #+#             */
/*   Updated: 2019/12/14 02:34:55 by abara            ###   ########.fr       */
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
	size_t		i;
	t_vertex	vert;
	t_vector	vec;

	i = 0;
	while (i < obj.size[V])
	{
		vert = obj.vertex[i];
		printf("vertex[%zu]: x:%f - y:%f - z:%f\n", i, vert.x, vert.y, vert.z);
		i++;
	}
	i = 0;
	while (i < obj.size[VT])
	{
		vec = obj.textures[i];
		printf("texture[%zu]: x:%f - y:%f - z:%f\n", i, vec.x, vec.y, vec.z);
		i++;
	}
	i = 0;
	while (i < obj.size[VN])
	{
		vec = obj.normals[i];
		printf("normal[%zu]: x:%f - y:%f - z:%f\n", i, vec.x, vec.y, vec.z);
		i++;
	}
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

void	free_obj(t_obj *obj)
{
	if (!obj)
		return ;
	ft_memdel((void**)&obj->vertex);
	ft_memdel((void**)&obj->textures);
	ft_memdel((void**)&obj->normals);
	ft_memdel((void**)&obj->vparam);
	ft_lstdel(&obj->mtllib, free_mtllib);
	ft_strdel(&obj->name);
	obj = NULL;
}
