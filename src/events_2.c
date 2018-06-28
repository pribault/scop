/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/28 13:57:56 by pribault          #+#    #+#             */
/*   Updated: 2018/06/28 17:25:06 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

int		move_event(t_env *env, SDL_Event *event)
{
	if (event->key.keysym.sym == SDLK_KP_4)
		env->obj_pos.x -= STEP;
	else if (event->key.keysym.sym == SDLK_KP_5)
		env->obj_pos.y -= STEP;
	else if (event->key.keysym.sym == SDLK_KP_6)
		env->obj_pos.z -= STEP;
	else if (event->key.keysym.sym == SDLK_KP_7)
		env->obj_pos.x += STEP;
	else if (event->key.keysym.sym == SDLK_KP_8)
		env->obj_pos.y += STEP;
	else if (event->key.keysym.sym == SDLK_KP_9)
		env->obj_pos.z += STEP;
	else
		return (-1);
	ft_printf("\e[1A\e[Kobject_position(%f %f %f)\n", env->obj_pos.x,
		env->obj_pos.y, env->obj_pos.z);
	return (0);
}

int		color_event(t_env *env, SDL_Event *event)
{
	if (event->key.keysym.sym == SDLK_KP_4)
		env->light.color.x -= COLOR_STEP;
	else if (event->key.keysym.sym == SDLK_KP_5)
		env->light.color.y -= COLOR_STEP;
	else if (event->key.keysym.sym == SDLK_KP_6)
		env->light.color.z -= COLOR_STEP;
	else if (event->key.keysym.sym == SDLK_KP_7)
		env->light.color.x += COLOR_STEP;
	else if (event->key.keysym.sym == SDLK_KP_8)
		env->light.color.y += COLOR_STEP;
	else if (event->key.keysym.sym == SDLK_KP_9)
		env->light.color.z += COLOR_STEP;
	else
		return (-1);
	ft_printf("\e[1A\e[Kcolor(%f %f %f)\n", env->light.color.x,
		env->light.color.y, env->light.color.z);
	return (0);
}

int		resize_event(t_env *env, SDL_Event *event)
{
	if (event->key.keysym.sym == SDLK_KP_4)
		env->size.x -= RESIZE_SPEED;
	else if (event->key.keysym.sym == SDLK_KP_5)
		env->size.y -= RESIZE_SPEED;
	else if (event->key.keysym.sym == SDLK_KP_6)
		env->size.z -= RESIZE_SPEED;
	else if (event->key.keysym.sym == SDLK_KP_7)
		env->size.x += RESIZE_SPEED;
	else if (event->key.keysym.sym == SDLK_KP_8)
		env->size.y += RESIZE_SPEED;
	else if (event->key.keysym.sym == SDLK_KP_9)
		env->size.z += RESIZE_SPEED;
	else
		return (-1);
	ft_printf("\e[1A\e[Ksize(%f %f %f)\n", env->size.x,
		env->size.y, env->size.z);
	return (0);
}

int		number_event(t_env *env, SDL_Event *event)
{
	if (event->key.keysym.sym == SDLK_1)
		env->draw_mode = GL_TRIANGLES;
	else if (event->key.keysym.sym == SDLK_2)
		env->draw_mode = GL_LINES;
	else
		return (-1);
	return (0);
}
