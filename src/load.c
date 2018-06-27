/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/10 19:27:45 by pribault          #+#    #+#             */
/*   Updated: 2018/06/27 21:35:39 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void	add_elem(t_buffer *list, char **params, size_t len)
{
	size_t	n[3];
	t_elem	f;
	size_t	i;

	i = 0;
	while (i < len - 1)
	{
		ft_bzero(n, sizeof(size_t) * 3);
		get_numbers(params[i + 1], n);
		if (n[0])
			f.v[i] = *((t_vec3*)get_listn(list->v, n[0])->content);
		if (n[1])
			f.vt[i] = *((t_vec2*)get_listn(list->vt, n[1])->content);
		if (n[2])
			f.vn[i] = *((t_vec3*)get_listn(list->vn, n[2])->content);
		i++;
	}
	ft_lstadd(&list->f, ft_lstnew(&f, sizeof(t_elem)));
}

void	add_vector(t_list **list, char **params, Uint8 n)
{
	t_vec4	new;

	new = new_vec4(0, 0, 0, 0);
	if (n >= 1)
		new.x = ft_atof(params[1]);
	if (n >= 2)
		new.y = ft_atof(params[2]);
	if (n >= 3)
		new.z = ft_atof(params[3]);
	if (n >= 4)
		new.w = ft_atof(params[4]);
	ft_lstadd(list, ft_lstnew(&new, sizeof(t_type) * n));
}

void	alloc_stack(t_env *env, t_buffer *list)
{
	t_stack	*stack;
	t_list	*l;
	size_t	i;

	if (!env->stack)
		return ;
	stack = env->stack->content;
	i = 0;
	l = list->f;
	stack->n_f = ft_listlen(l);
	if (!(stack->v = (t_vec3*)malloc(sizeof(t_vec3) * stack->n_f * 3)) ||
		!(stack->vn = (t_vec3*)malloc(sizeof(t_vec3) * stack->n_f * 3)) ||
		!(stack->vt = (t_vec2*)malloc(sizeof(t_vec2) * stack->n_f * 3)))
		error(1, NULL, 1);
	while (l)
	{
		ft_memcpy(&stack->v[i * 3], &((t_elem*)l->content)->v,
		sizeof(t_vec3) * 3);
		ft_memcpy(&stack->vn[i * 3], &((t_elem*)l->content)->vn,
		sizeof(t_vec3) * 3);
		ft_memcpy(&stack->vt[i++ * 3], &((t_elem*)l->content)->vt,
		sizeof(t_vec2) * 3);
		l = l->next;
	}
	stack->v_id = create_buffer(stack->v, sizeof(t_vec3) * stack->n_f * 3);
	stack->vn_id = create_buffer(stack->vn, sizeof(t_vec3) * stack->n_f * 3);
	stack->vt_id = create_buffer(stack->vt, sizeof(t_vec2) * stack->n_f * 3);
	ft_lstdel(&list->f, (void*)&free);
}

void	treat_params(t_env *env, t_buffer *list, char **params, size_t len)
{
	t_stack	stack;

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
		stack.mat = get_mat(list->mat, params[1]);
		ft_lstadd(&env->stack, ft_lstnew(&stack, sizeof(t_stack)));
	}
	else if (!ft_strcmp(params[0], "f") && (len == 4 || len == 5))
		add_elem(list, params, len);
	else
		error(100, params[0], 0);
}

/*
**	0:vertex 1:normals 2:textures 3:elems
*/

void	load_obj(t_env *env, char *file)
{
	t_buffer	list;
	size_t		len;
	char		**array;
	char		*line;
	int			fd;

	if ((fd = open(file, O_RDONLY)) == -1)
		error(2, file, 1);
	ft_bzero(&list, sizeof(t_buffer));
	while (ft_get_next_line(fd, &line) == 1)
	{
		array = ft_multisplit(line, SEPARATORS);
		len = ft_arraylen(array);
		if (len > 0 && array[0][0] != '#')
			treat_params(env, &list, array, len);
		ft_free_array((void**)array, len + 1);
		free(line);
	}
	if (env->stack)
		alloc_stack(env, &list);
	ft_lstdel(&list.v, (void*)&free);
	ft_lstdel(&list.vt, (void*)&free);
	ft_lstdel(&list.vn, (void*)&free);
	close(fd);
}
