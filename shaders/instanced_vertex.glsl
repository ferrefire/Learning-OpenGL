#version 460 core
layout (location = 0) in vec3 iPosition;
layout (location = 1) in vec2 iUV;
layout (location = 2) in vec3 iNormal;

struct datastruct
{
    vec3 pos;
    vec4 col;
};

layout(std430, binding = 3) buffer iColors
{
    datastruct data[];
};

out vec2 UV;
out vec3 Normal;
out vec3 FragmentPosition;
out vec4 Color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 viewPosition;
//uniform vec4 color;

uniform int instanceCount;
uniform int instanceCountSqrt;

float random (vec2 st)
{
    return fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43758.5453123);
}

void main()
{
    //int x = gl_InstanceID / instanceCountSqrt;
    //int z = gl_InstanceID - x * instanceCountSqrt;
    //int y = 5;

    //gl_Position = projection * view * model * vec4(iPosition + vec3(x, y, z), 1.0);
    //FragmentPosition = (model * vec4(iPosition + vec3(x, y, z), 1.0)).xyz;

    gl_Position = projection * view * model * vec4(iPosition + data[gl_InstanceID].pos, 1.0);
    FragmentPosition = (model * vec4(iPosition + data[gl_InstanceID].pos, 1.0)).xyz;

	UV = iUV;
	Normal =  (model * vec4(iNormal, 0.0)).xyz;
    //float ran = random(vec2(x, z));
    //Color = vec4(ran, random(vec2(ran, 1.0)), random(vec2(1.0, ran)), 1.0);
    Color = data[gl_InstanceID].col;
}