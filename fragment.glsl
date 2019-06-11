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

vec4	multiply(vec4 a, vec4 b)
{
	return (vec4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w));
}

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

	color = vec4(light[COLOR], 1) *
		(light[INTENSITY][AMBIENT] * color_ambient +
		light[INTENSITY][DIFFUSE] * color_diffuse * diffuse +
		light[INTENSITY][SPECULAR] * color_specular * specular);

	color.a = 1;
}
