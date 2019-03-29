/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/10 15:08:51 by pribault          #+#    #+#             */
/*   Updated: 2018/06/29 14:22:11 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void	init_sdl(t_win *win)
{
	if (SDL_Init(SDL_INIT_VIDEO) == -1 || !IMG_Init(IMG_INIT_PNG))
		error(3, NULL, 1);
	if (SDL_GetNumVideoDisplays() < 1)
		error(4, NULL, 1);
	if (SDL_GetDisplayMode(0, 0, &win->mode))
		error(5, NULL, 1);
	win->width = win->mode.w / 2;
	win->height = win->mode.h / 2;
	win->name = ft_strdup("scop");
	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4) < 0)
		error(8, NULL, 1);
	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0) < 0)
		error(8, NULL, 1);
	if (SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) < 0)
		error(8, NULL, 1);
	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
		SDL_GL_CONTEXT_PROFILE_CORE) < 0)
		error(8, NULL, 1);
	if (!(win->win = SDL_CreateWindow(win->name, 0, 0,
	win->width, win->height, SDL_WINDOW_OPENGL)))
		error(6, NULL, 1);
	if (!(win->context = SDL_GL_CreateContext(win->win)))
		error(7, NULL, 1);
#ifdef __LINUX__
	if (glewInit() != GLEW_OK)
		error(3, NULL, 1);
#endif
}

char	*get_file_content(char *file)
{
	char	*line;
	int		fd;

	if (!file)
		return (NULL);
	if ((fd = open(file, O_RDONLY)) == -1)
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
		env->win.height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
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
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glGenVertexArrays(1, &env->vao);
	glBindVertexArray(env->vao);
	init_opengl_2(env);
}

void	init_env(t_env	*env)
{
	ft_bzero(env, sizeof(t_env));
	init_sdl(&env->win);
	init_shaders(&env->shaders);
	env->pos = new_vec3(-100, 0, 0);
	env->dir = normalize_vec3(new_vec3(1, 0, 0));
	env->light.pos = new_vec3(0, 100, 0);
	env->light.color = new_vec3(1, 1, 1);
	env->light.i = new_vec3(0.42, 0.6, 0.2);
	env->quat = new_quat(0, 0, 0);
	env->obj_pos = new_vec3(0, 0, 0);
	env->size = new_vec3(1, 1, 1);
	env->rot_speed = new_vec3(0, ROT_SPEED, 0);
	env->input = INPUT_LIGHT;
	env->draw_mode = GL_TRIANGLES;
	env->shadow = 1;
	init_opengl(env);
	ft_vector_init(&env->stack, ALLOC_MALLOC, sizeof(t_stack));
}
