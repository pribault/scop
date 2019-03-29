/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/10 14:55:02 by pribault          #+#    #+#             */
/*   Updated: 2018/06/29 13:52:37 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCOP_H
# define SCOP_H

# include <sys/time.h>
# ifdef __APPLE__
#  include <OpenGL/gl3.h>
# else
#  ifdef __linux__
#   include <GL/glew.h>
#  endif
# endif
# include <SDL2/SDL.h>
# include <SDL2/SDL_image.h>
# include "ft_printf.h"
# include "vector.h"
# include "matrix.h"

# define SEPARATORS		"\a\b\t\n\v\f\r "
# define ANGLE			0.02
# define STEP			1
# define ROT_SPEED		0.02
# define PI				3.1415926535
# define FPS			60
# define LIGHT_STEP		0.01
# define COLOR_STEP		0.01
# define RESIZE_SPEED	0.01

typedef enum		e_input
{
	INPUT_LIGHT,
	INPUT_ROT,
	INPUT_MOVE,
	INPUT_COLOR,
	INPUT_SIZE,
	INPUT_MAX
}					t_input;

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

typedef struct		s_c
{
	Uint8			r;
	Uint8			g;
	Uint8			b;
	Uint8			a;
}					t_c;

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
	GLuint			depth_vertex;
	GLuint			depth_fragment;
	GLuint			depth_program;
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
	t_c				*img;
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
	t_vector		v;
	t_vector		vt;
	t_vector		vn;
	t_vector		f;
	t_vector		mat;
	t_vector		texture;
}					t_buffer;

typedef struct		s_env
{
	t_win			win;
	Uint8			stop;
	t_shaders		shaders;
	t_light			light;
	t_quat			quat;
	t_vec3			size;
	t_vec3			obj_pos;
	t_vec3			rot_speed;
	GLuint			vao;
	GLuint			framebuffer;
	GLuint			depth_map;
	t_buffer		buffer;
	t_vector		stack;
	t_vec3			pos;
	t_vec3			dir;
	t_input			input;
	GLenum			draw_mode;
	uint8_t			shadow;
	char			*path;
}					t_env;

void				init_env(t_env	*env);

void				events(t_env *env, SDL_Event *event);
int					move_event(t_env *env, SDL_Event *event);
int					color_event(t_env *env, SDL_Event *event);
int					resize_event(t_env *env, SDL_Event *event);
int					number_event(t_env *env, SDL_Event *event);

void				load_obj(t_env *env, char *file);
t_texture			*load_bmp(char *file);
void				load_mtllib(t_buffer *list, char *file);

char				*get_path(char *file);
t_mat				*get_mat(t_vector *vec, char *name);
void				add_vector(t_vector *vec, char **params, Uint8 n);
void				get_numbers(char *params, size_t *n);
void				invert_texture(t_texture *texture, SDL_Surface *surface);
void				check_materials(t_buffer *buffer);

void				error(int error, void *param, char state);

GLuint				create_buffer(void *data, size_t size);
GLuint				create_image_buffer(t_texture *texture);

#endif
