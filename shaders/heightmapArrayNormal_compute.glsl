#version 460 core

#define COMPUTE_STAGE

layout (local_size_x = 4, local_size_y = 4) in;

layout(rgba16_snorm, binding = 0) uniform image2DArray heightMapArrayNormal;

uniform vec2 offset;
uniform vec2 seed;
uniform int resolution;
uniform float resolutionMult;
uniform int chunksRadius;

#include "noise.glsl"

vec3 SampleArrayNormal(vec2 uv, vec2 indexUV)
{
    //GenerateNoise((vec2(uv.x, uv.y) * resolutionMult - 0.5 + vec2(xi, yi)) + offset + seed, noiseLayers)

	float left = GenerateNoise((vec2(uv.x - 0.1, uv.y) * resolutionMult - 0.5 + indexUV) + offset + seed, noiseLayers);
    float right = GenerateNoise((vec2(uv.x + 0.1, uv.y) * resolutionMult - 0.5 + indexUV) + offset + seed, noiseLayers);
    float down = GenerateNoise((vec2(uv.x, uv.y - 0.1) * resolutionMult - 0.5 + indexUV) + offset + seed, noiseLayers);
    float up = GenerateNoise((vec2(uv.x, uv.y + 0.1) * resolutionMult - 0.5 + indexUV) + offset + seed, noiseLayers);

    vec3 normalTS = vec3((left - right) / 0.0002, 1, (down - up) / 0.0002);
    //normalTS.xz *= 0.25;

    return (normalize(normalTS));
}

void main()
{
	int index = 0;

	for (int xi = -chunksRadius; xi <= chunksRadius; xi++)
	{
		for (int yi = -chunksRadius; yi <= chunksRadius; yi++)
		{
			imageStore(heightMapArrayNormal, ivec3(gl_GlobalInvocationID.xy, index), 
                vec4(SampleArrayNormal(vec2(gl_GlobalInvocationID.xy), vec2(xi, yi)), 1.0));

			index++;
		}
	}
}