#ifndef SHADOW_INCLUDED
#define SHADOW_INCLUDED

uniform sampler2D shadowMap;

#include "variables.glsl"
#include "heightmap.glsl"
#include "functions.glsl"

float RayInShadow(vec3 position)
{
	return 1;

	vec3 rayPosition = position;
	vec3 lightPos = rayPosition + lightDirection * 100;
	float i = 0;
	float occluded = 0;
	float stepSize = clamp(SquaredDistanceToViewPosition(position), 1000.0, 100000.0) * 0.00001 * 0.25;

	while (i <= 1.0 - stepSize && rayPosition.y < terrainHeight * 0.25 && occluded == 0)
	{
		i += stepSize;
		rayPosition = mix(rayPosition, lightPos, i);
		float sampleHeight = SampleDynamic(rayPosition.xz) * terrainHeight;
		if (sampleHeight >= rayPosition.y) occluded = 1;
	}

	return 1.0 - occluded;
}

float InShadow(vec3 position)
{
	vec2 uv = (position.xz) * terrainShadowSizeMult;
	if (abs(uv.x) > 0.5 || abs(uv.y) > 0.5) return 1;
	return textureLod(shadowMap, uv + 0.5, 0).r;
}

#endif