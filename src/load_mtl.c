/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_mtl.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/14 17:04:40 by pribault          #+#    #+#             */
/*   Updated: 2017/08/15 15:46:35 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

char	*get_path(char *file)
{
	int		i;
	char	*path;

	i = ft_strlen(file) - 1;
	while (i > 0 && file[i] != '/')
		i--;
	if (file[i] == '/')
	{
		path = (char*)malloc(i + 1);
		ft_memcpy(path, file, i);
		path[i] = '\0';
	}
	else
		return ("");
	return (path);
}

void	invert(t_color *c, Uint8 bpp)
{
	Uint8	tmp;

	tmp = c->r;
	c->r = c->b;
	c->b = tmp;
	if (bpp == 3)
		c->a = 255;
}

t_texture	*load_png(char *name)
{
	SDL_Surface	*surface;
	t_texture	*new;
	Uint8		bpp;
	size_t		i[2];

	if (!(surface = IMG_Load(name)))
		return (NULL);
	bpp = surface->format->BytesPerPixel;
	if (!(new = (t_texture*)malloc(sizeof(t_texture))))
		error(1, NULL, 1);
	new->name = ft_strdup(name);
	new->w = surface->w;
	new->h = surface->h;
	if (!(new->img = (t_color*)malloc(sizeof(t_color) * new->w * new->h)))
		error(1, NULL, 1);
	ft_bzero(new->img, sizeof(t_color) * new->w * new->h);
	i[0] = 0;
	while (i[0] < new->h)
	{
		i[1] = 0;
		while (i[1] < new->w)
		{
			ft_memcpy(&new->img[i[0] * new->w + i[1]], &((Uint8*)surface->pixels)[((new->h - i[0] - 1) * new->w + i[1]) * bpp], bpp);
			invert(&new->img[i[0] * new->w + i[1]], bpp);
			i[1]++;
		}
		i[0]++;
	}
	SDL_FreeSurface(surface);
	return (new);
}

t_texture	*get_texture(t_list *list, char *name, char *file)
{
	char		*path;
	t_texture	*current;

	path = get_path(file);
	if (ft_strlen(path))
		name = ft_joinf("%s/%s", get_path(file), name);
	else
		name = ft_strdup(name);
	while (list)
	{
		current = list->content;
		if (!ft_strcmp(name, current->name))
			return (current);
		list = list->next;
	}
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

void	mtl_treat_2(t_buffer *list, char **params, char *file, size_t len)
{
	if (!ft_strcmp(params[0], "map_Ka") && len == 2 && list->mat)
		((t_mat*)list->mat->content)->texture[0] = get_texture(list->texture,
		params[1], file);
	else if (!ft_strcmp(params[0], "map_Kd") && len == 2 && list->mat)
		((t_mat*)list->mat->content)->texture[1] = get_texture(list->texture,
		params[1], file);
	else if (!ft_strcmp(params[0], "map_Ks") && len == 2 && list->mat)
		((t_mat*)list->mat->content)->texture[2] = get_texture(list->texture,
		params[1], file);
	else
		error(100, params[0], 0);
}

void	mtl_treat(t_buffer *list, char **params, char *file, size_t len)
{
	t_mat	new;

	if (!ft_strcmp(params[0], "newmtl") && len == 2)
	{
		ft_bzero(&new, sizeof(t_mat));
		new.name = ft_strdup(params[1]);
		ft_lstadd(&list->mat, ft_lstnew(&new, sizeof(t_mat)));
	}
	else if (!ft_strcmp(params[0], "Ka") && len == 4 && list->mat)
		((t_mat*)list->mat->content)->ambient = new_vec3(ft_atof(params[1]),
		ft_atof(params[2]), ft_atof(params[3]));
	else if (!ft_strcmp(params[0], "Kd") && len == 4 && list->mat)
		((t_mat*)list->mat->content)->diffuse = new_vec3(ft_atof(params[1]),
		ft_atof(params[2]), ft_atof(params[3]));
	else if (!ft_strcmp(params[0], "Ks") && len == 4 && list->mat)
		((t_mat*)list->mat->content)->specular = new_vec3(ft_atof(params[1]),
		ft_atof(params[2]), ft_atof(params[3]));
	else
		mtl_treat_2(list, params, file, len);
}

void	load_mtllib(t_buffer *list, char *file)
{
	size_t	len;
	char	**params;
	char	*line;
	int		fd;

	if ((fd = open(file, O_RDONLY)) == -1)
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