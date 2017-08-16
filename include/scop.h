/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/10 14:55:02 by pribault          #+#    #+#             */
/*   Updated: 2017/08/15 18:19:57 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCOP_H
# define SCOP_H

# include <sys/time.h>
# include <GL/gl3.h>
# include <GL/gl.h>
# include <SDL2/SDL.h>
# include <SDL2/SDL_image.h>
# include "ft_printf.h"
# include "vector.h"
# include "matrix.h"

# define SEPARATORS	"\a\b\t\n\v\f\r "
# define ANGLE		0.02
# define STEP		0.001

# pragma pack(1)

typedef struct		s_win
{
	SDL_Window		*win;
	SDL_GLContext	context;
	SDL_Event		events;
	char			*name;
	Uint16			width;
	Uint16			height;
	SDL_DisplayMode	mode;
}					t_win;

typedef struct		s_color
{
	Uint8			r;
	Uint8			g;
	Uint8			b;
	Uint8			a;
}					t_color;

typedef struct		s_light
{
	t_vec3			pos;
	t_vec3			color;
	t_vec3			i;
}					t_light;

typedef struct		s_shaders
{
	GLuint			vertex;
	GLuint			fragment;
	GLuint			program;
}					t_shaders;

typedef struct		s_elem
{
	t_vec3			v[3];
	t_vec3			vn[3];
	t_vec2			vt[3];
}					t_elem;

typedef struct		s_texture
{
	char			*name;
	Uint16			w;
	Uint16			h;
	t_color			*img;
	GLuint			id;
}					t_texture;

typedef struct		s_mat
{
	char			*name;
	t_texture		*texture[3];
	t_vec3			ambient;
	t_vec3			diffuse;
	t_vec3			specular;
	t_type			reflection;
	t_type			alpha;
}					t_mat;

typedef struct		s_stack
{
	t_mat			*mat;
	size_t			n_f;
	t_vec3			*v;
	t_vec2			*vt;
	t_vec3			*vn;
	GLuint			v_id;
	GLuint			vt_id;
	GLuint			vn_id;
}					t_stack;

typedef struct		s_buffer
{
	t_list			*v;
	t_list			*vt;
	t_list			*vn;
	t_list			*f;
	t_list			*mat;
	t_list			*texture;
}					t_buffer;

typedef struct		s_env
{
	t_win			win;
	Uint8			stop;
	t_shaders		shaders;
	t_light			light;
	GLuint			light_id;
	t_list			*stack;
	t_vec3			pos;
	t_vec3			center;
	double			angle;
	char			*path;
}					t_env;

t_env		*init_env(void);

void		print_list(t_list *list);

void		load_obj(t_env *env, char *file);
t_texture	*load_bmp(char *file);
void		load_mtllib(t_buffer *list, char *file);

char		*get_path(char *file);
t_mat		*get_mat(t_list *list, char *name);
void		get_numbers(char *params, size_t *n);
t_list		*get_listn(t_list *list, size_t n);
size_t		ft_listlen(t_list *list);

void		error(int error, void *param, char state);

GLuint		create_buffer(void *data, size_t size);
GLuint		create_image_buffer(t_texture *texture);

#endif