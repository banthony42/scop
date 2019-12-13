/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abara <banthony@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/13 14:56:07 by abara             #+#    #+#             */
/*   Updated: 2019/12/13 17:59:38 by abara            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "obj.h"
#include "libft.h"
#include <stdio.h>

static const	char		*grm_description[NB_GRM + 1] =
{
	[V_GRM] = "Vertex",
	[VT_GRM] = "Texture Vertex",
	[VN_GRM] = "Vertex normal",
	[VP_GRM] = "Parameter space vertex",
	[F_GRM] = "Faces",
	[NB_GRM] = NULL,
};

char *get_grm_description(t_ogrammar grm)
{
	if (grm < V_GRM || grm >= NB_GRM)
		return (NULL);
	return ((char*)grm_description[grm]);
}

void	display_stat_obj(t_obj obj)
{
	t_ogrammar	index;

	index = V_GRM;
	printf("• OBJ composition:\n");
	while(grm_description[index])
	{
		printf("%s:%zu\n", grm_description[index], obj.size[index]);
		index++;
	}
	if (obj.mtllib)
		printf("\n• MTL files references:\n");
	while (obj.mtllib)
	{
		printf("%s\n", (char*)obj.mtllib->content);
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
	ft_lstdel(&obj->mtllib, free_mtllib);
}
