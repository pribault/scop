/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/30 10:49:27 by pribault          #+#    #+#             */
/*   Updated: 2019/03/30 10:54:06 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

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
	projection = ortho(new_vec2(-100, 100), new_vec2(-100, 100),
		new_vec2(0.1, 1000));
	bind_mat4(env->shaders.program, "lightView", &projection);
	bind_mat4(env->shaders.program, "lightProjection", &projection);
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
	bind_mat3(env->shaders.program, "light", (t_mat3 *)&env->light);
	bind_vec3(env->shaders.program, "camera", &env->pos);
	bind_vec4(env->shaders.program, "quaternion", &env->quat);
	if (env->shadow)
		bind_texture(env->shaders.depth_program, "depthMap", env->depth_map, 0);
	i = (size_t)-1;
	while (++i < env->stack.n && (stack = ft_vector_get(&env->stack, i)))
		render_stack(env, stack);
}
