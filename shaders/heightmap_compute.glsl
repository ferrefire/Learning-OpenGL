#version 460 core

#define COMPUTE_STAGE

layout (local_size_x = 4, local_size_y = 4) in;

layout(r16_snorm, binding = 0) uniform image2D heightMap;

uniform vec2 offset;
uniform vec2 seed;
uniform int resolution;
uniform float resolutionMult;

#include "noise.glsl"

void main()
{
	//if (gl_GlobalInvocationID.x > resolution || gl_GlobalInvocationID.y > resolution) return ;

    imageStore(heightMap, ivec2(gl_GlobalInvocationID.xy), vec4(GenerateNoise(vec2(gl_GlobalInvocationID.x, gl_GlobalInvocationID.y) * 
		resolutionMult + offset + seed, noiseLayers)));
}