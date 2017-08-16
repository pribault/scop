/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/13 11:14:04 by pribault          #+#    #+#             */
/*   Updated: 2017/08/13 11:18:57 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"

t_vec2	new_vec2(t_type x, t_type y)
{
	return ((t_vec2){x, y});
}

t_vec3	new_vec3(t_type x, t_type y, t_type z)
{
	return ((t_vec3){x, y, z});
}

t_vec4	new_vec4(t_type x, t_type y, t_type z, t_type w)
{
	return ((t_vec4){x, y, z, w});
}
