#version 460 core

#define COMPUTE_STAGE

layout (local_size_x = 4, local_size_y = 4) in;

struct datastruct
{
	uint posxz;
	uint posyroty;
};

layout(std430, binding = 7) buffer iData
{
    datastruct data[];
};

layout(std430, binding = 8) buffer oCount
{
    uint count;
};

uniform int instanceCount;
uniform float instanceMult;
uniform int instanceCountSqrt;
uniform float instanceCountSqrtMult;
uniform float lodRange = 256.0;

float spacing = 10;
float spacingMult = 0.1;

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
	return (abs(pos.x) < lodRange && abs(pos.y) < lodRange) ? 1 : 0;
}

void main()
{
	if (gl_GlobalInvocationID.x >= instanceCountSqrt || gl_GlobalInvocationID.y >= instanceCountSqrt) return ;

	float x = float(gl_GlobalInvocationID.x) - instanceCountSqrt * 0.5;
    float z = float(gl_GlobalInvocationID.y) - instanceCountSqrt * 0.5;

	//int lod = Inside(vec2(x, z));

	vec2 flooredViewPosition = vec2(floor(viewPosition.x), floor(viewPosition.z));
    x = x * spacing + flooredViewPosition.x;
    z = z * spacing + flooredViewPosition.y;

    float y = SampleDynamic(vec2(x, z)) * heightMapHeight;
    
	float squaredDistance = SquaredDistanceToViewPosition(vec3(x, y, z));

	float viewTolerance = 1.0 - clamp(squaredDistance, 0.0, 250.0) * 0.004;
	viewTolerance = pow(viewTolerance, 4);
	if (InView(vec3(x, y, z) + vec3(0, 0.5, 0), viewTolerance) == 0) return ;

	float maxDistance = pow(instanceCountSqrt * spacing, 2);
	float maxDistanceMult = pow(instanceCountSqrtMult * spacingMult, 2);

	float falloff = clamp(clamp(squaredDistance, 0.0, maxDistance) * maxDistanceMult, 0.0, 1.0);
	falloff = 1.0 - pow(1.0 - falloff, 16);
	//falloff = falloff * 1.025 - 0.025;
    //falloff = pow(falloff, 0.2);

    float ranMult = 1.0 - (abs(x) + abs(z)) * terrainSizeMult * 0.5;
    float ran = random(vec2(x, z) * ranMult);

    if (falloff > ran) return ;
	//if (RayOccluded(vec3(x, y, z)) == 1) return ;

    vec3 position = vec3(x, y, z);
    ran = random(vec2(ran * 100, ran * 200));
	position.x += (ran - 0.5) * 10.0;
    ran = random(vec2(ran * 200, ran * 100));
	position.z += (ran - 0.5) * 10.0;
	position.y = SampleDynamic(position.xz) * heightMapHeight;
    vec3 norm = SampleNormalDynamic(position, 0.5);

    float steepness = GetSteepness(norm);
    steepness = 1.0 - pow(1.0 - steepness, 15);
    if (steepness > 0.5 + (ran - 0.5) * 0.25) return ;
    
    //if (InView(position + vec3(0, 0.5, 0), 0.1) == 0) return ;

    float rotation = 0;
    //float wave = sin(time * 2 + (x + z) * 0.1) * 0.5 + 0.5;
    //float wave = 0;
    ran = random(position.xz * ranMult + vec2(position.y, -position.y) * 0.01);
    //rotations.x = mix(0.25, 1.0, ran);
	//rotations.x = mix(rotations.x, 1.0 + (ran - 0.5) * 0.25, wave);
    //ran = random(vec2(position.xz * ranMult + vec2(position.y + ran * 10, -position.y + ran * 10) * 0.01));
    rotation = ran * 360.0;

    uint index = atomicAdd(count, 1);
	data[index].posxz = packHalf2x16(position.xz - viewPosition.xz);
	data[index].posyroty = packHalf2x16(vec2(position.y - viewPosition.y, rot)); //maybe pos.y * terrainHeightMult for better precision

	//if (lod == 1)
	//{
	//	
	//}
	//else
	//{
	//	uint index = atomicAdd(lodCount, 1);
	//	lodData[index].posxz = packHalf2x16(position.xz - viewPosition.xz);
	//	lodData[index].normxz = packHalf2x16(norm.xz);
	//	lodData[index].posynormy = packHalf2x16(vec2(position.y - viewPosition.y, norm.y)); //maybe pos.y * terrainHeightMult for better precision
	//	lodData[index].rot = packHalf2x16(rotations);
	//}
}