/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opengl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/30 10:44:56 by pribault          #+#    #+#             */
/*   Updated: 2019/03/30 12:45:41 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

char	*get_file_content(char *file)
{
	struct stat	buf;
	char		*line;
	int			fd;

	if (!file)
		return (NULL);
	if ((fd = open(file, O_RDONLY)) == -1 || fstat(fd, &buf) ||
		(buf.st_mode & S_IFMT) != S_IFREG)
		error(2, file, 1);
	ft_get_all_lines(fd, &line);
	close(fd);
	return (line);
}

void	init_shaders_2(t_shaders *shaders)
{
	char	log[1024];
	char	*file;

	shaders->depth_vertex = glCreateShader(GL_VERTEX_SHADER);
	shaders->depth_fragment = glCreateShader(GL_FRAGMENT_SHADER);
	shaders->depth_program = glCreateProgram();
	file = get_file_content("shaders/depth_vertex.glsl");
	glShaderSource(shaders->depth_vertex, 1, (const GLchar**)&file, NULL);
	free(file);
	file = get_file_content("shaders/depth_fragment.glsl");
	glShaderSource(shaders->depth_fragment, 1, (const GLchar**)&file, NULL);
	free(file);
	glCompileShader(shaders->depth_vertex);
	glCompileShader(shaders->depth_fragment);
	glAttachShader(shaders->depth_program, shaders->depth_vertex);
	glAttachShader(shaders->depth_program, shaders->depth_fragment);
	glLinkProgram(shaders->depth_program);
	glGetShaderInfoLog(shaders->depth_vertex, 1024, NULL, log);
	ft_printf("%s: %s\n", "shaders/depth_vertex.glsl", log);
	glGetShaderInfoLog(shaders->depth_fragment, 1024, NULL, log);
	ft_printf("%s: %s\n", "shaders/depth_fragment.glsl", log);
}

void	init_shaders(t_shaders *shaders)
{
	char	log[1024];
	char	*file;

	shaders->vertex = glCreateShader(GL_VERTEX_SHADER);
	shaders->fragment = glCreateShader(GL_FRAGMENT_SHADER);
	shaders->program = glCreateProgram();
	file = get_file_content("shaders/vertex.glsl");
	glShaderSource(shaders->vertex, 1, (const GLchar**)&file, NULL);
	free(file);
	file = get_file_content("shaders/fragment.glsl");
	glShaderSource(shaders->fragment, 1, (const GLchar**)&file, NULL);
	free(file);
	glCompileShader(shaders->vertex);
	glCompileShader(shaders->fragment);
	glAttachShader(shaders->program, shaders->vertex);
	glAttachShader(shaders->program, shaders->fragment);
	glLinkProgram(shaders->program);
	glGetShaderInfoLog(shaders->vertex, 1024, NULL, log);
	ft_printf("%s: %s\n", "shaders/vertex.glsl", log);
	glGetShaderInfoLog(shaders->fragment, 1024, NULL, log);
	ft_printf("%s: %s\n", "shaders/fragment.glsl", log);
	init_shaders_2(shaders);
}

void	init_opengl_2(t_env *env)
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glGenFramebuffers(1, &env->framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, env->framebuffer);
	glGenTextures(1, &env->depth_map);
	glBindTexture(1, env->depth_map);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, env->win.width,
		env->win.height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, env->depth_map,
		0);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void	init_opengl(t_env *env)
{
	ft_printf("OpenGL version: %s\nShader Version: %s\n",
		glGetString(GL_VERSION),
		glGetString(GL_SHADING_LANGUAGE_VERSION));
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glGenVertexArrays(1, &env->vao);
	glBindVertexArray(env->vao);
	init_opengl_2(env);
}
