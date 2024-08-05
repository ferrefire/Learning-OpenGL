#ifndef CULLING_INCLUDED
#define CULLING_INCLUDED

#include "variables.glsl"
#include "transformation.glsl"
#include "heightmap.glsl"
#include "functions.glsl"
#include "depth.glsl"

uniform sampler2D depthMap;

int InView(vec3 position, vec3 tolerance)
{
    vec3 clipSpacePosition = WorldToClip(position);

    //if (clipSpacePosition.z > far) return (0);

    //return (clipSpacePosition.x < -tolerance.x || clipSpacePosition.x > 1.0 + tolerance.x ||
    //    clipSpacePosition.y < -tolerance.y || clipSpacePosition.y > 1.0 + tolerance.y ||
    //    clipSpacePosition.z < -tolerance.z) ? 0 : 1;

	bool xInView = clipSpacePosition.x > 0.0 - tolerance.x && clipSpacePosition.x < 1.0 + tolerance.x;
	bool yInView = clipSpacePosition.y > 0.0 - tolerance.y && clipSpacePosition.y < 1.0 + tolerance.y;
	bool zInView = clipSpacePosition.z > 0.0 && clipSpacePosition.z < far;

	return ((xInView && yInView && zInView) ? 1 : 0);
}

int InView(vec3 position, float tolerance)
{
    return InView(position, vec3(tolerance, tolerance, 0));
}

int AreaInView(vec3 position, vec2 areaSize)
{
	vec3 rightOffset = viewRight * areaSize.x;
	vec3 upOffset = viewUp * areaSize.y;
	float inDistance = pow((areaSize.x * areaSize.y) * 2, 2);

	if (SquaredDistanceToViewPosition(position) <= inDistance) return (1);

	vec3 areaPosition = position - rightOffset - upOffset;
    if (InView(areaPosition, 0) == 1) return (1);

	areaPosition = position + rightOffset + upOffset;
    if (InView(areaPosition, 0) == 1) return (1);

	areaPosition = position + rightOffset - upOffset;
    if (InView(areaPosition, 0) == 1) return (1);

	areaPosition = position - rightOffset + upOffset;
    if (InView(areaPosition, 0) == 1) return (1);

	return (0);
}

float MapOccluded(vec3 position)
{
	float occluded = 0;
	
	vec3 clipSpacePosition = WorldToClip(position);

	float depth = 1.0 - (clipSpacePosition.z * farMult);
	//float mapValue = textureLod(depthMap, clipSpacePosition.xy, 0).r;

	ivec2 screenUv = ivec2(0);
	screenUv.x = int(floor(clipSpacePosition.x * width));
	screenUv.y = int(floor(clipSpacePosition.y * height));
	float mapValue = GetDepthMap(screenUv);

	return (depth - mapValue);
}

int AreaOccluded(vec3 position, vec2 areaSize)
{
	vec3 rightOffset = viewRight * areaSize.x;
	vec3 upOffset = viewUp * areaSize.y;
	//float inDistance = pow((areaSize.x * areaSize.y) * 2, 2);

	//if (SquaredDistanceToViewPosition(position) <= inDistance) return (0);

	vec3 areaPosition = position - rightOffset - upOffset;
    if (MapOccluded(areaPosition) > 0) return (0);

	areaPosition = position + rightOffset + upOffset;
    if (MapOccluded(areaPosition) > 0) return (0);

	areaPosition = position + rightOffset - upOffset;
    if (MapOccluded(areaPosition) > 0) return (0);

	areaPosition = position - rightOffset + upOffset;
    if (MapOccluded(areaPosition) > 0) return (0);

	return (1);
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