#version 410 core

#define RED		0
#define GREEN	1
#define BLUE	2

#define AMBIENT		0
#define DIFFUSE		1
#define SPECULAR	2

#define POS			0
#define COLOR		1
#define INTENSITY	2

#define ALPHA		5

in vec2		vt;
in vec3		vn;

out vec4	color;

uniform sampler2D	ka;
uniform sampler2D	kd;
uniform sampler2D	ks;
uniform mat3		light;
uniform vec3		position;
uniform vec3		camera;

void	main()
{
	vec4	color_ambient = texture(ka, vt).rgba;
	vec4	color_diffuse = texture(kd, vt).rgba;
	vec4	color_specular = texture(ks, vt).rgba;
	vec3	reflected = normalize(normalize(position - camera) - 2 * vn * dot(normalize(position - camera), vn));
	float	diffuse = dot(normalize(light[POS] - position), vn);
	float	specular = pow(dot(normalize(camera - position), reflected), ALPHA);

	if (diffuse < 0)
	{
		diffuse = 0;
		specular = 0;
	}
	else if (specular < 0)
		specular = 0;
	color.a = 1 -
		(1 - color_ambient.a) * light[INTENSITY][AMBIENT];
	color.r =
		color_ambient.r * light[INTENSITY][AMBIENT] * light[COLOR][RED] +
		color_diffuse.r * light[INTENSITY][DIFFUSE] * light[COLOR][RED] * diffuse +
		color_specular.r * light[INTENSITY][SPECULAR] * light[COLOR][RED] * specular;
	color.g =
		color_ambient.g * light[INTENSITY][AMBIENT] * light[COLOR][GREEN] +
		color_diffuse.g * light[INTENSITY][DIFFUSE] * light[COLOR][GREEN] * diffuse +
		color_specular.g * light[INTENSITY][SPECULAR] * light[COLOR][GREEN] * specular;
	color.b =
		color_ambient.b * light[INTENSITY][AMBIENT] * light[COLOR][BLUE] +
		color_diffuse.b * light[INTENSITY][DIFFUSE] * light[COLOR][BLUE] * diffuse +
		color_specular.b * light[INTENSITY][SPECULAR] * light[COLOR][BLUE] * specular;
}
