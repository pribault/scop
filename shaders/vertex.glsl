#version 410 core

layout(location = 0) in vec3	v;
layout(location = 1) in vec2	vtin;
layout(location = 2) in vec3	vnin;

out vec2	vt;
out vec3	vn;
out vec3	pos;

uniform mat4	model;
uniform mat4	view;
uniform mat4	projection;
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

mat4	quatToMat(vec4 quat)
{
	mat4	new;

	new[0][0] = pow(quat.w, 2) + pow(quat.x, 2) - pow(quat.y, 2) - pow(quat.z, 2);
	new[0][1] = 2 * quat.x * quat.y - 2 * quat.w * quat.z;
	new[0][2] = 2 * quat.x * quat.z + 2 * quat.w * quat.y;
	new[1][0] = 2 * quat.x * quat.y + 2 * quat.w * quat.z;
	new[1][1] = pow(quat.w, 2) - pow(quat.x, 2) + pow(quat.y, 2) - pow(quat.z, 2);
	new[1][2] = 2 * quat.y * quat.z - 2 * quat.w * quat.x;
	new[2][0] = 2 * quat.x * quat.z - 2 * quat.w * quat.y;
	new[2][1] = 2 * quat.y * quat.z + 2 * quat.w * quat.x;
	new[2][2] = pow(quat.w, 2) - pow(quat.x, 2) - pow(quat.y, 2) + pow(quat.z, 2);
	new[3][3] = 1;
	return (new);
}

vec4	getSize(mat4 model)
{
	vec4	result;

	result.x = model[0][0];
	result.y = model[1][1];
	result.z = model[2][2];
	result.w = model[3][3];
	return (result);
}

void	main()
{
	pos = (vec4(rotate((vec4(v, 1) * getSize(model)).xyz, quaternion), 1) + model[3]).xyz;
	gl_Position = projection * view * vec4(pos, 1);
	vt = vtin;
	vn = rotate(vnin, quaternion);
}
