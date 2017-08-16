/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/13 11:08:59 by pribault          #+#    #+#             */
/*   Updated: 2017/08/14 10:32:28 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_H
# define VECTOR_H

# include <GL/gl3.h>
# include <math.h>

typedef GLfloat	t_type;

typedef struct	s_vec2
{
	t_type		x;
	t_type		y;
}				t_vec2;

typedef struct	s_vec3
{
	t_type		x;
	t_type		y;
	t_type		z;
}				t_vec3;

typedef struct	s_vec4
{
	t_type		x;
	t_type		y;
	t_type		z;
	t_type		w;
}				t_vec4;

t_vec2			new_vec2(t_type x, t_type y);
t_vec3			new_vec3(t_type x, t_type y, t_type z);
t_vec4			new_vec4(t_type x, t_type y, t_type z, t_type w);

void			normalize_vec2(t_vec2 *vec);
void			normalize_vec3(t_vec3 *vec);
void			normalize_vec4(t_vec4 *vec);

t_vec2			add_vec2(t_vec2 a, t_vec2 b);
t_vec3			add_vec3(t_vec3 a, t_vec3 b);
t_vec4			add_vec4(t_vec4 a, t_vec4 b);

t_vec2			sub_vec2(t_vec2 a, t_vec2 b);
t_vec3			sub_vec3(t_vec3 a, t_vec3 b);
t_vec4			sub_vec4(t_vec4 a, t_vec4 b);

t_vec2			mult_vec2(t_vec2 a, t_vec2 b);
t_vec3			mult_vec3(t_vec3 a, t_vec3 b);
t_vec4			mult_vec4(t_vec4 a, t_vec4 b);

t_type			scalar_vec2(t_vec2 a, t_vec2 b);
t_type			scalar_vec3(t_vec3 a, t_vec3 b);
t_type			scalar_vec4(t_vec4 a, t_vec4 b);

void			debug_vec2(t_vec2 *vec);
void			debug_vec3(t_vec3 *vec);
void			debug_vec4(t_vec4 *vec);

t_vec3			cross_vec3(t_vec3 a, t_vec3 b);

#endif
