#version 460 core

#define VERTEX_STAGE

layout (location = 0) in vec3 iPosition;
layout (location = 1) in vec2 iUV;
layout (location = 2) in vec3 iNormal;

struct datastruct
{
    uint posxz;
	uint posyroty;
};

layout(std430, binding = 7) buffer iData
{
    datastruct data[];
};

out vec2 UV;
out vec3 normal;
out vec3 worldPosition;

float spacing = 10.0;
float spacingMult = 0.1;

#include "variables.glsl"
#include "transformation.glsl"
#include "functions.glsl"

uniform int instanceCount;
uniform float instanceMult;
uniform int instanceCountSqrt;
uniform float instanceCountSqrtMult;

uniform int lod;
uniform vec3 computeViewPosition;
uniform vec3 computeViewPositionLod;

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
	vec3 pos = vec3(0);
	float rot = 0;

    pos.xz = unpackHalf2x16(data[gl_InstanceID].posxz) + computeViewPosition.xz;
	vec2 yy = unpackHalf2x16(data[gl_InstanceID].posyroty);
	pos.y = yy.x + computeViewPosition.y;
	rot = yy.y;

	//if (lod == 0)
	//{
	//	pos.xz = unpackHalf2x16(data[gl_InstanceID].posxz) + computeViewPosition.xz;
	//	norm.xz = unpackHalf2x16(data[gl_InstanceID].normxz);
	//	vec2 yy = unpackHalf2x16(data[gl_InstanceID].posynormy);
	//	pos.y = yy.x + computeViewPosition.y;
	//	norm.y = yy.y;
	//}
	//else
	//{
	//	pos.xz = unpackHalf2x16(lodData[gl_InstanceID].posxz) + computeViewPositionLod.xz;
	//	norm.xz = unpackHalf2x16(lodData[gl_InstanceID].normxz);
	//	vec2 yy = unpackHalf2x16(lodData[gl_InstanceID].posynormy);
	//	pos.y = yy.x + computeViewPositionLod.y;
	//	norm.y = yy.y;
	//}

	float squaredDistance = SquaredDistanceToViewPosition(pos);
	float maxDistance = pow(instanceCountSqrt * spacing, 2);
	float maxDistanceMult = pow(instanceCountSqrtMult * spacingMult, 2);

	float ran;
	ran = rot;

    vec3 position = iPosition;
    normal = iNormal;

    rotation = rotationMatrix(vec3(0.0, 1.0, 0.0), radians(ran));
    position = (rotation * vec4(position, 1.0)).xyz;
    normal = (rotation * vec4(normal, 0.0)).xyz;

	worldPosition = ObjectToWorld(position) + pos;
    gl_Position = projection * view * vec4(worldPosition, 1.0);

	UV = iUV;
}