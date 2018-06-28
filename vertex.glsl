#version 410 core

layout(location = 0) in vec3	v;
layout(location = 1) in vec2	vtin;
layout(location = 2) in vec3	vnin;

out vec2	vt;
out vec3	vn;
out vec3	pos;

uniform mat4	MVP;
uniform vec3	position;
uniform vec3	size;
uniform vec4	quaternion;

vec3	rotate(vec3 vec, vec4 quat)
{
	vec3	new;

	new.x = vec.x * (quat.w * quat.w + quat.x * quat.x - quat.y * quat.y -
		quat.z * quat.z) + vec.y * 2 * (quat.x * quat.y - quat.w * quat.z) +
		vec.z * 2 * (quat.x * quat.z + quat.w * quat.y);
	new.y = vec.x * 2 * (quat.x * quat.y + quat.w * quat.z) +
		vec.y * (quat.w * quat.w - quat.x * quat.x + quat.y * quat.y -
		quat.z * quat.z) + vec.z * 2 * (quat.y * quat.z - quat.w * quat.x);
	new.z = vec.x * 2 * (quat.x * quat.z - quat.w * quat.y) +
		vec.y * 2 * (quat.y * quat.z + quat.w * quat.x) +
		vec.z * (quat.w * quat.w - quat.x * quat.x - quat.y * quat.y +
		quat.z * quat.z);
	return (new);
}

void main()
{
	pos = rotate(v, quaternion) * size + position;
	gl_Position = MVP * vec4(pos, 1);
	vt = vtin;
	vn = rotate(vnin, quaternion);
}
