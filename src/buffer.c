/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/15 15:36:32 by pribault          #+#    #+#             */
/*   Updated: 2017/08/15 16:04:20 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

GLuint	create_image_buffer(t_texture *texture)
{
	GLuint	new;

	glGenTextures(1, &new);
	glBindTexture(GL_TEXTURE_2D, new);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->w, texture->h,
	0, GL_BGRA, GL_UNSIGNED_BYTE, texture->img);
	glBindTexture(GL_TEXTURE_2D, new);
	return (new);
}

GLuint	create_buffer(void *data, size_t size)
{
	GLuint	new;

	glGenBuffers(1, &new);
	glBindBuffer(GL_ARRAY_BUFFER, new);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	return (new);
}
