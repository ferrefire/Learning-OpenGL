#version 460 core
layout (location = 0) in vec3 iPosition;

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
	UV = iPosition.xz;
	Normal =  vec3(0.0, 1.0, 0.0);
    Color = color;
}