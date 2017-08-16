/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mult.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/13 11:40:32 by pribault          #+#    #+#             */
/*   Updated: 2017/08/13 11:42:28 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"

t_vec2	mult_vec2(t_vec2 a, t_vec2 b)
{
	return ((t_vec2){a.x * b.x, a.y * b.y});
}

t_vec3	mult_vec3(t_vec3 a, t_vec3 b)
{
	return ((t_vec3){a.x * b.x, a.y * b.y, a.z * b.z});
}

t_vec4	mult_vec4(t_vec4 a, t_vec4 b)
{
	return ((t_vec4){a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w});
}
