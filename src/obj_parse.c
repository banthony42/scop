/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abara <banthony@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/13 17:46:24 by abara             #+#    #+#             */
/*   Updated: 2019/12/13 18:22:31 by abara            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "obj.h"
#include "libft.h"
#include <stdio.h>

/*
**	This array define the content of an .obj file.
**	'grm'  : obj file format instruction.
**	'len'  : size of the grm field.
**	'comp' : Composition / charset for this instruction. (NULL when no restrict)
*/

static const	t_grm_entry g_obj_grammar[NB_GRM] =
{
	[V_GRM] = {.grm = "v ", .len = sizeof("v ") - 1, .comp = "v -.0123456789" },
	[VT_GRM] = {.grm = "vt ", .len = sizeof("vt ") - 1, .comp = "vt -.0123456789" },
	[VN_GRM] = {.grm = "vn ", .len = sizeof("vn ") - 1, .comp = "vn -.0123456789" },
	[VP_GRM] = {.grm = "vp ", .len = sizeof("vp ") - 1, .comp = "vp -.0123456789" },
	[F_GRM] = {.grm = "f ", .len = sizeof("f ") - 1, .comp = "f -/0123456789" },
	[MTLLIB] = {.grm = "mtllib ", .len = sizeof("mtllib ") - 1, .comp = NULL},
	[USEMTL] = {.grm = "usemtl ", .len = sizeof("usemtl ") - 1, .comp = NULL},
	[NAME] = {.grm = "o ", .len = sizeof("o ") - 1, .comp = NULL},
	[G_NAME] = {.grm = "g ", .len = sizeof("g ") - 1, .comp = NULL},
	[SS] = {.grm = "s ", .len = sizeof("s ") - 1, .comp = "s 0123456789off"},
};

static int		special_entry(t_ogrammar index, t_obj *obj, char *line)
{
	char	*content;
	size_t	size;

	if (index == MTLLIB)
	{
		content = &line[g_obj_grammar[index].len];
		// Add 1 to also copy the '\0'
		size = ft_strlen(content) + 1;
		if (!obj->mtllib)
			obj->mtllib = ft_lstnew(content, size);
		else
			ft_lstadd(&obj->mtllib, ft_lstnew(content, size));
	}
	else if (index == USEMTL){}
	else if (index == NAME) {}
	else if (index == G_NAME) {}
	return (1);
}

int preprocess_obj(char *line, void *data)
{
	t_obj		*obj;
	t_ogrammar	index;
	size_t		len;

	if (!line || !(obj = (t_obj*)data))
		return (-1);
	len = ft_strlen(line);
	if (line[0] == '#' || len < 3)
		return (1);
	index = V_GRM;
	while (index < NB_GRM)
	{
		if (ft_strnequ(line, g_obj_grammar[index].grm, g_obj_grammar[index].len))
		{
			if (!g_obj_grammar[index].comp)
				return (special_entry(index, obj, line));
			else if (strspn(line, g_obj_grammar[index].comp) == len)
			{
				obj->size[index]++;
				return (1);
			}
			printf("\n• Grammar error:[%s]\n  %s grammar can only contain:[%s]\n\n",
				   line, get_grm_description(index), g_obj_grammar[index].comp);
			return (-1);
		}
		index++;
	}
	printf("\n• Unknow grammar entry:[%s]\n", line);
	return (-1);
}
