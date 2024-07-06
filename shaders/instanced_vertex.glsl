#version 460 core

#define VERTEX_STAGE

layout (location = 0) in vec3 iPosition;
//layout (location = 1) in vec2 iUV;
//layout (location = 2) in vec3 iNormal;

struct datastruct
{
    vec3 pos;
    //vec4 col;
	vec3 norm;
};

layout(std430, binding = 3) buffer iColors
{
    datastruct data[];
};

out vec2 UV;
out vec3 Normal;
out vec3 TerrainNormal;
out vec3 FragmentPosition;
out vec4 Color;

uniform mat4 model;

uniform sampler2D heightMap;

#include "variables.glsl"
#include "heightmap.glsl"

float random (vec2 st)
{
    return fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43758.5453123);
}

mat4 rotationMatrix(vec3 axis, float angle)
{
    //axis = normalize(axis);
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
    vec3 normal = vec3(0, 0, -1);
    mat4 rotation = rotationMatrix(vec3(1.0, 0.0, 0.0), radians(random(data[gl_InstanceID].pos.xz) * 45.0 * (iPosition.y + 1)));
    vec3 position = (rotation * vec4(iPosition, 1.0)).xyz;
    normal = (rotation * vec4(normal, 0.0)).xyz;
    rotation = rotationMatrix(vec3(0.0, 1.0, 0.0), radians(random(data[gl_InstanceID].pos.xz + data[gl_InstanceID].pos.y) * 360.0));
    position = (rotation * vec4(position, 1.0)).xyz + data[gl_InstanceID].pos;
    normal = (rotation * vec4(normal, 0.0)).xyz;
    //vec3 position = iPosition + data[gl_InstanceID].pos;

    gl_Position = projection * view * model * vec4(position, 1.0);
    FragmentPosition = (model * vec4(position, 1.0)).xyz;

	UV = vec2(iPosition.x * 10 + 0.5, iPosition.y);
	//Normal = (model * vec4(iNormal, 0.0)).xyz;
	//Normal = vec3(0.0, 1.0, 0.0);

	//Normal = GenerateNoiseNormal(position.xz * 0.001, 7, 0.001);
	//Normal = SampleNormal(position.xz * 0.0001 + 0.5, 1);
	//Normal = data[gl_InstanceID].norm;
	TerrainNormal = data[gl_InstanceID].norm;
	Normal = normal;
	
    //float ran = random(vec2(x, z));
    //Color = vec4(ran, random(vec2(ran, 1.0)), random(vec2(1.0, ran)), 1.0);
    //Color = data[gl_InstanceID].col;
    Color = vec4(0.25, 0.6, 0.1, 1.0);
}