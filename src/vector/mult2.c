/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mult2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/27 20:43:45 by pribault          #+#    #+#             */
/*   Updated: 2018/06/27 20:46:49 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"

t_vec2	mult2_vec2(t_vec2 a, t_type b)
{
	return ((t_vec2){a.x * b, a.y * b});
}

t_vec3	mult2_vec3(t_vec3 a, t_type b)
{
	return ((t_vec3){a.x * b, a.y * b, a.z * b});
}

t_vec4	mult2_vec4(t_vec4 a, t_type b)
{
	return ((t_vec4){a.x * b, a.y * b, a.z * b, a.w * b});
}
