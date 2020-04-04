/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abara <banthony@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/13 17:46:24 by abara             #+#    #+#             */
/*   Updated: 2019/12/19 13:05:32 by abara            ###   ########.fr       */
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

static const	t_grm_entry g_ogrammar[NB_OELM] =
{
	[VERTEX] = {
		.grm = "v ",
		.len = sizeof("v ") - 1,
		.comp = "v -.0123456789",
		.acset = {3, 4, 6},
		.parser = geometric_vertex_parse,
	},
	[VERTEX_T] = {
		.grm = "vt ",
		.len = sizeof("vt ") - 1,
		.comp = "vt -.0123456789",
		.acset = {2, 3},
		.parser = vertex_parse,
	},
	[VERTEX_N] = {
		.grm = "vn ",
		.len = sizeof("vn ") - 1,
		.comp = "vn -.0123456789",
		.acset = {3},
		.parser = vertex_parse,
	},
	[VERTEX_P] = {
		.grm = "vp ",
		.len = sizeof("vp ") - 1,
		.comp = "vp -.0123456789",
		.acset = {3},
		.parser = vertex_parse,
	},
	[FACE] = {
		.grm = "f ",
		.len = sizeof("f ") - 1,
		.comp = "f -/0123456789",
		.acset = {3, 4},
		.parser = faces_parse,
	},
	[MTLLIB] = {
		.grm = "mtllib ",
		.len = sizeof("mtllib ") - 1,
		.comp = NULL,
		.acset = {1},
		.parser = NULL,
	},
	[USEMTL] = {
		.grm = "usemtl ",
		.len = sizeof("usemtl ") - 1,
		.comp = NULL,
		.acset = {1},
		.parser = grouping_and_attributes_parse,
	},
	[NAME] = {
		.grm = "o ",
		.len = sizeof("o ") - 1,
		.comp = NULL,
		.acset = {1},
		.parser = NULL,
	},
	[G_NAME] = {
		.grm = "g ",
		.len = sizeof("g ") - 1,
		.comp = NULL,
		.acset = {1},
		.parser = grouping_and_attributes_parse,
	},
	[SG] = {
		.grm = "s ",
		.len = sizeof("s ") - 1,
		.comp = "s 0123456789off",
		.acset = {1},
		.parser = NULL,
	},
};

t_grm_entry		get_grmentry(t_oelm index)
{
	if (index >= VERTEX && index < NB_OELM)
		return (g_ogrammar[index]);
	return (g_ogrammar[VERTEX]);
}

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

/*
**	This function verify the grammar of the obj file.
**	During this process, the number of instruction is count for each type of instruction,
**	also, the name and external files references are stored.
*/
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
	index = VERTEX;
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
	index = VERTEX;
	while (index < NB_OELM)
	{
		if (ft_strnequ(line, g_ogrammar[index].grm, g_ogrammar[index].len))
			if (g_ogrammar[index].parser)
			{
				if (!g_ogrammar[index].parser(index, line, obj))
					printf("Error during parsing:\nLine: [%s]", line);
				break ;
			}
		index++;
	}
	return (1);
}
