/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/27 13:38:34 by pribault          #+#    #+#             */
/*   Updated: 2018/06/27 21:18:46 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

int		camera_events_2(t_env *env, SDL_Event *event)
{
	if (event->key.keysym.sym == SDLK_UP)
		env->dir = rotate_axed_vec3(env->dir, rotate_vec3(new_vec3(env->dir.x,
			0, env->dir.z), new_quat(0, -PI / 2, 0)), -ANGLE);
	else if (event->key.keysym.sym == SDLK_DOWN)
		env->dir = rotate_axed_vec3(env->dir, rotate_vec3(new_vec3(env->dir.x,
			0, env->dir.z), new_quat(0, -PI / 2, 0)), ANGLE);
	else
		return (-1);
	return (0);
}

int		camera_events(t_env *env, SDL_Event *event)
{
	if (event->key.keysym.sym == SDLK_a)
		env->pos = mult2_vec3(add_vec3(env->pos, rotate_vec3(new_vec3(
			env->dir.x, 0, env->dir.z), new_quat(0, -PI / 2, 0))), STEP);
	else if (event->key.keysym.sym == SDLK_d)
		env->pos = mult2_vec3(add_vec3(env->pos, rotate_vec3(new_vec3(
			env->dir.x, 0, env->dir.z), new_quat(0, PI / 2, 0))), STEP);
	else if (event->key.keysym.sym == SDLK_q)
		env->pos = mult2_vec3(add_vec3(env->pos, new_vec3(0, -STEP, 0)), STEP);
	else if (event->key.keysym.sym == SDLK_e)
		env->pos = mult2_vec3(add_vec3(env->pos, new_vec3(0, STEP, 0)), STEP);
	else if (event->key.keysym.sym == SDLK_w)
		env->pos = add_vec3(env->pos, env->dir);
	else if (event->key.keysym.sym == SDLK_s)
		env->pos = sub_vec3(env->pos, env->dir);
	else if (event->key.keysym.sym == SDLK_LEFT)
		env->dir = rotate_vec3(env->dir, new_quat(0, -ANGLE, 0));
	else if (event->key.keysym.sym == SDLK_RIGHT)
		env->dir = rotate_vec3(env->dir, new_quat(0, ANGLE, 0));
	else
		return (camera_events_2(env, event));
	return (0);
}

void	events(t_env *env, SDL_Event *event)
{
	if (event->type == SDL_QUIT)
		env->stop |= 1;
	else if (event->type == SDL_WINDOWEVENT)
	{
		if (event->window.event == SDL_WINDOWEVENT_CLOSE)
			env->stop |= 1;
	}
	else if (event->type == SDL_KEYDOWN)
	{
		if (event->key.keysym.sym == SDLK_ESCAPE)
			env->stop += (!(env->stop & 1)) ? 1 : 0;
		else if (!camera_events(env, event))
			return ;
		else if (event->key.keysym.sym == SDLK_UP)
			env->dir = rotate_vec3(env->dir, new_quat(0, 0, ANGLE));
		else if (event->key.keysym.sym == SDLK_DOWN)
			env->dir = rotate_vec3(env->dir, new_quat(0, 0, -ANGLE));
		else if (event->key.keysym.sym == SDLK_KP_7)
			env->light.i.x += 0.01;
		else if (event->key.keysym.sym == SDLK_KP_8)
			env->light.i.y += 0.01;
		else if (event->key.keysym.sym == SDLK_KP_9)
			env->light.i.z += 0.01;
		else if (event->key.keysym.sym == SDLK_KP_4)
			env->light.i.x -= (env->light.i.x - 0.01 >= 0) ? 0.01 : 0;
		else if (event->key.keysym.sym == SDLK_KP_5)
			env->light.i.y -= (env->light.i.y - 0.01 >= 0) ? 0.01 : 0;
		else if (event->key.keysym.sym == SDLK_KP_6)
			env->light.i.z -= (env->light.i.z - 0.01 >= 0) ? 0.01 : 0;
		else if (event->key.keysym.sym == SDLK_KP_PLUS)
			;
		else if (event->key.keysym.sym == SDLK_KP_MINUS)
			;
		else if (event->key.keysym.sym == SDLK_LSHIFT)
			ft_printf("shift\n");
	}
}
