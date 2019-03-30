/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_depth.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/30 10:49:27 by pribault          #+#    #+#             */
/*   Updated: 2019/03/30 10:54:12 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

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
	projection = ortho(new_vec2(-100, 100), new_vec2(-100, 100),
		new_vec2(0.1, 1000));
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

	bind_mvp_depth(env);
	bind_vec4(env->shaders.depth_program, "quaternion", &env->quat);
	i = (size_t)-1;
	while (++i < env->stack.n && (stack = ft_vector_get(&env->stack, i)))
		render_stack_depth(env, stack);
}
