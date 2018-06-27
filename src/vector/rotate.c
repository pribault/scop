/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/03 19:13:58 by pribault          #+#    #+#             */
/*   Updated: 2018/06/27 20:15:12 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"

t_vec3	rotate_vec3(t_vec3 vec, t_quat quat)
{
	t_vec3	new;

	new.x = vec.x * (quat.w * quat.w + quat.x * quat.x - quat.y * quat.y -
		quat.z * quat.z) + vec.y * 2 * (quat.x * quat.y - quat.w * quat.z) +
		vec.z * 2 * (quat.x * quat.z + quat.w * quat.y);
	new.y = vec.x * 2 * (quat.x * quat.y + quat.w * quat.z) +
		vec.y * (quat.w * quat.w - quat.x * quat.x + quat.y * quat.y -
		quat.z * quat.z) + vec.z * 2 * (quat.y * quat.z - quat.w * quat.x);
	new.z = vec.x * 2 * (quat.x * quat.z - quat.w * quat.y) +
		vec.y * 2 * (quat.y * quat.z + quat.w * quat.x) +
		vec.z * (quat.w * quat.w - quat.x * quat.x - quat.y * quat.y +
		quat.z * quat.z);
	return (new);
}

t_vec3	rotate_axed_vec3(t_vec3 vec, t_vec3 axis, t_type angle)
{
	t_type	f;
	t_quat	quat;

	f = sin(angle / 2);
	quat.x = axis.x * f;
	quat.y = axis.y * f;
	quat.z = axis.z * f;
	quat.w = cos(angle / 2);
	quat = normalize_vec4(quat);
	return (rotate_vec3(vec, quat));
}
