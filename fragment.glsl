#version 330 core

in vec2		vt;
in vec3		vn;

out vec4	color;

uniform sampler2D	ka;
uniform sampler2D	kd;
uniform sampler2D	ks;
uniform mat3		light;

void main()
{
	vec4	color_a = texture(ka, vt).rgba;
	vec4	color_d = texture(kd, vt).rgba;
	vec4	color_s = texture(ks, vt).rgba;
	float	angle = dot(vec3(light[0]), vn);

	color.a = 1 - ((1 - color_a.a) * light[2][0] + (1 - color_d.a) * light[2][1]);
	color.r = (color_a.r * light[2][0] * light[1][0]) + (color_d.r * light[2][1] * light[1][0] * angle);
	color.g = (color_a.g * light[2][0] * light[1][1]) + (color_d.g * light[2][1] * light[1][1] * angle);
	color.b = (color_a.b * light[2][0] * light[1][2]) + (color_d.b * light[2][1] * light[1][2] * angle);
}
