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
uniform sampler2D	depthMap;
uniform mat3		light;
uniform mat4		lightView;
uniform mat4		lightProjection;
uniform vec3		camera;

void	main()
{
	vec4	color_ambient = texture(ka, vt).rgba;
	vec4	color_diffuse = texture(kd, vt).rgba;
	vec4	color_specular = texture(ks, vt).rgba;
	vec3	reflected = normalize(normalize(pos - camera) - 2 * vn * dot(normalize(pos - camera), vn));
	float	diffuse = dot(normalize(light[POS] - pos), vn);
	float	specular = pow(dot(normalize(camera - pos), reflected), ALPHA);
	vec4	depthPos = lightProjection * lightView * vec4(pos, 1);
	float	depth = texture(depthMap, depthPos.xy).a;

	if (depth < depthPos.z - 0.01)
	{
		diffuse = 0;
		specular = 0;
	}
	if (diffuse < 0 || isinf(diffuse) || isnan(diffuse))
	{
		diffuse = 0;
		specular = 0;
	}
	else if (specular < 0 || isinf(specular) || isnan(specular))
		specular = 0;

	color = light[COLOR][AMBIENT] * light[INTENSITY][AMBIENT] * color_ambient +
			light[COLOR][DIFFUSE] * light[INTENSITY][DIFFUSE] * color_diffuse * diffuse +
			light[COLOR][SPECULAR] * light[INTENSITY][SPECULAR] * color_specular * specular;

	color.a = 1;
}
