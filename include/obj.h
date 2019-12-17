/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abara <banthony@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/12 12:08:07 by abara             #+#    #+#             */
/*   Updated: 2019/12/17 16:03:41 by abara            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		OBJ_H
# define	OBJ_H

#include "utils.h"
#include "libft.h"
#include <string.h>

# define OBJ_MIN_LINE_LEN sizeof("s 0") - 1

typedef enum	e_oelm
{
	V,
	VT,
	VN,
	VP,
	FACE,
	MTLLIB,
	USEMTL,
	NAME,
	G_NAME,
	SG,
	NB_OELM,
}				t_oelm;

# define NB_VERTEX_ARRAY VP + 1

# define ACSET_MAX 3

typedef int	(*t_grm_check)(char *line);

typedef struct	s_grm_entry
{
	const char	*grm;
	const char	*comp;
	size_t		len;
	int			acset[ACSET_MAX];
}				t_grm_entry;

typedef struct	s_vertex
{
	float		x;
	float		y;
	float		z;
	float		w;
	int			color;
}				t_vertex;

typedef struct	s_vector
{
	float		x;
	float		y;
	float		z;
}				t_vector;

typedef struct	s_face
{
	int			v[4][3];
}				t_face;

typedef struct	s_element
{
	char		*gname;
	char		*mtl;
	int			smoothing_grp;
	t_list		*faces;
}				t_element;

typedef struct	s_obj
{
	char		*name;
	size_t		size[NB_OELM];
	size_t		vertex_off[NB_VERTEX_ARRAY];
	t_list		*mtllib;
	t_vertex	*g_vertex;
	t_vector	*vertex;
	t_list		*elements;
	t_bool		flag;
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
int				extract_data_obj(char *line, void *data);

/*
**	obj_utils
*/
void			display_vertex(t_obj obj);
void			display_stat_obj(t_obj obj);
char			*get_grm_description(t_oelm grm);
void			free_obj(t_obj *obj);

/*
**	obj_check
*/
int				grammar_argc_check(char *line, const int ac_set[ACSET_MAX]);

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
