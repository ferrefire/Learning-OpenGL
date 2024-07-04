#version 460 core

#define VERTEX_STAGE

layout (location = 0) in vec3 iPosition;
layout (location = 1) in vec2 iUV;
layout (location = 2) in vec3 iNormal;

out vec2 UV;
out vec3 Normal;
out vec3 FragmentPosition;
out vec4 Color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 viewPosition;
uniform vec4 color;

void main()
{
    gl_Position = projection * view * model * vec4(iPosition, 1.0);
    FragmentPosition = (model * vec4(iPosition, 1.0)).xyz;
	UV = iUV;
	Normal =  (model * vec4(iNormal, 0.0)).xyz;
    Color = color;
}