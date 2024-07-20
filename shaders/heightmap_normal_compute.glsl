#version 460 core

#define COMPUTE_STAGE

layout (local_size_x = 4, local_size_y = 4) in;

layout(rgba16_snorm, binding = 0) uniform image2D heightMapNormal;

uniform vec2 offset;
uniform vec2 worldOffset;
uniform vec2 seed;
uniform int resolution;
uniform float resolutionMult;
uniform float mapScale;

#include "noise.glsl"

vec3 SampleNormal(vec2 uv)
{
	float left = GenerateNoise(((uv - vec2(0.1 / mapScale, 0)) *resolutionMult - 0.5 + offset) * mapScale + worldOffset + seed, noiseLayers);
    float right = GenerateNoise(((uv + vec2(0.1 / mapScale, 0)) *resolutionMult - 0.5 + offset) * mapScale + worldOffset + seed, noiseLayers);
    float down = GenerateNoise(((uv - vec2(0, 0.1 / mapScale)) *resolutionMult - 0.5 + offset) * mapScale + worldOffset + seed, noiseLayers);
    float up = GenerateNoise(((uv + vec2(0, 0.1 / mapScale)) *resolutionMult - 0.5 + offset) * mapScale + worldOffset + seed, noiseLayers);

    vec3 normalTS = vec3((left - right) / 0.0002, 1, (down - up) / 0.0002);

    return (normalize(normalTS));
}

void main()
{
	//if (gl_GlobalInvocationID.x > resolution || gl_GlobalInvocationID.y > resolution) return ;

	vec2 uv = vec2(gl_GlobalInvocationID.x, gl_GlobalInvocationID.y) * resolutionMult - 0.5 + offset;
	uv *= mapScale;

    imageStore(heightMapNormal, ivec2(gl_GlobalInvocationID.xy), vec4(SampleNormal(vec2(gl_GlobalInvocationID.xy)), 1.0));
}