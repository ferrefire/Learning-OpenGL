#ifndef TRANSFORMATION_INCLUDED
#define TRANSFORMATION_INCLUDED

#include "variables.glsl"

vec3 WorldToClip(vec3 position)
{
    vec4 viewSpace = view * vec4(position, 1.0);

    vec3 clipSpace = viewSpace.xyz;
    clipSpace /= -viewSpace.w;

    clipSpace.x = clipSpace.x * 0.5f + 0.5f;
    clipSpace.y = clipSpace.y * 0.5f + 0.5f;
    clipSpace.z = viewSpace.w;

    return (clipSpace);
}

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

#endif