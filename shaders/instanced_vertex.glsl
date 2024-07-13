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
out vec3 normal;
out vec3 terrainNormal;
out vec3 worldPosition;
out vec4 Color;

#include "variables.glsl"
#include "transformation.glsl"
#include "functions.glsl"

float random (vec2 st)
{
    return fract(sin(dot(st.xy * 0.001, vec2(12.9898,78.233))) * 43758.5453123);
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
    normal = vec3(0, 0, -1);
	float ran = random(data[gl_InstanceID].pos.xz + vec2(data[gl_InstanceID].pos.y, -data[gl_InstanceID].pos.y));
    mat4 rotation = rotationMatrix(vec3(1.0, 0.0, 0.0), radians(ran * 60.0 * (iPosition.y)));
	float scale = 1.0 - pow(clamp(SquaredDistanceToViewPosition(data[gl_InstanceID].pos), 0.0, 10000.0) * 0.0001, 3);
	vec3 position = iPosition * scale;
    position = (rotation * vec4(position, 1.0)).xyz;
    normal = (rotation * vec4(normal, 0.0)).xyz;
	ran = random(vec2(data[gl_InstanceID].pos.xz + vec2(data[gl_InstanceID].pos.y + ran * 10, -data[gl_InstanceID].pos.y + ran * 10)));
    rotation = rotationMatrix(vec3(0.0, 1.0, 0.0), radians(ran * 360.0));
    position = (rotation * vec4(position, 1.0)).xyz + data[gl_InstanceID].pos;
    normal = (rotation * vec4(normal, 0.0)).xyz;

	worldPosition = ObjectToWorld(position);
    gl_Position = projection * view * vec4(worldPosition, 1.0);

	UV = vec2(iPosition.x * 10 + 0.5, iPosition.y);

	terrainNormal = data[gl_InstanceID].norm;

    Color = vec4(0.25, 0.6, 0.1, 1.0);
}