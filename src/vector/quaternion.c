/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quaternion.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/03 19:17:47 by pribault          #+#    #+#             */
/*   Updated: 2018/06/27 14:30:58 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"

t_quat	new_quat(t_type x, t_type y, t_type z)
{
	double	tmp[6];
	t_quat	new;

	tmp[0] = cos(-z / 2);
	tmp[1] = sin(-z / 2);
	tmp[2] = cos(-x / 2);
	tmp[3] = sin(-x / 2);
	tmp[4] = cos(-y / 2);
	tmp[5] = sin(-y / 2);
	new.w = tmp[0] * tmp[2] * tmp[4] + tmp[1] * tmp[3] * tmp[5];
	new.x = tmp[0] * tmp[3] * tmp[4] - tmp[1] * tmp[2] * tmp[5];
	new.y = tmp[0] * tmp[2] * tmp[5] + tmp[1] * tmp[3] * tmp[4];
	new.z = tmp[1] * tmp[2] * tmp[4] - tmp[0] * tmp[3] * tmp[5];
	return (new);
}

double	get_quaternion_norm(t_quat q)
{
	return (q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
}

t_quat	normalize_quat(t_quat q)
{
	double	norm;

	norm = get_quaternion_norm(q);
	q.x /= norm;
	q.y /= norm;
	q.z /= norm;
	q.w /= norm;
	return (q);
}

t_quat	mult_quat(t_quat q1, t_quat q2)
{
	t_quat	new;

	new.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
	new.x = q1.w * q2.x + q1.x * q2.w - q1.y * q2.z + q1.z * q2.y;
	new.y = q1.w * q2.y + q1.x * q2.z + q1.y * q2.w - q1.z * q2.x;
	new.z = q1.w * q2.z - q1.x * q2.y + q1.y * q2.x + q1.z * q2.w;
	normalize_quat(new);
	return (new);
}
