#version 460 core
layout (local_size_x = 8, local_size_y = 8) in;

struct datastruct
{
    vec3 pos;
    vec4 col;
};

layout(std430, binding = 3) buffer iColors
{
    datastruct data[];
};

layout(std430, binding = 4) buffer oComputeCount
{
    uint computeCount;
};

uniform mat4 view;
uniform mat4 projection;

uniform vec3 viewPosition;

uniform float time;
uniform float far;
uniform float near;

uniform int instanceCount;
uniform int instanceCountSqrt;

#include "culling.glsl"
#include "noise.glsl"

float GetRandom(float x)
{
    return (fract(sin(x * 10000)*100000.0));
}

vec3 GetRandomVec3(float i)
{
    float x = GetRandom(i);
    float y = GetRandom(x);
    float z = GetRandom(y);

    return (vec3(x, y, z));
}

void main()
{
    if (gl_GlobalInvocationID.x >= instanceCountSqrt || gl_GlobalInvocationID.y >= instanceCountSqrt) return ;
    uint index = gl_GlobalInvocationID.x + gl_GlobalInvocationID.y * instanceCountSqrt;
    //uint index = gl_GlobalInvocationID;
    int x = int(gl_GlobalInvocationID.x);
    int y = 5;
    int z = int(gl_GlobalInvocationID.y);

    vec3 pos = GetRandomVec3(float(index) / instanceCount);
    //vec3 position = vec3(x, 5, z) + pos * (sin(time + index / (instanceCountSqrt * 50.0)) * 0.5 + 0.5) * 100;
    //float perlinNoise = snoise(vec2(x, z) / instanceCountSqrt * 10) * 100;
    float perlinNoise = GenerateNoise(vec2(x, z) / instanceCountSqrt * 2, 7) * 500;
    perlinNoise = floor(perlinNoise);
    vec3 position = vec3(x, perlinNoise, z);
    //if (dot(viewPosition - position, viewPosition - position) > (far * far) * 1.25 || InView(position, 0.0) == 0) return ;
    if (InView(position, 0.1) == 0) return ;
    //data[gl_GlobalInvocationID.x].pos = ((sin(time + float(gl_GlobalInvocationID.x) / float(gl_NumWorkGroups.x) * 25) * 0.5 + 0.5) * pos * 100) + vec3(x, y, z);
    index = atomicAdd(computeCount, 1);
    data[index].pos = position;
    //data[index].col = vec4(GetRandomVec3(length(pos)), 1.0);
    data[index].col = vec4(0.2, 0.75, 0.2, 1.0);
    
    //data[index].col = vec4(vec3(1.0 - float(index) / instanceCount), 1.0);
}