/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/13 11:32:23 by pribault          #+#    #+#             */
/*   Updated: 2017/08/13 11:34:36 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "matrix.h"

void	clean_mat2(t_mat2 *mat)
{
	mat->x = new_vec2(0, 0);
	mat->y = new_vec2(0, 0);
}

void	clean_mat3(t_mat3 *mat)
{
	mat->x = new_vec3(0, 0, 0);
	mat->y = new_vec3(0, 0, 0);
	mat->z = new_vec3(0, 0, 0);
}

void	clean_mat4(t_mat4 *mat)
{
	mat->x = new_vec4(0, 0, 0, 0);
	mat->y = new_vec4(0, 0, 0, 0);
	mat->z = new_vec4(0, 0, 0, 0);
	mat->w = new_vec4(0, 0, 0, 0);
}
