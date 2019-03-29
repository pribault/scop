/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/27 13:38:34 by pribault          #+#    #+#             */
/*   Updated: 2018/06/29 13:42:48 by pribault         ###   ########.fr       */
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
	else if (event->key.keysym.sym == SDLK_UP)
		env->dir = rotate_vec3(env->dir, new_quat(0, 0, ANGLE));
	else if (event->key.keysym.sym == SDLK_DOWN)
		env->dir = rotate_vec3(env->dir, new_quat(0, 0, -ANGLE));
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

int		light_event(t_env *env, SDL_Event *event)
{
	if (event->key.keysym.sym == SDLK_KP_4)
		env->light.i.x -= (env->light.i.x - LIGHT_STEP >= 0) ? LIGHT_STEP : 0;
	else if (event->key.keysym.sym == SDLK_KP_5)
		env->light.i.y -= (env->light.i.y - LIGHT_STEP >= 0) ? LIGHT_STEP : 0;
	else if (event->key.keysym.sym == SDLK_KP_6)
		env->light.i.z -= (env->light.i.z - LIGHT_STEP >= 0) ? LIGHT_STEP : 0;
	else if (event->key.keysym.sym == SDLK_KP_7)
		env->light.i.x += LIGHT_STEP;
	else if (event->key.keysym.sym == SDLK_KP_8)
		env->light.i.y += LIGHT_STEP;
	else if (event->key.keysym.sym == SDLK_KP_9)
		env->light.i.z += LIGHT_STEP;
	else
		return (-1);
	ft_printf("\e[1A\e[Klight_intensity(%f %f %f)\n", env->light.i.x,
		env->light.i.y, env->light.i.z);
	return (0);
}

int		rot_event(t_env *env, SDL_Event *event)
{
	if (event->key.keysym.sym == SDLK_KP_4)
		env->rot_speed.x -= ANGLE;
	else if (event->key.keysym.sym == SDLK_KP_5)
		env->rot_speed.y -= ANGLE;
	else if (event->key.keysym.sym == SDLK_KP_6)
		env->rot_speed.z -= ANGLE;
	else if (event->key.keysym.sym == SDLK_KP_7)
		env->rot_speed.x += ANGLE;
	else if (event->key.keysym.sym == SDLK_KP_8)
		env->rot_speed.y += ANGLE;
	else if (event->key.keysym.sym == SDLK_KP_9)
		env->rot_speed.z += ANGLE;
	else
		return (-1);
	ft_printf("\e[1A\e[Krotation_speed(%f %f %f)\n", env->rot_speed.x,
		env->rot_speed.y, env->rot_speed.z);
	return (0);
}

void	events(t_env *env, SDL_Event *event)
{
	if (event->type == SDL_QUIT ||
		(event->type == SDL_WINDOWEVENT &&
		event->window.event == SDL_WINDOWEVENT_CLOSE))
		env->stop |= 1;
	else if (event->type == SDL_KEYDOWN)
	{
		if (event->key.keysym.sym == SDLK_ESCAPE)
			env->stop += (!(env->stop & 1)) ? 1 : 0;
		if (event->key.keysym.sym == SDLK_p)
			env->shadow = (env->shadow) ? 0 : 1;
		else if (!camera_events(env, event))
			return ;
		else if (event->key.keysym.sym == SDLK_KP_PLUS)
			env->input = (env->input + 1) % INPUT_MAX;
		else if (event->key.keysym.sym == SDLK_KP_MINUS)
			env->input = (env->input + INPUT_MAX - 1) % INPUT_MAX;
		else if ((env->input == INPUT_LIGHT && !light_event(env, event)) ||
			(env->input == INPUT_ROT && !rot_event(env, event)) ||
			(env->input == INPUT_MOVE && !move_event(env, event)) ||
			(env->input == INPUT_COLOR && !color_event(env, event)) ||
			(env->input == INPUT_SIZE && !resize_event(env, event)))
			return ;
		else if (!number_event(env, event))
			return ;
	}
}
