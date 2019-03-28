/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/10 14:54:59 by pribault          #+#    #+#             */
/*   Updated: 2018/08/28 11:36:07 by pribault         ###   ########.fr       */
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

void	bind_mvp(t_env *env)
{
	t_mat4	model;
	t_mat4	view;
	t_mat4	projection;

	set_id_mat4(&model, 1);
	model.x.x = env->size.x;
	model.y.y = env->size.y;
	model.z.z = env->size.z;
	model.w.x = env->obj_pos.x;
	model.w.y = env->obj_pos.y;
	model.w.z = env->obj_pos.z;
	view = lookat(env->pos, add_vec3(env->pos, env->dir), new_vec3(0, 1, 0));
	projection = perspective(45, 16.0 / 9.0, 1, 1000);
	glUniformMatrix4fv(env->model_id, 1, GL_FALSE, (GLfloat*)&model);
	glUniformMatrix4fv(env->view_id, 1, GL_FALSE, (GLfloat*)&view);
	glUniformMatrix4fv(env->projection_id, 1, GL_FALSE, (GLfloat*)&projection);
}

void	render_stack(t_env *env, t_stack *stack)
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

void	render_stacks(t_env *env)
{
	t_stack			*stack;
	size_t			i;

	bind_mvp(env);
	glUniformMatrix3fv(env->light_id, 1, GL_FALSE, (GLfloat*)&env->light);
	glUniform4fv(env->quat_id, 1, (GLfloat *)&env->quat);
	glUniform3fv(env->size_id, 1, (GLfloat *)&env->size);
	glUniform3fv(env->pos_id, 1, (GLfloat *)&env->obj_pos);
	glUniform3fv(env->cam_id, 1, (GLfloat *)&env->pos);
	i = (size_t)-1;
	while (++i < env->stack.n && (stack = ft_vector_get(&env->stack, i)))
		render_stack(env, stack);
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
