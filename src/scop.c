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

void	bind_mat3(GLuint program, const char *name, t_mat3 *value)
{
	glUniformMatrix3fv(glGetUniformLocation(program, name), 1, GL_FALSE, (GLfloat *)value);
}

void	bind_mat4(GLuint program, const char *name, t_mat4 *value)
{
	glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_FALSE, (GLfloat *)value);
}

void	bind_vec3(GLuint program, const char *name, t_vec3 *value)
{
	glUniform3fv(glGetUniformLocation(program, name), 1, (GLfloat *)value);
}

void	bind_vec4(GLuint program, const char *name, t_vec4 *value)
{
	glUniform4fv(glGetUniformLocation(program, name), 1, (GLfloat *)value);
}

void	bind_texture(GLuint program, const char *name, GLuint texture, size_t index)
{
	glUniform1i(glGetUniformLocation(program, name), index);
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, texture);
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
	bind_mat4(env->shaders.program, "model", &model);
	bind_mat4(env->shaders.program, "view", &view);
	bind_mat4(env->shaders.program, "projection", &projection);
	view = lookat(env->light.pos, new_vec3(0, 0, 0), new_vec3(0, 1, 0));
	projection = ortho(new_vec2(-100, 100), new_vec2(-100, 100), new_vec2(0.1, 1000));
	bind_mat4(env->shaders.program, "lightView", &projection);
	bind_mat4(env->shaders.program, "lightProjection", &projection);
}

void	bind_mvp_depth(t_env *env)
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
	bind_mat4(env->shaders.depth_program, "model", &model);
	view = lookat(env->light.pos, new_vec3(0, 0, 0), new_vec3(0, 1, 0));
	projection = ortho(new_vec2(-100, 100), new_vec2(-100, 100), new_vec2(0.1, 1000));
	bind_mat4(env->shaders.depth_program, "lightView", &view);
	bind_mat4(env->shaders.depth_program, "lightProjection", &projection);
}

void	render_stack_depth(t_env *env, t_stack *stack)
{
	glBindBuffer(GL_ARRAY_BUFFER, stack->v_id);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glDrawArrays(env->draw_mode, 0, 3 * stack->n_f);
}

void	render_stacks_depth(t_env *env)
{
	t_stack			*stack;
	size_t			i;

	bind_mvp(env);
	bind_mat3(env->shaders.depth_program, "light", &env->light);
	bind_vec4(env->shaders.depth_program, "quaternion", &env->quat);
	i = (size_t)-1;
	while (++i < env->stack.n && (stack = ft_vector_get(&env->stack, i)))
		render_stack_depth(env, stack);
}

void	render_stack(t_env *env, t_stack *stack)
{
	glBindBuffer(GL_ARRAY_BUFFER, stack->v_id);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, stack->vt_id);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, stack->vn_id);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	bind_texture(env->shaders.program, "ka", stack->mat->texture[0]->id, 1);
	bind_texture(env->shaders.program, "kd", stack->mat->texture[1]->id, 2);
	bind_texture(env->shaders.program, "ks", stack->mat->texture[2]->id, 3);
	glDrawArrays(env->draw_mode, 0, 3 * stack->n_f);
}

void	render_stacks(t_env *env)
{
	t_stack			*stack;
	size_t			i;

	bind_mvp(env);
	bind_mat3(env->shaders.program, "light", &env->light);
	bind_vec3(env->shaders.program, "camera", &env->pos);
	bind_vec4(env->shaders.program, "quaternion", &env->quat);
	if (env->shadow)
		bind_texture(env->shaders.depth_program, "depthMap", env->depth_map, 0);
	i = (size_t)-1;
	while (++i < env->stack.n && (stack = ft_vector_get(&env->stack, i)))
		render_stack(env, stack);
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

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glBindFramebuffer(GL_FRAMEBUFFER, env.framebuffer);
			glClear(GL_DEPTH_BUFFER_BIT);
			glDisable(GL_CULL_FACE);
			glUseProgram(env.shaders.depth_program);
			render_stacks_depth(&env);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glUseProgram(env.shaders.program);
			render_stacks(&env);
			glUseProgram(0);
			glBindTexture(GL_TEXTURE_2D, env.depth_map);
			glDrawArrays(env.draw_mode, 0, 2);
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
