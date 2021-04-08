#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float time;

out float Height;
void main()
{
	float loc_y = sin(time * 2 + aPos.x * 25.0f) * 0.05f;
	gl_Position = projection * view * model * vec4(aPos.x, loc_y, aPos.z, 1.0);

	Height = (loc_y * 20 + 1) / 2;
}