/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_mtl_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/29 13:51:28 by pribault          #+#    #+#             */
/*   Updated: 2019/06/18 18:17:02 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

char		*get_path(char *file)
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
		return (ft_strdup(""));
	return (path);
}

void		invert_pixel(t_c *c, Uint8 bpp)
{
	Uint8	tmp;

	tmp = c->r;
	c->r = c->b;
	c->b = tmp;
	if (bpp == 3)
		c->a = 255;
}

void		invert_texture(t_texture *texture, SDL_Surface *surface)
{
	size_t	i;
	size_t	j;
	Uint8	bpp;

	bpp = surface->format->BytesPerPixel;
	i = (size_t)-1;
	while (++i < texture->h)
	{
		j = (size_t)-1;
		while (++j < texture->w)
		{
			ft_memcpy(&texture->img[i * texture->w + j],
				&((Uint8*)surface->pixels)[((texture->h - i - 1) * texture->w +
					j) * bpp], bpp);
			invert_pixel(&texture->img[i * texture->w + j], bpp);
		}
	}
}
