/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/13 10:01:48 by pribault          #+#    #+#             */
/*   Updated: 2017/08/13 15:35:48 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATRIX_H
# define MATRIX_H

# include "vector.h"

typedef struct	s_mat2
{
	t_vec2		x;
	t_vec2		y;
}				t_mat2;

typedef struct	s_mat3
{
	t_vec3		x;
	t_vec3		y;
	t_vec3		z;
}				t_mat3;

typedef struct	s_mat4
{
	t_vec4		x;
	t_vec4		y;
	t_vec4		z;
	t_vec4		w;
}				t_mat4;

t_mat2			new_mat2(t_vec2 x, t_vec2 y);
t_mat3			new_mat3(t_vec3 x, t_vec3 y, t_vec3 z);
t_mat4			new_mat4(t_vec4 x, t_vec4 y, t_vec4 z, t_vec4 w);

void			clean_mat2(t_mat2 *mat);
void			clean_mat3(t_mat3 *mat);
void			clean_mat4(t_mat4 *mat);

void			set_id_mat2(t_mat2 *mat, t_type id);
void			set_id_mat3(t_mat3 *mat, t_type id);
void			set_id_mat4(t_mat4 *mat, t_type id);

t_mat2			mat_2_mat(t_mat2 a, t_mat2 b);
t_mat3			mat_3_mat(t_mat3 a, t_mat3 b);
t_mat4			mat_4_mat(t_mat4 a, t_mat4 b);

t_vec2			mat_2_vec(t_mat2 mat, t_vec2 vec);
t_vec3			mat_3_vec(t_mat3 mat, t_vec3 vec);
t_vec4			mat_4_vec(t_mat4 mat, t_vec4 vec);

void			debug_mat2(t_mat2 *mat);
void			debug_mat3(t_mat3 *mat);
void			debug_mat4(t_mat4 *mat);

t_mat4			lookat(t_vec3 eye, t_vec3 center, t_vec3 up);
t_mat4			perspective(t_type fy, t_type a, t_type zn, t_type zf);

#endif
