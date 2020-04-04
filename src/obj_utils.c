/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abara <banthony@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/13 14:56:07 by abara             #+#    #+#             */
/*   Updated: 2019/12/19 13:00:32 by abara            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "obj.h"
#include "libft.h"
#include <stdio.h>

static const	char		*grm_description[NB_OELM + 1] =
{
	[VERTEX] = "Vertex",
	[VERTEX_T] = "Texture Vertex",
	[VERTEX_N] = "Vertex normal",
	[VERTEX_P] = "Parameter space vertex",
	[FACE] = "Faces",
	[MTLLIB] = "Reference to .mtl file",
	[USEMTL] = "Use .mtl file",
	[NAME] = "Object name",
	[G_NAME] = "Group name",
	[SG] = "Smoothing group information",
	[NB_OELM] = NULL,
};

char *get_grm_description(t_oelm grm)
{
	if (grm < VERTEX || grm >= NB_OELM)
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
		  */	printf("\n\t* MESHES:\n");
	t_list *tmp = obj.meshes;
	t_list *faces;
	t_mesh *mesh;
	t_face *face;
	int f = 0;
	int e = 0;
	while (tmp)
	{
		mesh = (t_mesh*)tmp->content;
		faces = mesh->faces;
		f = 0;
		printf("MESH[%d]:\nName:\t%s\nS:\t%d\nmtl:\t%s\n", e, mesh->gname,
			   mesh->smoothing_grp, mesh->mtl);
		while (faces)
		{
			face = (t_face*)faces->content;
			printf("Face[%d]:%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", f,
				   face->v[0][VERTEX], face->v[0][VERTEX_T], face->v[0][VERTEX_N],
				   face->v[1][VERTEX], face->v[1][VERTEX_T], face->v[1][VERTEX_N],
				   face->v[2][VERTEX], face->v[2][VERTEX_T], face->v[2][VERTEX_N],
				   face->v[3][VERTEX], face->v[3][VERTEX_T], face->v[3][VERTEX_N]);
			f++;
			faces = faces->next;
		}
		e++;
		tmp = tmp->next;
	}
	printf("\nNb mesh:%zu\n", ft_lstlen(obj.meshes));
}

void	display_stat_obj(t_obj obj)
{
	t_oelm	index;

	index = VERTEX;
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

static	void free_meshes(void *data, size_t size)
{
	(void)size;
	t_mesh *mesh = (t_mesh*)data;
	ft_strdel(&mesh->gname);
	ft_strdel(&mesh->mtl);
	ft_lstdel(&mesh->faces, free_faces);
	ft_memdel(&data);
}

void	free_obj(t_obj *obj)
{
	if (!obj)
		return ;
	ft_memdel((void**)&obj->vertex);
	ft_memdel((void**)&obj->g_vertex);
	ft_lstdel(&obj->mtllib, free_mtllib);
	ft_lstdel(&obj->meshes, free_meshes);
	ft_strdel(&obj->name);
	obj = NULL;
}
