/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/10 14:54:59 by pribault          #+#    #+#             */
/*   Updated: 2019/03/30 10:58:18 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void	set_sleep(void)
{
	static struct timeval	prev;
	struct timeval			t;
	Uint32					diff;

	gettimeofday(&t, NULL);
	diff = (t.tv_sec - prev.tv_sec) * 1000000 + (t.tv_usec - prev.tv_usec);
	prev = t;
	if (diff < 1000000 / FPS)
		usleep(1000000 / FPS - diff);
}

void	draw_depth(t_env *env)
{
	glBindFramebuffer(GL_FRAMEBUFFER, env->framebuffer);
	glClear(GL_DEPTH_BUFFER_BIT);
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glUseProgram(env->shaders.depth_program);
	render_stacks_depth(env);
}

void	draw(t_env *env)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glUseProgram(env->shaders.program);
	render_stacks(env);
}

int		main(int argc, char **argv)
{
	t_env	env;

	if (argc == 2)
	{
		init_env(&env);
		env.path = get_path(argv[1]);
		load_obj(&env, argv[1]);
		ft_putchar('\n');
		while (!env.stop)
		{
			while (SDL_PollEvent(&env.win.events))
				events(&env, &env.win.events);
			if (env.shadow)
				draw_depth(&env);
			draw(&env);
			glUseProgram(0);
			SDL_GL_SwapWindow(env.win.win);
			set_sleep();
			env.quat = mult_quat(env.quat, new_quat(env.rot_speed.x,
				env.rot_speed.y, env.rot_speed.z));
		}
	}
	else
		error(0, NULL, 1);
	return (0);
}
