#version 460 core

#define COMPUTE_STAGE

layout (local_size_x = 32, local_size_y = 32) in;

struct datastruct
{
    vec3 pos;
    //vec4 col;
	vec3 norm;
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
uniform float instanceMult;
uniform int instanceCountSqrt;

uniform sampler2D heightMap;

#include "culling.glsl"
#include "noise.glsl"
#include "heightmap.glsl"

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
    float x = float(gl_GlobalInvocationID.x) - instanceCountSqrt * 0.5;
    float z = float(gl_GlobalInvocationID.y) - instanceCountSqrt * 0.5;
    float indexDis = max(abs(x), abs(z));
    x = x * 0.5 + floor(viewPosition.x);
    z = z * 0.5 + floor(viewPosition.z);

    vec2 uv = vec2(x, z) * 0.0001 + 0.5;
    float falloff = float(indexDis) / float(instanceCountSqrt * 0.5);
	vec3 norm = SampleNormal(uv, 1);
	float ran = GetRandom(float(x + z * instanceCountSqrt) * instanceMult);
    if (GetSteepness(norm) > 0.5 + (ran - 0.5) * 0.5 || falloff > pow(ran, 3)) return ;
	//if (GetSteepness(GenerateNoiseNormal(uv, noiseLayers, 0.001)) > 0.5) return ;

    //float y = GenerateNoise(uv, noiseLayers) * noiseHeight + 1.5;
    float y = texture(heightMap, uv).r * noiseHeight + 1.5;

    vec3 pos = GetRandomVec3(float(x + z * instanceCountSqrt) * instanceMult);
    //vec3 pos = vec3(0);
    vec3 position = vec3(x, y, z) + pos;
    if (InView(position, 0.1) == 0) return ;
    index = atomicAdd(computeCount, 1);
    data[index].pos = position;
	data[index].norm = norm;
    //data[index].col = vec4(0.25, 0.6, 0.1, 1.0);
    
}