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
in vec3		pos;

out vec4	color;

uniform sampler2D	ka;
uniform sampler2D	kd;
uniform sampler2D	ks;
uniform mat3		light;
uniform vec3		camera;

void	main()
{
	vec4	color_ambient = texture(ka, vt).rgba;
	vec4	color_diffuse = texture(kd, vt).rgba;
	vec4	color_specular = texture(ks, vt).rgba;
	vec3	reflected = normalize(normalize(pos - camera) - 2 * vn * dot(normalize(pos - camera), vn));
	float	diffuse = dot(normalize(light[POS] - pos), vn);
	float	specular = pow(dot(normalize(camera - pos), reflected), ALPHA);

	if (diffuse < 0 || isinf(diffuse) || isnan(diffuse))
	{
		diffuse = 0;
		specular = 0;
	}
	else if (specular < 0 || isinf(specular) || isnan(specular))
		specular = 0;

	color.r = light[COLOR][RED] * (
		color_ambient.r * light[INTENSITY][AMBIENT] +
		color_diffuse.r * light[INTENSITY][DIFFUSE] * diffuse +
		color_specular.r * light[INTENSITY][SPECULAR] * specular);
	color.g = light[COLOR][GREEN] * (
		color_ambient.g * light[INTENSITY][AMBIENT] +
		color_diffuse.g * light[INTENSITY][DIFFUSE] * diffuse +
		color_specular.g * light[INTENSITY][SPECULAR] * specular);
	color.b = light[COLOR][BLUE] * (
		color_ambient.b * light[INTENSITY][AMBIENT] +
		color_diffuse.b * light[INTENSITY][DIFFUSE] * diffuse +
		color_specular.b * light[INTENSITY][SPECULAR] * specular);

	color.a = 1;
}
