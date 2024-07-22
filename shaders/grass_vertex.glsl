#version 460 core

#define VERTEX_STAGE

layout (location = 0) in vec3 iPosition;
//layout (location = 1) in vec2 iUV;
//layout (location = 2) in vec3 iNormal;

struct datastruct
{
    //vec3 pos;
	//vec3 norm;
	uint posxz;
	uint normxz;
	uint posynormy;
    //vec2 rot;
    uint rot;
};

layout(std430, binding = 3) buffer iData
{
    datastruct data[];
};

layout(std430, binding = 4) buffer iDataLod
{
    datastruct lodData[];
};

out vec2 UV;
out vec3 normal;
out vec3 terrainNormal;
out vec3 worldPosition;
out vec4 Color;
out float shadow;

float spacing = 0.25;
float spacingMult = 4;

#include "variables.glsl"
#include "transformation.glsl"
#include "functions.glsl"
#include "shadow.glsl"

uniform int lod;

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
    normal = normalize(mix(vec3(0, 0, -1), vec3(sign(iPosition.x) * 0.5, 0, 0), clamp(abs(iPosition.x) * 10, 0.0, 1.0)));

	vec2 rot;
	if (lod == 0) rot = unpackHalf2x16(data[gl_InstanceID].rot);
	else rot = unpackHalf2x16(lodData[gl_InstanceID].rot);

	vec3 pos = vec3(0);
	vec3 norm = vec3(0);
	if (lod == 0)
	{
		pos.xz = unpackHalf2x16(data[gl_InstanceID].posxz) + viewPosition.xz;
		norm.xz = unpackHalf2x16(data[gl_InstanceID].normxz);
		vec2 yy = unpackHalf2x16(data[gl_InstanceID].posynormy);
		pos.y = yy.x + viewPosition.y;
		norm.y = yy.y;
	}
	else
	{
		pos.xz = unpackHalf2x16(lodData[gl_InstanceID].posxz) + viewPosition.xz;
		norm.xz = unpackHalf2x16(lodData[gl_InstanceID].normxz);
		vec2 yy = unpackHalf2x16(lodData[gl_InstanceID].posynormy);
		pos.y = yy.x + viewPosition.y;
		norm.y = yy.y;
	}

	float ran;
	//if (lod == 0) ran = data[gl_InstanceID].rot.x;
	//else ran = lodData[gl_InstanceID].rot.x;
	ran = rot.x;

    mat4 rotation = rotationMatrix(vec3(1.0, 0.0, 0.0), radians(ran * (iPosition.y + 0.25)));
	//float scale = 1.0 - pow(clamp(SquaredDistanceToViewPosition(data[gl_InstanceID].pos), 0.0, 1000000) * 0.000001, 3);

	//vec3 pos;
	//if (lod == 0) pos = data[gl_InstanceID].pos;
	//else pos = lodData[gl_InstanceID].pos;

	float scale = clamp(SquaredDistanceToViewPosition(pos), 0.0, pow(2048 * 0.5 * spacing, 2)) * pow(0.000488281 * 2.0 * spacingMult, 2);
	scale = 1 + scale * 2;
	vec3 position = iPosition * scale;
    position = (rotation * vec4(position, 1.0)).xyz;
    normal = (rotation * vec4(normal, 0.0)).xyz;

	//if (lod == 0) ran = data[gl_InstanceID].rot.y;
	//else ran = lodData[gl_InstanceID].rot.y;
	ran = rot.y;

    rotation = rotationMatrix(vec3(0.0, 1.0, 0.0), radians(ran));
    position = (rotation * vec4(position, 1.0)).xyz;
    normal = (rotation * vec4(normal, 0.0)).xyz;

	worldPosition = ObjectToWorld(position) + pos;
    gl_Position = projection * view * vec4(worldPosition, 1.0);

	UV = vec2(iPosition.x * 10 + 0.5, iPosition.y);

	//if (lod == 0) terrainNormal = data[gl_InstanceID].norm;
	//else terrainNormal = lodData[gl_InstanceID].norm;
	terrainNormal = norm;

    Color = vec4(0.25, 0.6, 0.1, 1.0);
	shadow = RayInShadow(worldPosition);
	//Color.rgb *= RayInShadow(worldPosition);
}