#version 460 core

#define COMPUTE_STAGE

layout (local_size_x = 4, local_size_y = 4) in;

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

uniform int instanceCount;
uniform float instanceMult;
uniform int instanceCountSqrt;
uniform float instanceCountSqrtMult;

#include "variables.glsl"
#include "culling.glsl"
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
    x = x * 0.25 + floor(viewPosition.x);
    z = z * 0.25 + floor(viewPosition.z);

    vec2 uv = vec2(x, z) * 0.0001 + 0.5;
    float falloff = float(indexDis) * float(instanceCountSqrtMult * 2.0);
	float ran = GetRandom(float(x + z * instanceCountSqrt) * instanceMult);
	vec3 norm = SampleNormalDynamic(vec3(x, 0, z), 0.25);
    float steepness = GetSteepness(norm);
    steepness = 1.0 - pow(1.0 - steepness, 15);
	//vec3 norm = SampleNormalUnNorm(uv);
	//vec3 steepnessNormal = norm;
	//norm.xz *= 0.25;
	//norm = normalize(norm);
	//steepnessNormal.xz *= 0.5;
	//steepnessNormal = normalize(steepnessNormal);
    if (steepness > 0.5 + (ran - 0.5) * 0.5 || falloff > pow(ran, 3)) return ;
	//if (GetSteepness(GenerateNoiseNormal(uv, noiseLayers, 0.001)) > 0.5) return ;

    //float y = GenerateNoise(uv, noiseLayers) * noiseHeight + 1.5;
    float y = SampleDynamic(vec2(x, z)) * heightMapHeight;

    vec3 pos = GetRandomVec3(float(x + z * instanceCountSqrt) * instanceMult);
    pos.y = 0;
    //vec3 pos = vec3(0);
    vec3 position = vec3(x, y, z) + pos;
    if (InView(position + vec3(0, 0.5, 0), 0.1) == 0) return ;
    index = atomicAdd(computeCount, 1);
    data[index].pos = position;
	data[index].norm = norm;
    //data[index].col = vec4(0.25, 0.6, 0.1, 1.0);
    
}