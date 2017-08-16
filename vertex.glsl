#version 330 core

layout(location = 0) in vec3	v;
layout(location = 1) in vec2	vtin;
layout(location = 2) in vec3	vnin;

out vec2	vt;
out vec3	vn;

uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4(v, 1);
	vt = vtin;
	vn = vnin;
}
