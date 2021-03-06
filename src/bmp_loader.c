/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmp_loader.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/14 13:13:08 by pribault          #+#    #+#             */
/*   Updated: 2018/06/26 11:49:08 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bmp.h"

t_texture	*invert_alpha(t_texture *new)
{
	Uint32	i;

	i = 0;
	while (i < (Uint32)(new->w * new->h))
	{
		new->img[i].a = 255 - new->img[i].a;
		i++;
	}
	return (new);
}

t_texture	*load_bmp(char *file)
{
	t_texture		*new;
	t_file_header	header;
	t_dib_header	dib;
	int				fd;

	if ((fd = open(file, O_RDONLY)) == -1)
		error(2, file, 1);
	if (read(fd, &header, sizeof(header)) != sizeof(header) ||
	header.magic != MAGIC)
		error(9, file, 1);
	if (read(fd, &dib, sizeof(dib)) != sizeof(dib) || dib.bpp != 32 ||
	dib.raw_size != dib.width * dib.height * dib.bpp / 8)
		error(9, file, 1);
	if (!(new = (t_texture*)malloc(sizeof(t_texture))))
		error(1, NULL, 1);
	if (!(new->img = (t_c*)malloc(dib.raw_size)))
		error(1, NULL, 1);
	lseek(fd, header.offset, SEEK_SET);
	new->name = ft_strdup(file);
	new->w = dib.width;
	new->h = dib.height;
	if (read(fd, (void*)new->img, dib.raw_size) != dib.raw_size)
		error(9, NULL, 1);
	close(fd);
	return (invert_alpha(new));
}
