/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bind.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/30 10:50:34 by pribault          #+#    #+#             */
/*   Updated: 2019/03/30 10:53:56 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void	bind_mat3(GLuint program, const char *name, t_mat3 *value)
{
	glUniformMatrix3fv(glGetUniformLocation(program, name), 1, GL_FALSE,
		(GLfloat *)value);
}

void	bind_mat4(GLuint program, const char *name, t_mat4 *value)
{
	glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_FALSE,
		(GLfloat *)value);
}

void	bind_vec3(GLuint program, const char *name, t_vec3 *value)
{
	glUniform3fv(glGetUniformLocation(program, name), 1, (GLfloat *)value);
}

void	bind_vec4(GLuint program, const char *name, t_vec4 *value)
{
	glUniform4fv(glGetUniformLocation(program, name), 1, (GLfloat *)value);
}

void	bind_texture(GLuint program, const char *name, GLuint texture,
		size_t index)
{
	glUniform1i(glGetUniformLocation(program, name), index);
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, texture);
}
