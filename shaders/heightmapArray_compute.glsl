#version 460 core

#define COMPUTE_STAGE

layout (local_size_x = 4, local_size_y = 4) in;

layout(r16_snorm, binding = 0) uniform image2DArray heightMapArray;

uniform vec2 offset;
uniform vec2 seed;
uniform int resolution;
uniform float resolutionMult;
uniform int chunksRadius;
uniform int chunkIndex = 0;
uniform vec2 chunkCoordinates = vec2(0);

#include "noise.glsl"

void main()
{
	/*int index = 0;

	for (int xi = -chunksRadius; xi <= chunksRadius; xi++)
	{
		for (int yi = -chunksRadius; yi <= chunksRadius; yi++)
		{
			imageStore(heightMapArray, ivec3(gl_GlobalInvocationID.xy, index), 
				vec4(GenerateNoise((vec2(gl_GlobalInvocationID.x, gl_GlobalInvocationID.y) * resolutionMult - 0.5 + vec2(xi, yi)) 
					+ offset + seed, noiseLayers)));

			index++;
		}
	}*/

	imageStore(heightMapArray, ivec3(gl_GlobalInvocationID.xy, chunkIndex), 
				vec4(GenerateNoise((vec2(gl_GlobalInvocationID.x, gl_GlobalInvocationID.y) * resolutionMult - 0.5 + chunkCoordinates) 
					+ offset + seed, noiseLayers)));
}