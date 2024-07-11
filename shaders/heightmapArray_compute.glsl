#version 460 core

#define COMPUTE_STAGE

layout (local_size_x = 4, local_size_y = 4) in;

layout(r16_snorm, binding = 1) uniform image2DArray heightMapArray;

uniform vec2 offset = vec2(5, 5);
uniform float resolution = 1024;
uniform float scale = 4;
uniform float sizeMultiplier;

uniform int chunksRadius;

#include "noise.glsl"

void main()
{
    ivec2 texCoord = ivec2(gl_GlobalInvocationID.xy);
	int index = 0;

	for (int xi = -chunksRadius; xi <= chunksRadius; xi++)
	{
		for (int yi = -chunksRadius; yi <= chunksRadius; yi++)
		{
			imageStore(heightMapArray, ivec3(texCoord, index), vec4(GenerateNoise((vec2(gl_GlobalInvocationID.x, gl_GlobalInvocationID.y) + 
				vec2(xi, yi)) * sizeMultiplier * scale + offset, 10)));

			index++;
		}
	}
}