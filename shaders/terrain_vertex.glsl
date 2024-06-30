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

#include "noise.glsl"

void main()
{
    UV = iPosition.xz * 0.001;
    vec3 position = iPosition;
    position.y = GenerateNoise(UV, 7) * 500;
    gl_Position = projection * view * model * vec4(position, 1.0);
    FragmentPosition = (model * vec4(position, 1.0)).xyz;
	Normal = GenerateNoiseNormal(UV, 7, 0.001);
    float steepness = GetSteepness(Normal);
    steepness = pow(steepness, 2);
    Color = mix(color, vec4(0.25, 0.25, 0.25, 1), steepness);
}