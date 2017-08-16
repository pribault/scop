/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmp.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/14 14:12:16 by pribault          #+#    #+#             */
/*   Updated: 2017/08/14 21:50:56 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BMP_H
# define BMP_H

# include <SDL2/SDL.h>
# include "ft_printf.h"

# define MAGIC	0x4d42

# pragma pack(1)

typedef struct	s_color
{
	Uint8		r;
	Uint8		g;
	Uint8		b;
	Uint8		a;
}				t_color;

typedef struct	s_texture
{
	char		*name;
	Uint16		w;
	Uint16		h;
	t_color		*img;
}				t_texture;

typedef struct	s_file_header
{
	Uint16		magic;
	Uint32		size;
	Uint32		reserved;
	Uint32		offset;
}				t_file_header;

typedef struct	s_dib_header
{
	Uint32		size;
	Uint32		width;
	Uint32		height;
	Uint16		color_planes;
	Uint16		bpp;
	Uint32		compression;
	Uint32		raw_size;
	int			hres;
	int			vres;
	Uint32		colors;
	Uint32		importants;
}				t_dib_header;

void		error(int error, void *param, char state);

#endif
