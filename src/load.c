/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/10 19:27:45 by pribault          #+#    #+#             */
/*   Updated: 2018/06/28 16:59:19 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void	add_elem(t_buffer *list, char *param1, char *param2, char *param3)
{
	char	*params[3];
	size_t	n[3];
	t_elem	f;
	size_t	i;

	i = (size_t)-1;
	params[0] = param1;
	params[1] = param2;
	params[2] = param3;
	ft_bzero(&f, sizeof(t_elem));
	while (++i < 3)
	{
		get_numbers(params[i], n);
		if (n[0])
			f.v[i] = *(t_vec3 *)ft_vector_get(&list->v, n[0] - 1);
		if (n[1])
			f.vt[i] = *(t_vec2 *)ft_vector_get(&list->vt, n[1] - 1);
		if (n[2])
			f.vn[i] = *(t_vec3 *)ft_vector_get(&list->vn, n[2] - 1);
		else
			f.vn[i] = (t_vec3){f.v[i].x, f.v[i].y, f.v[i].z};
	}
	ft_vector_add(&list->f, &f);
}

void	alloc_stack(t_env *env, t_buffer *list)
{
	t_stack	*stack;
	t_elem	*elem;
	size_t	i;

	i = (size_t)-1;
	if (!env->stack.n || !(stack =
		ft_vector_get(&env->stack, env->stack.n - 1)))
		return ;
	stack->n_f = list->f.n;
	if (!(stack->v = (t_vec3*)malloc(sizeof(t_vec3) * stack->n_f * 3)) ||
		!(stack->vn = (t_vec3*)malloc(sizeof(t_vec3) * stack->n_f * 3)) ||
		!(stack->vt = (t_vec2*)malloc(sizeof(t_vec2) * stack->n_f * 3)))
		error(1, NULL, 1);
	while (++i < list->f.n && (elem = ft_vector_get(&list->f, i)))
	{
		ft_memcpy(&stack->v[i * 3], &elem->v, sizeof(t_vec3) * 3);
		ft_memcpy(&stack->vn[i * 3], &elem->vn, sizeof(t_vec3) * 3);
		ft_memcpy(&stack->vt[i * 3], &elem->vt, sizeof(t_vec2) * 3);
	}
	stack->v_id = create_buffer(stack->v, sizeof(t_vec3) * stack->n_f * 3);
	stack->vn_id = create_buffer(stack->vn, sizeof(t_vec3) * stack->n_f * 3);
	stack->vt_id = create_buffer(stack->vt, sizeof(t_vec2) * stack->n_f * 3);
	ft_vector_resize(&list->f, 0);
}

void	treat_params(t_env *env, t_buffer *list, char **params, size_t len)
{
	t_stack		stack;

	if (!ft_strcmp(params[0], "v") && len == 4)
		add_vector(&list->v, params, 3);
	else if (!ft_strcmp(params[0], "vn") && len == 4)
		add_vector(&list->vn, params, 3);
	else if (!ft_strcmp(params[0], "vt") && len >= 3)
		add_vector(&list->vt, params, 2);
	else if (!ft_strcmp(params[0], "mtllib") && len == 2)
		load_mtllib(list, (ft_strlen(env->path)) ? ft_joinf("%s/%s",
		env->path, params[1]) : ft_strdup(params[1]));
	else if (!ft_strcmp(params[0], "usemtl") && len == 2)
	{
		alloc_stack(env, list);
		ft_bzero(&stack, sizeof(t_stack));
		stack.mat = get_mat(&list->mat, params[1]);
		ft_vector_add(&env->stack, &stack);
	}
	else if (!ft_strcmp(params[0], "f") && (len == 4 || len == 5))
		add_elem(list, params[1], params[2], params[3]);
	else
		error(100, params[0], 0);
	if (!ft_strcmp(params[0], "f") && len == 5)
		add_elem(list, params[1], params[3], params[4]);
}

void	init_buffer(t_env *env, t_buffer *buffer)
{
	static t_c			default_color = {255, 255, 255, 255};
	static t_texture	default_texture = {"default_white", 1, 1,
		&default_color, 0};
	static t_mat		default_mat = {"default", {&default_texture,
		&default_texture, &default_texture}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1},
		0, 0};
	t_stack				stack;

	ft_vector_init(&buffer->v, ALLOC_MALLOC, sizeof(t_vec3));
	ft_vector_init(&buffer->vt, ALLOC_MALLOC, sizeof(t_vec2));
	ft_vector_init(&buffer->vn, ALLOC_MALLOC, sizeof(t_vec3));
	ft_vector_init(&buffer->f, ALLOC_MALLOC, sizeof(t_elem));
	ft_vector_init(&buffer->mat, ALLOC_MALLOC, sizeof(t_mat));
	ft_vector_init(&buffer->texture, ALLOC_MALLOC, sizeof(t_texture));
	ft_vector_add(&buffer->mat, &default_mat);
	ft_vector_add(&buffer->texture, &default_texture);
	ft_bzero(&stack, sizeof(t_stack));
	stack.mat = get_mat(&buffer->mat, "default");
	ft_vector_add(&env->stack, &stack);
}

void	load_obj(t_env *env, char *file)
{
	size_t		len;
	char		**array;
	char		*line;
	int			fd;

	if ((fd = open(file, O_RDONLY)) == -1)
		error(2, file, 1);
	init_buffer(env, &env->buffer);
	while (ft_get_next_line(fd, &line) == 1)
	{
		array = ft_multisplit(line, SEPARATORS);
		len = ft_arraylen(array);
		if (len > 0 && array[0][0] != '#')
			treat_params(env, &env->buffer, array, len);
		ft_free_array((void**)array, len + 1);
		free(line);
	}
	alloc_stack(env, &env->buffer);
	ft_vector_del(&env->buffer.v);
	ft_vector_del(&env->buffer.vt);
	ft_vector_del(&env->buffer.vn);
	ft_vector_del(&env->buffer.f);
	close(fd);
	check_materials(&env->buffer);
}
