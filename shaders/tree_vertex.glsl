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

layout(std430, binding = 7) buffer iLod0Data
{
    datastruct lod0Data[];
};

layout(std430, binding = 8) buffer oLod0Count
{
    uint lod0Count;
};

layout(std430, binding = 9) buffer iLod1Data
{
    datastruct lod1Data[];
};

layout(std430, binding = 10) buffer oLod1Count
{
    uint lod1Count;
};

layout(std430, binding = 11) buffer iLod2Data
{
    datastruct lod2Data[];
};

layout(std430, binding = 12) buffer oLod2Count
{
    uint lod2Count;
};

out vec2 UV;
out vec3 normal;
out vec3 worldPosition;

float spacing = 100;
float spacingMult = 0.01;

#include "variables.glsl"
#include "transformation.glsl"
#include "functions.glsl"

uniform int instanceCount;
uniform float instanceMult;
uniform int instanceCountSqrt;
uniform float instanceCountSqrtMult;

uniform int lod;
uniform vec3 computeViewPosition;

float random (vec2 st)
{
    return fract(sin(dot(st.xy * 0.001, vec2(12.9898,78.233))) * 43758.5453123);
}

void main()
{
	vec3 pos = vec3(0);
	float rot = 0;

    //pos.xz = unpackHalf2x16(data[gl_InstanceID].posxz) + computeViewPosition.xz;
	//vec2 yy = unpackHalf2x16(data[gl_InstanceID].posyroty);
	//pos.y = yy.x + computeViewPosition.y;
	//rot = yy.y;

	if (lod == 2)
	{
		pos.xz = unpackHalf2x16(lod2Data[gl_InstanceID].posxz) + computeViewPosition.xz;
		vec2 yy = unpackHalf2x16(lod2Data[gl_InstanceID].posyroty);
		pos.y = yy.x + computeViewPosition.y;
		rot = yy.y;
	}
	else if (lod == 1)
	{
		pos.xz = unpackHalf2x16(lod1Data[gl_InstanceID].posxz) + computeViewPosition.xz;
		vec2 yy = unpackHalf2x16(lod1Data[gl_InstanceID].posyroty);
		pos.y = yy.x + computeViewPosition.y;
		rot = yy.y;
	}
	else if (lod == 0)
	{
		pos.xz = unpackHalf2x16(lod0Data[gl_InstanceID].posxz) + computeViewPosition.xz;
		vec2 yy = unpackHalf2x16(lod0Data[gl_InstanceID].posyroty);
		pos.y = yy.x + computeViewPosition.y;
		rot = yy.y;
	}
	else
	{
		worldPosition = ObjectToWorld(iPosition) + vec3(0, 2000, 0);
    	gl_Position = projection * view * vec4(worldPosition, 1.0);
		UV = iUV;
		normal = iNormal;
		return ;
	}

	float squaredDistance = SquaredDistanceToViewPosition(pos);
	float maxDistance = pow(instanceCountSqrt * spacing, 2);
	float maxDistanceMult = pow(instanceCountSqrtMult * spacingMult, 2);

	float ran;
	ran = rot;

    vec3 position = iPosition;
    normal = iNormal;

    mat4 rotation = GetRotationMatrix(radians(ran), vec3(0.0, 1.0, 0.0));
    position = (rotation * vec4(position, 1.0)).xyz;
    normal = (rotation * vec4(normal, 0.0)).xyz;

	worldPosition = ObjectToWorld(position) + pos;
    gl_Position = projection * view * vec4(worldPosition, 1.0);

	UV = iUV;
}