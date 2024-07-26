#version 460 core

#define COMPUTE_STAGE

layout (local_size_x = 4, local_size_y = 4) in;

layout(r16_snorm, binding = 0) uniform image2D heightMap;

uniform vec2 offset = vec2(0.0);
uniform vec2 worldOffset = vec2(0, 0);
uniform vec2 seed = vec2(0, 0);
uniform uint resolution = 1024;
uniform float resolutionMult = 0.0009765625;
uniform float mapScale = 1.0;

#include "noise.glsl"

void main()
{
	if (gl_GlobalInvocationID.x >= resolution || gl_GlobalInvocationID.y >= resolution) return ;

	vec2 index = gl_GlobalInvocationID.xy;
	vec2 uv = index * resolutionMult - 0.5 + offset;
	uv *= mapScale;
    imageStore(heightMap, ivec2(index), vec4(GenerateNoise(uv + worldOffset + seed, noiseLayers)));
}