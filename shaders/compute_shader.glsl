#version 460 core
layout (local_size_x = 1) in;

struct datastruct
{
    vec3 pos;
    vec4 col;
};

layout(std430, binding = 3) buffer iColors
{
    datastruct data[];
};

uniform float time;
uniform float far;
uniform float near;
uniform int instanceCount;
uniform int instanceCountSqrt;

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
    int x = int(gl_GlobalInvocationID.x) / instanceCountSqrt;
    int z = int(gl_GlobalInvocationID.x) - x * instanceCountSqrt;
    int y = 5;

    vec3 pos = GetRandomVec3(float(gl_GlobalInvocationID.x) / float(gl_NumWorkGroups.x));
    data[gl_GlobalInvocationID.x].pos = ((sin(time + float(gl_GlobalInvocationID.x) / float(gl_NumWorkGroups.x) * 25) * 0.5 + 0.5) * pos * 100) + vec3(x, y, z);
    data[gl_GlobalInvocationID.x].col = vec4(GetRandomVec3(length(pos)), 1.0);
}