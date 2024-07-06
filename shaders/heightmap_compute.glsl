#version 460 core

#define COMPUTE_STAGE

layout (local_size_x = 4, local_size_y = 4) in;

layout(r16_snorm, binding = 0) uniform image2D heightMap;

uniform vec2 offset = vec2(5, 5);
uniform float resolution = 1024;
uniform float scale = 4;
uniform float sizeMultiplier;

#include "noise.glsl"

void main()
{
    ivec2 texCoord = ivec2(gl_GlobalInvocationID.xy);

    //imageStore(heightMap, ivec2(0, 0), vec4(1));
    //imageStore(heightMap, texCoord, vec4(GenerateNoise(vec2(gl_GlobalInvocationID.xy) / 1024.0, 7)));
    imageStore(heightMap, texCoord, vec4(GenerateNoise(vec2(gl_GlobalInvocationID.x, gl_GlobalInvocationID.y) * 
		sizeMultiplier * scale + offset, 10)));
}