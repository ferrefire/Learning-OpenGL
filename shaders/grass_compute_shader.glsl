#version 460 core

#define COMPUTE_STAGE

layout (local_size_x = 4, local_size_y = 4) in;

struct datastruct
{
    //vec3 pos;
	uint posxz;
	//vec3 norm;
	uint normxz;
	uint posynormy;
	uint rot;
    //vec2 rot;
};

layout(std430, binding = 3) buffer iData
{
    datastruct data[];
};

layout(std430, binding = 4) buffer iDataLod
{
    datastruct lodData[];
};

layout(std430, binding = 5) buffer oCount
{
    uint count;
};

layout(std430, binding = 6) buffer oCountLod
{
    uint lodCount;
};

uniform int instanceCount;
uniform float instanceMult;
uniform int instanceCountSqrt;
uniform float instanceCountSqrtMult;
uniform float lodRange = 256.0;
uniform int specificLod = -1;

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

int Inside(vec2 pos)
{
	float dis = SquaredDistanceVec2(pos, vec2(0));
	if (dis > pow(instanceCountSqrt * 0.5, 2)) return (-1);
	else if (dis > pow(lodRange * 0.5, 2)) return (0);
	else return (1);
}

void main()
{
	if (gl_GlobalInvocationID.x >= instanceCountSqrt || gl_GlobalInvocationID.y >= instanceCountSqrt) return ;
	if (gl_GlobalInvocationID.x == 0 && gl_GlobalInvocationID.y == 0)
	{
		atomicExchange(count, 0);
		atomicExchange(lodCount, 0);
	}

	float x = float(gl_GlobalInvocationID.x) - instanceCountSqrt * 0.5;
    float z = float(gl_GlobalInvocationID.y) - instanceCountSqrt * 0.5;

	int lod = Inside(vec2(x, z));

	if (lod == specificLod || lod == -1) return ;

	//if (SquaredDistanceVec2(vec2(x, z), vec2(0)) > pow(instanceCountSqrt * 0.5, 2)) return ;
	//uint packedv2 = packHalf2x16(vec2(0.5, 1.75));

	//uint index = gl_GlobalInvocationID.x + gl_GlobalInvocationID.y * instanceCountSqrt;

	vec2 flooredViewPosition = vec2(floor(viewPosition.x * spacingMult) * spacing, floor(viewPosition.z * spacingMult) * spacing);
    x = x * spacing + flooredViewPosition.x;
    z = z * spacing + flooredViewPosition.y;
    float y = SampleDynamic(vec2(x, z)) * heightMapHeight;

	vec3 position = vec3(x, y, z);

	if (InView(position + vec3(0, 0.5, 0), 0) == 0) return ;

	//float depth = GetWorldDepth(position);

	if (MapOccluded(position) <= -0.0001) return ;
    
	float squaredDistance = SquaredDistanceToViewPosition(position);
	float maxDistance = pow(instanceCountSqrt * spacing, 2);
	float maxDistanceMult = pow(instanceCountSqrtMult * spacingMult, 2);

	float falloff = clamp(clamp(squaredDistance, 0.0, maxDistance) * maxDistanceMult, 0.0, 1.0);
	falloff = 1.0 - pow(1.0 - falloff, 16);
	//falloff = falloff * 1.025 - 0.025;
    //falloff = pow(falloff, 0.2);

    float ranMult = 1.0 - (abs(position.x) + abs(position.z)) * terrainSizeMult * 0.5;
    float ran = random(position.xz * ranMult);

    if (falloff > ran) return ;
	
	//if (MapOccluded(vec3(x, y + 0.5, z)) == 1) return ;
	//if (RayOccluded(vec3(x, y, z)) == 1) return ;

    ran = random(vec2(ran * 100, ran * 200));
	position.x += ran - 0.5;
    ran = random(vec2(ran * 200, ran * 100));
	position.z += ran - 0.5;
	position.y = SampleDynamic(position.xz) * heightMapHeight;
    vec3 norm = SampleNormalDynamic(position, 0.5);

    float steepness = GetSteepness(norm);
    steepness = 1.0 - pow(1.0 - steepness, 15);
    if (steepness > 0.5 + (ran - 0.5) * 0.25) return ;
    
    //if (InView(position + vec3(0, 0.5, 0), 0.1) == 0) return ;

    vec2 rotations = vec2(0);
    float wave = sin(time * 2 + (x + z) * 0.1) * 0.5 + 0.5;
    //float wave = 0;
    ran = random(position.xz * ranMult + vec2(position.y, -position.y) * 0.01);
    rotations.x = mix(0.25, 1.0, ran);
	rotations.x = mix(rotations.x, 1.0 + (ran - 0.5) * 0.25, wave);
    ran = random(vec2(position.xz * ranMult + vec2(position.y + ran * 10, -position.y + ran * 10) * 0.01));
    rotations.y = mix(ran, (ran * 0.5 + 1.8) * 0.5, wave);
    rotations.x *= 60.0;
    rotations.y *= 360.0;

	if (lod == 1)
	{
		uint index = atomicAdd(count, 1);
		data[index].posxz = packHalf2x16(position.xz - viewPosition.xz);
		data[index].normxz = packHalf2x16(norm.xz);
		data[index].posynormy = packHalf2x16(vec2(position.y - viewPosition.y, norm.y));
		data[index].rot = packHalf2x16(rotations);
	}
	else
	{
		uint index = atomicAdd(lodCount, 1);
		lodData[index].posxz = packHalf2x16(position.xz - viewPosition.xz);
		lodData[index].normxz = packHalf2x16(norm.xz);
		lodData[index].posynormy = packHalf2x16(vec2(position.y - viewPosition.y, norm.y));
		lodData[index].rot = packHalf2x16(rotations);
	}
}