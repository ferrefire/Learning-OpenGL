#version 460 core

#define COMPUTE_STAGE

layout (local_size_x = 4, local_size_y = 4) in;

layout(r16_snorm, binding = 0) uniform image2D shadowMap;

uniform vec2 offset;
uniform vec2 worldOffset;
uniform vec2 seed;
uniform int resolution;
uniform float resolutionMult;
uniform float mapScale;

#include "variables.glsl"
#include "heightmap.glsl"

float RayInShadow(vec3 position)
{
	vec3 rayPosition = position;
	float i = 0;
	float occluded = 0;

	while (rayPosition.y < terrainHeight * 0.25 && occluded == 0)
	{
		rayPosition += lightDirection * (terrainShadowSize * resolutionMult);
		if (SampleDynamic(rayPosition.xz) * terrainHeight >= rayPosition.y) occluded = 0.8;
	}

	return occluded;
}

void main()
{
	//if (gl_GlobalInvocationID.x > resolution || gl_GlobalInvocationID.y > resolution) return ;

	vec2 index = gl_GlobalInvocationID.xy;
	vec2 uv = (index * resolutionMult - 0.5) * terrainShadowSize;
	vec3 position = vec3(uv.x, SampleDynamic(uv) * terrainHeight, uv.y);

    imageStore(shadowMap, ivec2(index), vec4(1.0 - RayInShadow(position)));
}