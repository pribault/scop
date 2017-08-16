/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/13 11:36:23 by pribault          #+#    #+#             */
/*   Updated: 2017/08/13 11:38:36 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"

t_vec2	add_vec2(t_vec2 a, t_vec2 b)
{
	return ((t_vec2){a.x + b.x, a.y + b.y});
}

t_vec3	add_vec3(t_vec3 a, t_vec3 b)
{
	return ((t_vec3){a.x + b.x, a.y + b.y, a.z + b.z});
}

t_vec4	add_vec4(t_vec4 a, t_vec4 b)
{
	return ((t_vec4){a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w});
}
