#version 460 core

#define COMPUTE_STAGE

layout (local_size_x = 4, local_size_y = 4) in;

layout(r8_snorm, binding = 2) uniform image2D occlusionMap;

uniform int resolution;
uniform float resolutionMult;

#include "variables.glsl"
#include "functions.glsl"
#include "heightmap.glsl"
#include "transformation.glsl"

float Occluded(vec3 position)
{
	vec3 rayPosition = viewPosition;
	vec3 transitionPosition;
	//rayPosition.y += 0.5;
	//vec3 direction = (position - rayPosition) * 0.01;
	float transition = 0;

	float occluded = 0;

	//int iterations = 0;

	while (transition < 0.99 && occluded == 0)
	{
		//rayPosition += direction;
		transition += 0.01;
		transitionPosition = mix(rayPosition, position, transition);
		if (SampleDynamic(transitionPosition.xz) * terrainHeight > transitionPosition.y) occluded = 1;
		//iterations++;
	}

	return occluded;
}

void main()
{
	//uint index = gl_GlobalInvocationID.x + gl_GlobalInvocationID.y * instanceCountSqrt;
    float x = ((float(gl_GlobalInvocationID.x) * resolutionMult) - 0.5) * terrainOccludeSize + (viewPosition.x);
    float z = ((float(gl_GlobalInvocationID.y) * resolutionMult) - 0.5) * terrainOccludeSize + (viewPosition.z);
    float y = SampleDynamic(vec2(x, z)) * terrainHeight;
	
	vec3 position = vec3(x, y, z);

	float occluded = Occluded(position);

    imageStore(occlusionMap, ivec2(gl_GlobalInvocationID.xy), vec4(occluded));
}