/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/10 15:08:51 by pribault          #+#    #+#             */
/*   Updated: 2018/06/28 17:22:15 by pribault         ###   ########.fr       */
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

void	init_shaders(t_shaders *shaders)
{
	char	log[1024];
	char	*file;

	shaders->vertex = glCreateShader(GL_VERTEX_SHADER);
	shaders->fragment = glCreateShader(GL_FRAGMENT_SHADER);
	shaders->program = glCreateProgram();
	file = get_file_content("vertex.glsl");
	glShaderSource(shaders->vertex, 1, (const GLchar**)&file, NULL);
	free(file);
	file = get_file_content("fragment.glsl");
	glShaderSource(shaders->fragment, 1, (const GLchar**)&file, NULL);
	free(file);
	glCompileShader(shaders->vertex);
	glCompileShader(shaders->fragment);
	glAttachShader(shaders->program, shaders->vertex);
	glAttachShader(shaders->program, shaders->fragment);
	glLinkProgram(shaders->program);
	glGetShaderInfoLog(shaders->vertex, 1024, NULL, log);
	ft_printf("%s: %s\n", "vertex.glsl", log);
	glGetShaderInfoLog(shaders->fragment, 1024, NULL, log);
	ft_printf("%s: %s\n", "fragment.glsl", log);
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
	glUseProgram(env->shaders.program);
	env->mvp_id = glGetUniformLocation(env->shaders.program, "MVP");
	env->light_id = glGetUniformLocation(env->shaders.program, "light");
	env->quat_id = glGetUniformLocation(env->shaders.program, "quaternion");
	env->pos_id = glGetUniformLocation(env->shaders.program, "position");
	env->size_id = glGetUniformLocation(env->shaders.program, "size");
	env->cam_id = glGetUniformLocation(env->shaders.program, "camera");
	env->tex_id[0] = glGetUniformLocation(env->shaders.program, "ka");
	env->tex_id[1] = glGetUniformLocation(env->shaders.program, "kd");
	env->tex_id[2] = glGetUniformLocation(env->shaders.program, "ks");
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
}

t_env	*init_env(void)
{
	t_env	*env;

	if (!(env = (t_env*)malloc(sizeof(t_env))))
		error(1, NULL, 1);
	ft_bzero(env, sizeof(t_env));
	init_sdl(&env->win);
	init_shaders(&env->shaders);
	env->pos = new_vec3(-100, 0, 0);
	env->dir = normalize_vec3(new_vec3(1, 0, 0));
	env->light.pos = new_vec3(0, 100, 0);
	env->light.color = new_vec3(1, 1, 1);
	env->light.i = new_vec3(0.42, 0.6, 0.1);
	env->quat = new_quat(0, 0, 0);
	env->obj_pos = new_vec3(0, 0, 0);
	env->size = new_vec3(1, 1, 1);
	env->rot_speed = new_vec3(0, ROT_SPEED, 0);
	env->input = INPUT_LIGHT;
	env->draw_mode = GL_TRIANGLES;
	init_opengl(env);
	ft_vector_init(&env->stack, ALLOC_MALLOC, sizeof(t_stack));
	return (env);
}
