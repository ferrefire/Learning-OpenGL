#version 460 core

#define COMPUTE_STAGE

layout (local_size_x = 4, local_size_y = 4) in;

struct datastruct
{
    vec3 pos;
	vec3 norm;
    vec2 rot;
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

float spacing = 0.25;
float spacingMult = 4;

#include "variables.glsl"
#include "culling.glsl"
#include "heightmap.glsl"
#include "functions.glsl"

float random(vec2 st)
{
    return fract(sin(dot(st.xy * 0.001, vec2(12.9898,78.233))) * 43758.5453123);
}

void main()
{
    if (gl_GlobalInvocationID.x >= instanceCountSqrt || gl_GlobalInvocationID.y >= instanceCountSqrt) return ;
    uint index = gl_GlobalInvocationID.x + gl_GlobalInvocationID.y * instanceCountSqrt;
    float x = float(gl_GlobalInvocationID.x) - instanceCountSqrt * 0.5;
    float z = float(gl_GlobalInvocationID.y) - instanceCountSqrt * 0.5;

    x = x * spacing + floor(viewPosition.x);
    z = z * spacing + floor(viewPosition.z);

	//if (MapOccluded(vec3(x, 0, z)) == 1) return ;

    float y = SampleDynamic(vec2(x, z)) * heightMapHeight;
    
    float falloff = SquaredDistanceToViewPosition(vec3(x, y, z)) * pow(instanceCountSqrtMult * 2.0 * spacingMult, 2);
	if (InView(vec3(x, y, z) + vec3(0, 0.5, 0), pow(1.0 - falloff, 2) * 0.5) == 0) return ;
	falloff = falloff * 1.025 - 0.025;
    falloff = pow(falloff, 0.175);

	
	//vec3 norm = SampleNormalDynamic(vec2(x, z), 0.25);
    //float steepness = GetSteepness(norm);
    //steepness = 1.0 - pow(1.0 - steepness, 15);

    float ranMult = 1.0 - (abs(x) + abs(z)) * terrainSizeMult * 0.5;
    float ran = random(vec2(x, z) * ranMult);

    if (falloff > ran) return ;
	
	if (RayOccluded(vec3(x, y, z)) == 1) return ;

    vec3 position = vec3(x, y, z);
    ran = random(vec2(ran * 100, ran * 200));
	position.x += ran - 0.5;
    ran = random(vec2(ran * 200, ran * 100));
	position.z += ran - 0.5;
	position.y = SampleDynamic(position.xz) * heightMapHeight;
    vec3 norm = SampleNormalDynamic(position.xz, 0.25);

    float steepness = GetSteepness(norm);
    steepness = 1.0 - pow(1.0 - steepness, 15);
    if (steepness > 0.125 + (ran - 0.5) * 0.25) return ;
    
    //if (InView(position + vec3(0, 0.5, 0), 0.1) == 0) return ;
    index = atomicAdd(computeCount, 1);
    data[index].pos = position;
	data[index].norm = norm;
    vec2 rotations = vec2(0);
    float wave = sin(time * 2 + (x + z) * 0.1) * 0.5 + 0.5;
    //float wave = 0;
    ran = random(position.xz * ranMult + vec2(position.y, -position.y) * 0.01);
    //rotations.x = mix(ran, (ran * 0.5 + 2.0) * 0.5, wave);
    rotations.x = mix(0.25, 1.0, ran);
	rotations.x = mix(rotations.x, 1.0 + (ran - 0.5) * 0.25, wave);
    ran = random(vec2(position.xz * ranMult + vec2(position.y + ran * 10, -position.y + ran * 10) * 0.01));
    rotations.y = mix(ran, (ran * 0.5 + 1.8) * 0.5, wave);
    rotations.x *= 60.0;
    rotations.y *= 360.0;
	data[index].rot = rotations;  
}