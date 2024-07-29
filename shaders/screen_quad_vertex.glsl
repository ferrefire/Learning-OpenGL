#version 460 core

#define VERTEX_STAGE

layout (location = 0) in vec3 iPosition;

out vec2 UV;

void main()
{
	UV = iPosition.xy + 0.5;
	vec3 position = iPosition;
	//position.x *= 0.625;
	//position.y *= 1.111;
    gl_Position = vec4(position, 1.0);
}