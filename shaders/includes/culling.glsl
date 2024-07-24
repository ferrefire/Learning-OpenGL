#ifndef CULLING_INCLUDED
#define CULLING_INCLUDED

#include "variables.glsl"
#include "transformation.glsl"
#include "heightmap.glsl"
#include "functions.glsl"

uniform sampler2D occlusionMap;

int InView(vec3 position, float tolerance)
{
    vec4 viewSpace = projection * view * vec4(position, 1.0);

    vec3 clipSpace = viewSpace.xyz;
    clipSpace /= -viewSpace.w;

    clipSpace.x = clipSpace.x * 0.5f + 0.5f;
    clipSpace.y = clipSpace.y * 0.5f + 0.5f;
    clipSpace.z = viewSpace.w;

    if (clipSpace.z <= 0.0 || clipSpace.z > far) return (0);

    return (clipSpace.x < -tolerance || clipSpace.x > 1.0f + tolerance ||
        clipSpace.y < -tolerance || clipSpace.y > 1.0f + tolerance ||
        clipSpace.z <= -(tolerance * 0.5f)) ? 0 : 1;
}

float MapOccluded(vec3 position)
{
	vec2 uv = (position.xz - viewPosition.xz) * terrainOccludeSizeMult + 0.5;
	return textureLod(occlusionMap, uv, 0).r;
}

int RayOccluded(vec3 position)
{
	vec3 rayPosition;
	float i = 0;
	int occluded = 0;

	while (i <= 0.95 && occluded == 0)
	{
		i += 0.05;
		rayPosition = mix(position, viewPosition, i);
		if (SampleDynamic(rayPosition.xz) * terrainHeight >= rayPosition.y) occluded = 1; 
	}

	return occluded;
}

float NormalToViewDot(vec3 viewDirection, vec3 normal)
{
	return (1.0 - (dot(viewDirection, normal) * 0.5 + 0.5));
}

float PositionToViewDot(vec3 p1, vec3 p2, vec3 p3)
{
	vec3 p1cs = WorldToClip(p1);
	vec3 p2cs = WorldToClip(p2);
	vec3 p3cs = WorldToClip(p3);

	vec3 normal = cross(p2cs - p1cs, p3cs - p1cs);
	
	return (normal.z);
}

#endif