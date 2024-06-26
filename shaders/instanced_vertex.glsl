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

//uniform int instanceCount;
//uniform int instanceCountSqrt;

//#define PI    3.14159265358
//#define TWOPI 6.28318530718

#include "noise.glsl"

float random (vec2 st)
{
    return fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43758.5453123);
}

mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

void main()
{
    //int x = gl_InstanceID / instanceCountSqrt;
    //int z = gl_InstanceID - x * instanceCountSqrt;
    //int y = 5;

    //gl_Position = projection * view * model * vec4(iPosition + vec3(x, y, z), 1.0);
    //FragmentPosition = (model * vec4(iPosition + vec3(x, y, z), 1.0)).xyz;

    //float angle = acos(dot(data[gl_InstanceID].pos.xz, viewPosition.xz) / (length(data[gl_InstanceID].pos.xz) * length(viewPosition.xz)));
    mat4 rotation = rotationMatrix(vec3(1.0, 0.0, 0.0), radians(random(data[gl_InstanceID].pos.xz) * 25.0 * (iUV.y + 1)));
    vec3 position = (rotation * vec4(iPosition, 1.0)).xyz;
    rotation = rotationMatrix(vec3(0.0, 1.0, 0.0), radians(random(data[gl_InstanceID].pos.xz + data[gl_InstanceID].pos.y) * 360.0));
    position = (rotation * vec4(position, 1.0)).xyz + data[gl_InstanceID].pos;
    //vec3 position = iPosition + data[gl_InstanceID].pos;

    gl_Position = projection * view * model * vec4(position, 1.0);
    FragmentPosition = (model * vec4(position, 1.0)).xyz;

	UV = iUV;
	//Normal = (model * vec4(iNormal, 0.0)).xyz;
	//Normal = vec3(0.0, 1.0, 0.0);
	Normal = GenerateNoiseNormal(position.xz * 0.001, 7, 0.001);
    //float ran = random(vec2(x, z));
    //Color = vec4(ran, random(vec2(ran, 1.0)), random(vec2(1.0, ran)), 1.0);
    Color = data[gl_InstanceID].col;
}