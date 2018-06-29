/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/10 14:54:59 by pribault          #+#    #+#             */
/*   Updated: 2018/06/29 12:20:43 by pribault         ###   ########.fr       */
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

Uint32	get_fps(void)
{
	static struct timeval	prev;
	struct timeval			t;
	static Uint32			mem = 0;
	Uint32					fps;
	static Uint32			frames = 0;

	gettimeofday(&t, NULL);
	fps = (t.tv_sec - prev.tv_sec) * 1000000 + (t.tv_usec - prev.tv_usec);
	if (fps >= 500000)
	{
		prev = t;
		mem = (1000000 * frames) / fps;
		frames = 0;
	}
	else
		frames++;
	return (mem);
}

t_mat4	create_mvp(t_env *env)
{
	t_mat4	m;
	t_mat4	v;
	t_mat4	p;

	set_id_mat4(&m, 1);
	v = lookat(env->pos, add_vec3(env->pos, env->dir), new_vec3(0, 1, 0));
	p = perspective(45, 16.0 / 9.0, 1, 1000);
	return (mat_4_mat(m, mat_4_mat(v, p)));
}

void	render_stacks(t_env *env)
{
	t_mat4			mvp;
	t_stack			*stack;
	size_t			i;

	mvp = create_mvp(env);
	glUniformMatrix4fv(env->mvp_id, 1, GL_FALSE, (GLfloat*)&mvp);
	glUniformMatrix3fv(env->light_id, 1, GL_FALSE, (GLfloat*)&env->light);
	glUniform4fv(env->quat_id, 1, (GLfloat *)&env->quat);
	glUniform3fv(env->size_id, 1, (GLfloat *)&env->size);
	glUniform3fv(env->pos_id, 1, (GLfloat *)&env->obj_pos);
	glUniform3fv(env->cam_id, 1, (GLfloat *)&env->pos);
	i = (size_t)-1;
	while (++i < env->stack.n && (stack = ft_vector_get(&env->stack, i)))
	{
		glBindBuffer(GL_ARRAY_BUFFER, stack->v_id);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, stack->vt_id);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, stack->vn_id);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glUniform1i(env->tex_id[0], 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, stack->mat->texture[0]->id);
		glUniform1i(env->tex_id[1], 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, stack->mat->texture[1]->id);
		glUniform1i(env->tex_id[2], 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, stack->mat->texture[2]->id);
		glDrawArrays(env->draw_mode, 0, 3 * stack->n_f);
	}
}

int		main(int argc, char **argv)
{
	t_env	*env;

	if (argc == 2)
	{
		env = init_env();
		env->path = get_path(argv[1]);
		load_obj(env, argv[1]);
		ft_putchar('\n');
		while (!env->stop)
		{
			while (SDL_PollEvent(&env->win.events))
				events(env, &env->win.events);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			render_stacks(env);
			SDL_GL_SwapWindow(env->win.win);
			set_sleep();
			env->quat = mult_quat(env->quat, new_quat(env->rot_speed.x,
				env->rot_speed.y, env->rot_speed.z));
		}
	}
	else
		error(0, NULL, 1);
	return (0);
}
