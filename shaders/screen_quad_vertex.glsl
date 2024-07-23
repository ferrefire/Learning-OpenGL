#version 460 core

#define VERTEX_STAGE

layout (location = 0) in vec3 iPosition;

out vec2 UV;

void main()
{
	UV = iPosition.xy + 0.5;
    gl_Position = vec4(iPosition, 1.0);
}