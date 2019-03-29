/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ortho.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/13 11:02:11 by pribault          #+#    #+#             */
/*   Updated: 2017/08/13 11:31:18 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "matrix.h"

t_mat4		ortho(t_vec2 x, t_vec2 y, t_vec2 z)
{
	t_mat4	result;

	set_id_mat4(&result, 1);
	result.x.x = 2 / (x.y - x.x);
	result.y.y = 2 / (y.y - y.y);
	result.z.z = -2 / (z.y - z.x);
	result.w.x = -(x.y + x.x) / (x.y - x.x);
	result.w.y = -(y.y + y.x) / (y.y - y.x);
	result.w.z = -(z.y + z.x) / (z.y - z.x);
	return (result);
}
