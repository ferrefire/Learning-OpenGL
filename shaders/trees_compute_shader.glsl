#version 460 core

#define COMPUTE_STAGE

layout (local_size_x = 4, local_size_y = 4) in;

struct datastruct
{
	uint posxz;
	uint posyroty;
};

layout(std430, binding = 7) buffer iLod0Data
{
    datastruct lod0Data[];
};

layout(std430, binding = 8) buffer oLod0Count
{
    uint lod0Count;
};

layout(std430, binding = 9) buffer iLod1Data
{
    datastruct lod1Data[];
};

layout(std430, binding = 10) buffer oLod1Count
{
    uint lod1Count;
};

layout(std430, binding = 11) buffer iLod2Data
{
    datastruct lod2Data[];
};

layout(std430, binding = 12) buffer oLod2Count
{
    uint lod2Count;
};

uniform int instanceCount;
uniform float instanceMult;
uniform int instanceCountSqrt;
uniform float instanceCountSqrtMult;
uniform float lod0Range = 256.0;
uniform float lod1Range = 256.0;
uniform int renderLod0;
uniform int renderLod1;
uniform int renderLod2;

float spacing = 100;
float spacingMult = 0.01;

#include "variables.glsl"
#include "culling.glsl"
#include "heightmap.glsl"
#include "functions.glsl"
#include "depth.glsl"

float random(vec2 st)
{
    return fract(sin(dot(st.xy * 0.001, vec2(12.9898,78.233))) * 43758.5453123);
}

int Inside(vec2 pos)
{
	float dis = SquaredDistanceVec2(pos, vec2(0));
	if (dis > pow(instanceCountSqrt * 0.5, 2)) return (-1);
	else if (dis > pow(lod1Range * 0.5, 2)) return (2);
	else if (dis > pow(lod0Range * 0.5, 2)) return (1);
	else return (0);
}

void main()
{
	if (gl_GlobalInvocationID.x >= instanceCountSqrt || gl_GlobalInvocationID.y >= instanceCountSqrt) return ;
	if (gl_GlobalInvocationID.x == 0 && gl_GlobalInvocationID.y == 0)
	{
		atomicExchange(lod0Count, 0);
		atomicExchange(lod1Count, 0);
		atomicExchange(lod2Count, 0);
	}

	/*if (gl_GlobalInvocationID.x == 0 && gl_GlobalInvocationID.y == 0)
	{
		float rotation = 0;
		vec3 position = vec3(0, 2000, 0);

		uint index = atomicAdd(count, 1);
		data[index].posxz = packHalf2x16(position.xz - viewPosition.xz);
		data[index].posyroty = packHalf2x16(vec2(position.y - viewPosition.y, rotation));
		return ;
	}*/

	float x = float(gl_GlobalInvocationID.x) - instanceCountSqrt * 0.5;
    float z = float(gl_GlobalInvocationID.y) - instanceCountSqrt * 0.5;

	int lod = Inside(vec2(x, z));

	if (lod == -1 || (lod == 2 && renderLod2 == 0) || (lod == 1 && renderLod1 == 0) || (lod == 0 && renderLod0 == 0)) return ;

	//if (SquaredDistanceVec2(vec2(x, z), vec2(0)) > pow(instanceCountSqrt * 0.5, 2)) return ;

	//int lod = Inside(vec2(x, z));

	vec2 flooredViewPosition = vec2(round(viewPosition.x * spacingMult) * spacing, 
		round(viewPosition.z * spacingMult) * spacing);
    x = x * spacing + flooredViewPosition.x;
    z = z * spacing + flooredViewPosition.y;

    //float y = SampleDynamic(vec2(x, z)) * heightMapHeight;
    float y = SampleArray((vec2(x, z) + terrainWorldOffset) * terrainSizeMult + 0.5) * heightMapHeight;

	//vec3 clipSpacePosition = WorldToClip(vec3(x, y, z));
	//vec2 depthUV = clipSpacePosition.xy;
	//float depthVal = GetDepth(clipSpacePosition.z, near, far);

	//if (MapOccluded(depthUV, (1.0 - depthVal)) > 0.5) return ;
    
	//float squaredDistance = SquaredDistanceToViewPosition(vec3(x, y, z));

	//float maxDistance = pow(instanceCountSqrt * spacing, 2);
	//float maxDistanceMult = pow(instanceCountSqrtMult * spacingMult, 2);

	//float falloff = clamp(clamp(squaredDistance, 0.0, maxDistance) * maxDistanceMult, 0.0, 1.0);
	//falloff = 1.0 - pow(1.0 - falloff, 16);

    float ranMult = 1.0 - (abs(x) + abs(z)) * terrainSizeMult * 0.5;
    float ran = random(vec2(x, z) * ranMult);

    //if (falloff > ran) return ;
	//if (RayOccluded(vec3(x, y, z)) == 1) return ;

    vec3 position = vec3(x, y, z);
    ran = random(vec2(ran * 100, ran * 200));
	position.x += (ran - 0.5) * spacing;
    ran = random(vec2(ran * 200, ran * 100));
	position.z += (ran - 0.5) * spacing;
	//position.y = SampleDynamic(position.xz) * heightMapHeight;
	position.y = SampleArray((position.xz + terrainWorldOffset) * terrainSizeMult + 0.5) * heightMapHeight;

    //float viewTolerance = 1.0 - clamp(squaredDistance, 0.0, 1000000.0) * 0.000001;
	//viewTolerance = pow(viewTolerance, 8);
	//if (squaredDistance > 10000 && InView(position + vec3(0, 25, 0), vec3(viewTolerance * 0.1, viewTolerance, 0)) == 0) return ;
	//if (InView(position, 0) == 0) return ;
	

	if (lod == 2)
	{
		if (MapOccluded(position + vec3(0, 75, 0)) <= 0) return ;
		if (InView(position + vec3(0, 50, 0), vec3(0)) == 0) return ;
	}
	else if (lod == 1)
	{
		//if (AreaOccluded(position + vec3(0, 75, 0), vec2(25, 75)) == 1) return ;
		if (MapOccluded(position + vec3(0, 150, 0)) <= 0) return ;
		if (AreaInView(position + vec3(0, 75, 0), vec2(25, 75)) == 0) return ;
	}
	else if (lod == 0)
	{
		if (MapOccluded(position + vec3(0, 150, 0)) <= 0) return ;
		if (AreaInView(position + vec3(0, 75, 0), vec2(25, 75)) == 0) return ;
	}
	

    //vec3 norm = SampleNormalDynamic(position, 0.5);
    vec3 norm = SampleArrayNormal(position + vec3(terrainWorldOffset.x, 0, terrainWorldOffset.y), 0.5);
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

    //uint index = atomicAdd(count, 1);
	//data[index].posxz = packHalf2x16(position.xz - viewPosition.xz);
	//data[index].posyroty = packHalf2x16(vec2(position.y - viewPosition.y, rotation)); //maybe pos.y * terrainHeightMult for better precision

	if (lod == 2)
	{
		uint index = atomicAdd(lod2Count, 1);
		lod2Data[index].posxz = packHalf2x16(position.xz - viewPosition.xz);
		lod2Data[index].posyroty = packHalf2x16(vec2(position.y - viewPosition.y, rotation));
	}
	else if (lod == 1)
	{
		uint index = atomicAdd(lod1Count, 1);
		lod1Data[index].posxz = packHalf2x16(position.xz - viewPosition.xz);
		lod1Data[index].posyroty = packHalf2x16(vec2(position.y - viewPosition.y, rotation));
	}
	else if (lod == 0)
	{
		uint index = atomicAdd(lod0Count, 1);
		lod0Data[index].posxz = packHalf2x16(position.xz - viewPosition.xz);
		lod0Data[index].posyroty = packHalf2x16(vec2(position.y - viewPosition.y, rotation));
	}
}