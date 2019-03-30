/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_mtl.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/14 17:04:40 by pribault          #+#    #+#             */
/*   Updated: 2019/03/30 12:47:54 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

t_texture	*load_png(char *name)
{
	SDL_Surface	*surface;
	t_texture	*new;

	if (!(surface = IMG_Load(name)) ||
		!(new = (t_texture *)malloc(sizeof(t_texture))) ||
		!(new->img = (t_c *)malloc(sizeof(t_c) * surface->w * surface->h)) ||
		!(new->name = ft_strdup(name)))
	{
		error(1, NULL, 1);
		return (NULL);
	}
	new->w = surface->w;
	new->h = surface->h;
	invert_texture(new, surface);
	SDL_FreeSurface(surface);
	return (new);
}

t_texture	*get_texture(t_vector *vec, char *name, char *file)
{
	char		*path;
	t_texture	*current;
	size_t		i;

	path = get_path(file);
	current = NULL;
	if (ft_strlen(path))
		name = ft_joinf("%s/%s", get_path(file), name);
	else
		name = ft_strdup(name);
	i = (size_t)-1;
	while (++i < vec->n && (current = ft_vector_get(vec, i)))
		if (!ft_strcmp(name, current->name))
			return (current);
	if (ft_strlen(name) < 4)
		return (NULL);
	if (!ft_strcmp(name + ft_strlen(name) - 4, ".bmp"))
		current = load_bmp(name);
	else if (!ft_strcmp(name + ft_strlen(name) - 4, ".png"))
		current = load_png(name);
	if (current)
		current->id = create_image_buffer(current);
	free(name);
	return (current);
}

void		mtl_treat_2(t_buffer *list, char **params, char *file, size_t len)
{
	if (!ft_strcmp(params[0], "map_Ka") && len == 2 && list->mat.n)
		((t_mat*)ft_vector_get(&list->mat, list->mat.n - 1))->texture[0] =
			get_texture(&list->texture, params[1], file);
	else if (!ft_strcmp(params[0], "map_Kd") && len == 2 && list->mat.n)
		((t_mat*)ft_vector_get(&list->mat, list->mat.n - 1))->texture[1] =
			get_texture(&list->texture, params[1], file);
	else if (!ft_strcmp(params[0], "map_Ks") && len == 2 && list->mat.n)
		((t_mat*)ft_vector_get(&list->mat, list->mat.n - 1))->texture[2] =
			get_texture(&list->texture, params[1], file);
	else
		error(100, params[0], 0);
}

void		mtl_treat(t_buffer *list, char **params, char *file, size_t len)
{
	t_mat	new;

	if (!ft_strcmp(params[0], "newmtl") && len == 2)
	{
		ft_bzero(&new, sizeof(t_mat));
		new.name = ft_strdup(params[1]);
		ft_vector_add(&list->mat, &new);
	}
	else if (!ft_strcmp(params[0], "Ka") && len == 4 && list->mat.n)
		((t_mat*)ft_vector_get(&list->mat, list->mat.n - 1))->ambient =
		new_vec3(ft_atof(params[1]), ft_atof(params[2]), ft_atof(params[3]));
	else if (!ft_strcmp(params[0], "Kd") && len == 4 && list->mat.n)
		((t_mat*)ft_vector_get(&list->mat, list->mat.n - 1))->diffuse =
		new_vec3(ft_atof(params[1]), ft_atof(params[2]), ft_atof(params[3]));
	else if (!ft_strcmp(params[0], "Ks") && len == 4 && list->mat.n)
		((t_mat*)ft_vector_get(&list->mat, list->mat.n - 1))->specular =
		new_vec3(ft_atof(params[1]), ft_atof(params[2]), ft_atof(params[3]));
	else
		mtl_treat_2(list, params, file, len);
}

void		load_mtllib(t_buffer *list, char *file)
{
	struct stat	buf;
	size_t		len;
	char		**params;
	char		*line;
	int			fd;

	if ((fd = open(file, O_RDONLY)) == -1 || fstat(fd, &buf) ||
		(buf.st_mode & S_IFMT) != S_IFREG)
		error(2, file, 1);
	while (ft_get_next_line(fd, &line) == 1)
	{
		params = ft_multisplit(line, SEPARATORS);
		len = ft_arraylen(params);
		if (len > 0 && params[0][0] != '#')
			mtl_treat(list, params, file, len);
		ft_free_array((void**)params, len + 1);
		free(line);
	}
	free(line);
	free(file);
	close(fd);
}
