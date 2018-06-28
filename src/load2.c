/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/14 18:16:11 by pribault          #+#    #+#             */
/*   Updated: 2018/06/28 17:16:59 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void		get_numbers(char *params, size_t *n)
{
	size_t	i;

	i = 0;
	n[0] = (ft_strlen(params)) ? ft_atoi(params) : 0;
	while (ft_isdigit(params[i]))
		i++;
	if (params[i] == '/')
		i++;
	n[1] = (ft_strlen(params + i)) ? ft_atoi(params + i) : 0;
	while (ft_isdigit(params[i]))
		i++;
	if (params[i] == '/')
		i++;
	n[2] = (ft_strlen(params + i)) ? ft_atoi(params + i) : 0;
}

t_mat		*get_mat(t_vector *vec, char *name)
{
	t_mat	*current;
	size_t	i;

	i = (size_t)-1;
	while (++i < vec->n && (current = ft_vector_get(vec, i)))
		if (!ft_strcmp(name, current->name))
			return (current);
	error(102, name, 0);
	return (NULL);
}

void		add_vector(t_vector *vec, char **params, Uint8 n)
{
	t_vec4	new;

	new = new_vec4(0, 0, 0, 0);
	if (n >= 1)
		new.x = ft_atof(params[1]);
	if (n >= 2)
		new.y = ft_atof(params[2]);
	if (n >= 3)
		new.z = ft_atof(params[3]);
	if (n >= 4)
		new.w = ft_atof(params[4]);
	ft_vector_add(vec, &new);
}

t_texture	*gen_texture_from_color(t_buffer *buffer, t_vec3 *color)
{
	t_texture	texture;

	texture.w = 1;
	texture.h = 1;
	if (!(texture.name = ft_joinf("%p", color)) ||
		!(texture.img = malloc(sizeof(t_c))))
		error(1, NULL, 1);
	if (!color->x && !color->y && !color->z)
	{
		texture.img->r = 255;
		texture.img->g = 255;
		texture.img->b = 255;
	}
	else
	{
		texture.img->r = color->x * 255;
		texture.img->g = color->y * 255;
		texture.img->b = color->z * 255;
	}
	texture.img->a = 0;
	texture.id = create_image_buffer(&texture);
	ft_vector_add(&buffer->texture, &texture);
	return (ft_vector_get(&buffer->texture, buffer->texture.n - 1));
}

void		check_materials(t_buffer *buffer)
{
	t_mat	*mat;
	size_t	i;

	i = (size_t)-1;
	while (++i < buffer->mat.n && (mat = ft_vector_get(&buffer->mat, i)))
	{
		if (!mat->texture[0])
			mat->texture[0] = gen_texture_from_color(buffer, &mat->ambient);
		if (!mat->texture[1])
			mat->texture[1] = gen_texture_from_color(buffer, &mat->diffuse);
		if (!mat->texture[2])
			mat->texture[2] = gen_texture_from_color(buffer, &mat->specular);
	}
}
