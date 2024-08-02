#ifndef TRANSFORMATION_INCLUDED
#define TRANSFORMATION_INCLUDED

#include "variables.glsl"

vec4 WorldToView(vec3 position)
{
    vec4 viewSpace = view * vec4(position, 1.0);

    return (viewSpace);
}

vec3 WorldToClip(vec3 position)
{
    vec4 viewSpace = projection * WorldToView(position);

    vec3 clipSpace = viewSpace.xyz;
    clipSpace /= viewSpace.w;

    clipSpace.x = clipSpace.x * 0.5 + 0.5;
    clipSpace.y = clipSpace.y * 0.5 + 0.5;
    clipSpace.z = viewSpace.w;

    return (clipSpace);
}

//vec4 WorldToClip(vec3 position)
//{
//    vec4 clipSpace = projection * WorldToView(position);
//
//    return (clipSpace);
//}

vec3 ObjectToWorld(vec3 position)
{
    vec3 WorldSpace = (model * vec4(position, 1.0)).xyz;

    return (WorldSpace);
}

vec2 WorldToUV(vec3 worldPosition)
{
	return vec2(worldPosition.xz * terrainSizeMult + 0.5);
}

vec2 ObjectToUV(vec3 objectPosition)
{
	vec3 worldPosition = ObjectToWorld(objectPosition);

	return vec2(worldPosition.xz * terrainSizeMult + 0.5);
}

mat4 GetRotationMatrix(float angle, vec3 axis)
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

vec3 Rotate(vec3 vec, float angle, vec3 axis)
{
	return (GetRotationMatrix(angle, axis) * vec4(vec, 1.0)).xyz;
}

#endif