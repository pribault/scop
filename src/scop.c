/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/10 14:54:59 by pribault          #+#    #+#             */
/*   Updated: 2018/05/03 19:07:20 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void	events(t_env *env, SDL_Event *event)
{
	t_vec3	move;

	if (event->type == SDL_QUIT)
		env->stop += (!(env->stop & 1)) ? 1 : 0;
	else if (event->type == SDL_WINDOWEVENT)
	{
		if (event->window.event == SDL_WINDOWEVENT_CLOSE)
			env->stop += (!(env->stop & 1)) ? 1 : 0;
	}
	else if (event->type == SDL_KEYDOWN)
	{
		move = env->pos;
		normalize_vec3(&move);
		if (event->key.keysym.sym == SDLK_ESCAPE)
			env->stop += (!(env->stop & 1)) ? 1 : 0;
		else if (event->key.keysym.sym == SDLK_q)
			env->angle -= STEP;
		else if (event->key.keysym.sym == SDLK_d)
			env->angle += STEP;
		else if (event->key.keysym.sym == SDLK_z)
			env->pos = add_vec3(new_vec3(-move.x, -move.y, -move.z), env->pos);
		else if (event->key.keysym.sym == SDLK_s)
			env->pos = add_vec3(move, env->pos);
		else if (event->key.keysym.sym == SDLK_a)
			env->pos = add_vec3(new_vec3(0, -1, 0), env->pos);
		else if (event->key.keysym.sym == SDLK_e)
			env->pos = add_vec3(new_vec3(0, 1, 0), env->pos);
		else if (event->key.keysym.sym == SDLK_UP)
			env->center = add_vec3(new_vec3(0, 1, 0), env->center);
		else if (event->key.keysym.sym == SDLK_DOWN)
			env->center = add_vec3(new_vec3(0, -1, 0), env->center);
		else if (event->key.keysym.sym == SDLK_KP_7)
			env->light.i = add_vec3(new_vec3(0.01, 0, 0), env->light.i);
		else if (event->key.keysym.sym == SDLK_KP_8)
			env->light.i = add_vec3(new_vec3(0, 0.01, 0), env->light.i);
		else if (event->key.keysym.sym == SDLK_KP_9)
			env->light.i = add_vec3(new_vec3(0, 0, 0.01), env->light.i);
		else if (event->key.keysym.sym == SDLK_KP_4)
			env->light.i = add_vec3(new_vec3(-0.01, 0, 0), env->light.i);
		else if (event->key.keysym.sym == SDLK_KP_5)
			env->light.i = add_vec3(new_vec3(0, -0.01, 0), env->light.i);
		else if (event->key.keysym.sym == SDLK_KP_6)
			env->light.i = add_vec3(new_vec3(0, 0, -0.01), env->light.i);
		env->light.i.x = (env->light.i.x < 0) ? 0 : env->light.i.x;
		env->light.i.y = (env->light.i.y < 0) ? 0 : env->light.i.y;
		env->light.i.z = (env->light.i.z < 0) ? 0 : env->light.i.z;
	}
}

void	set_sleep(void)
{
	static struct timeval	prev;
	struct timeval			t;
	Uint32					diff;

	gettimeofday(&t, NULL);
	diff = (t.tv_sec - prev.tv_sec) * 1000000 + (t.tv_usec - prev.tv_usec);
	prev = t;
	if (diff < 1000000 / 60)
	{
		// printf("dif=%u/%u sleep for %u\n", diff, 1000000 / 60, 1000000 / 60 - diff);
		usleep(1000000 / 60 - diff);
	}
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
		// printf("prev(%u %u) t(%u %u) fps=%u\n", prev.tv_sec, prev.tv_usec, t.tv_sec, t.tv_usec, fps);
		mem = (1000000 * frames) / fps;
		frames = 0;
	}
	else
		frames++;
	return (mem);
}

void	print_list(t_list *list)
{
	t_stack	*stack;
	size_t	i;

	while (list)
	{
		stack = (t_stack*)list->content;
		// printf("stack using %s; size(%u)\n", stack->mat->name, stack->n_f);
		i = 0;
		while (i < stack->n_f)
		{
			printf("  [%.2f,%.2f ; %.2f,%.2f ; %.2f,%.2f]\n",
			stack->vt[i * 3].x, stack->vt[i * 3].y,
			stack->vt[i * 3 + 1].x, stack->vt[i * 3 + 1].y,
			stack->vt[i * 3 + 2].x, stack->vt[i * 3 + 2].y);
			i++;
		}
		list = list->next;
	}
}

t_mat4	create_mvp(t_env *env)
{
	t_mat4	m;
	t_mat4	v;
	t_mat4	p;

	set_id_mat4(&m, 1);
	v = lookat(env->pos, env->center, new_vec3(0, 1, 0));
	p = perspective(45, 16.0 / 9.0, 1, 1000);
	return (mat_4_mat(m, mat_4_mat(v, p)));
}

void	render_stacks(t_env *env)
{
	GLuint			mvp_id;
	t_mat4			mvp;
	t_list			*list;
	t_stack			*stack;

	list = env->stack;
	mvp = create_mvp(env);
	mvp_id = glGetUniformLocation(env->shaders.program, "MVP");
	env->light_id = glGetUniformLocation(env->shaders.program, "light");
	glUniformMatrix4fv(mvp_id, 1, GL_FALSE, (GLfloat*)&mvp);
	glUniformMatrix3fv(env->light_id, 1, GL_FALSE, (GLfloat*)&env->light);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(env->shaders.program);
	while (list)
	{
		stack = list->content;

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, stack->v_id);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, stack->vt_id);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, stack->vn_id);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		if (stack->mat && stack->mat->texture[0])
			glBindTexture(GL_TEXTURE_2D, stack->mat->texture[0]->id);
		if (stack->mat && stack->mat->texture[1])
			glBindTexture(GL_TEXTURE_2D, stack->mat->texture[1]->id);
		if (stack->mat && stack->mat->texture[2])
			glBindTexture(GL_TEXTURE_2D, stack->mat->texture[2]->id);
		glDrawArrays(GL_TRIANGLES, 0, 3 * stack->n_f);
		if (stack->mat && stack->mat->texture[1])
			glBindTexture(GL_TEXTURE_2D, stack->mat->texture[1]->id);
		if (stack->mat && stack->mat->texture[1])
			glBindTexture(GL_TEXTURE_2D, stack->mat->texture[1]->id);
		if (stack->mat && stack->mat->texture[2])
			glBindTexture(GL_TEXTURE_2D, stack->mat->texture[2]->id);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		list = list->next;
	}
}

// void	render_triangles(t_env *env)
// {
// 	GLuint	mvp_id;
// 	GLuint	b;
// 	t_mat4	mvp;
// 	t_list	*list;
// 	t_elem	*f;

// 	list = env->f;
// 	mvp = create_mvp(env);
// 	printf("\033[1A\033[Kangle=%f center=(%f %f %f)\n", env->angle, env->center.x, env->center.y, env->center.z);
// 	mvp_id = glGetUniformLocation(env->shaders.program, "MVP");
// 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// 	glUseProgram(env->shaders.program);
// 	while (list)
// 	{
// 		f = (t_elem*)list->content;
// 		glUniformMatrix4fv(mvp_id, 1, GL_FALSE, (GLfloat*)&mvp);
// 		glGenBuffers(1, &b);
// 		glBindBuffer(GL_ARRAY_BUFFER, b);
// 		glBufferData(GL_ARRAY_BUFFER, sizeof(t_vec3) * 3, f->v, GL_STATIC_DRAW);
// 		glEnableVertexAttribArray(0);
// 		glBindBuffer(GL_ARRAY_BUFFER, b);
// 		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
// 		glDrawArrays(GL_TRIANGLES, 0, 3);
// 		glDisableVertexAttribArray(0);
// 		list = list->next;
// 		glDeleteBuffers(1, &b);
// 	}
// }

int		main(int argc, char **argv)
{
	t_env	*env;

	if (argc == 2)
	{
		env = init_env();
		env->path = get_path(argv[1]);
		load_obj(env, argv[1]);
		// print_list(env->stack);
		while (!env->stop)
		{
			while (SDL_PollEvent(&env->win.events))
				events(env, &env->win.events);
			env->pos = mat_3_vec(new_mat3(new_vec3(cos(-env->angle), 0,
			-sin(-env->angle)), new_vec3(0, 1, 0), new_vec3(sin(-env->angle),
			0, cos(-env->angle))), env->pos);
			render_stacks(env);
			// render_triangles(env);
			printf("\033[1A\033[Kfps=%u angle=%f i(%f %f %f)\n", get_fps(), env->angle, env->light.i.x, env->light.i.y, env->light.i.z);
			SDL_GL_SwapWindow(env->win.win);
			set_sleep();
		}
	}
	else
		error(0, NULL, 1);
	return (0);
}
