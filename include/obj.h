/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abara <banthony@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/12 12:08:07 by abara             #+#    #+#             */
/*   Updated: 2019/12/13 17:55:29 by abara            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		OBJ_H
# define	OBJ_H

#include "utils.h"
#include "libft.h"
#include <string.h>

typedef enum	e_ogrammar
{
	V_GRM,
	VT_GRM,
	VN_GRM,
	VP_GRM,
	F_GRM,
	MTLLIB,
	USEMTL,
	NAME,
	G_NAME,
	SS,
	NB_GRM,
}				t_ogrammar;

typedef struct	s_grm_entry
{
	const char	*grm;
	const char	*comp;
	size_t		len;
}				t_grm_entry;

typedef struct	s_obj
{
	size_t		size[NB_GRM];
	t_list		*mtllib;
	t_list		*faces;
}				t_obj;

/*
**	obj
*/
t_bool			parse_obj(char *data, off_t size, t_obj *obj);
t_bool			load_obj(int ac, char **av, t_obj *obj);

/*
**	obj_parse
*/
int				preprocess_obj(char *line, void *data);

/*
**	obj_utils
*/
void			display_stat_obj(t_obj obj);
char			*get_grm_description(t_ogrammar grm);
void			free_obj(t_obj *obj);


/*
**	OBJ file format content:
**	'#' 	- Comment
**	'v'		- Vertice with x,y,z [w] coordinates, where w is optional. (default:1.0)
**			  Some app support vertex colors by putting R,G,B after x,y,z.
**	'vt'	- Texture coordinate with u, [v], [w] where v and w are optional. (default: 0)
**	'vn'	- Vertex normal with x,y,z
**	'vp'	- Parameter space vertice with u, [v], [w]
**	'f'		- Polygonal face element composed with index of elements above.
**			  f 1 2 3 : define a face that use the first, second and third vertice.
**			  f 1/4 2/5 3/6 : define the same face but with 4th, 5th and 6th of vt list.
**			  f 1/4/7 2/5/8 3/6/9 : define the same face but with normal vectors, 7th, 8th, 9th of vn list.
**			  f 1//7 2//8 3//9 : define the same face but without textures, which are optional.
**			  Conclusion: f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 where all vt are optional but we have to keep the slash.
**	'l'		- Line element, contains vertices.
**
**	Note:	Face or Line are define using index of each element.
**			Positive index mean 1 represent the first defined vertex.
**			Negative index mean -1 represent the latest defined vertex.
**
**	Materials:
**	'mtllib'	- Referencing material file (.mtl) for the obj file.
**	'usemtl'	- Specifie the material name (define in .mtl file) for the next element.
**	'o'			- Name of the object.
**	'g'			- Group name of polygons groups.
**	's'			- Smooth shading across polygons by smoothing groups. (1 or off)
**
**
**	Material Template Library - .mtl files:
**
*/

#endif
