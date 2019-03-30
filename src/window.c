/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/10 15:08:51 by pribault          #+#    #+#             */
/*   Updated: 2019/03/30 10:49:08 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

#ifdef __LINUX__

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
	if (glewInit() != GLEW_OK)
		error(3, NULL, 1);
}

#else

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

#endif

void	init_env(t_env *env)
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
