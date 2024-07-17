#ifndef HEIGHTMAP_INCLUDED
#define HEIGHTMAP_INCLUDED

uniform float heightMapHeight;

uniform int chunksLength;
uniform float chunksLengthMult;

uniform sampler2D heightMapLod0;
uniform sampler2D heightMapLod1;
uniform sampler2DArray heightMapArray;

uniform float worldSampleDistance;
uniform float worldSampleDistanceMult;

uniform vec2 terrainOffsetLod0;
uniform vec2 terrainOffsetLod1;

float SampleArray(vec2 uvPosition)
{
	//maybe covert to on line and operate on vector instead 
	float x = ceil(uvPosition.x * chunksLength) * chunksLengthMult - chunksLengthMult * 0.5;
	float y = ceil(uvPosition.y * chunksLength) * chunksLengthMult - chunksLengthMult * 0.5;

	float ix = floor(x * chunksLength);
	float iy = floor(y * chunksLength);

	uvPosition -= vec2(x, y);
	uvPosition = (uvPosition * chunksLength * 2.0) * 0.5 + 0.5; //+ uv offset

	return textureLod(heightMapArray, vec3(uvPosition, ix * chunksLength + iy), 0).r;
}

float Sample(vec2 uv, int lod)
{
	if (lod == 0) return textureLod(heightMapLod0, uv, 0).r;
	else if (lod == 1) return textureLod(heightMapLod1, uv, 0).r;
}

float SampleDynamic(vec2 worldPosition)
{
	vec2 worldUV = (worldPosition + terrainWorldOffset) * terrainSizeMult;
	if (abs(worldUV.x) > 0.5 || abs(worldUV.y) > 0.5) return 0;
	if (abs(worldPosition.x - terrainOffsetLod0.x) < terrainLod0Size * 0.5 && abs(worldPosition.y - terrainOffsetLod0.y) < terrainLod0Size * 0.5)
	{
		//return 0.05;
		return (Sample((worldPosition - terrainOffsetLod0) * terrainLod0SizeMult + 0.5, 0));
	}
	else if (abs(worldPosition.x - terrainOffsetLod1.x) < terrainLod1Size * 0.5 && abs(worldPosition.y - terrainOffsetLod1.y) < terrainLod1Size * 0.5)
	{
		//return 0.1;
		return (Sample((worldPosition - terrainOffsetLod1) * terrainLod1SizeMult + 0.5, 1));
	}
	else
	{
		//return 0.15;
		return (SampleArray(worldUV + 0.5));
	}
}

vec3 SampleNormalDynamic(vec2 worldPosition, float power)
{
	float left = SampleDynamic(worldPosition - vec2(worldSampleDistance, 0));
    float right = SampleDynamic(worldPosition + vec2(worldSampleDistance, 0));
    float down = SampleDynamic(worldPosition - vec2(0, worldSampleDistance));
    float up = SampleDynamic(worldPosition + vec2(0, worldSampleDistance));

    vec3 normalTS = vec3((left - right) / worldSampleDistanceMult, 1, (down - up) / worldSampleDistanceMult);
    normalTS.xz *= power;

    return (normalize(normalTS));
}

float GetSteepness(vec3 normal)
{
    float steepness = dot(normal, vec3(0.0, 1.0, 0.0));
    //steepness = steepness * steepness;
    steepness = 1.0 - steepness;

    return steepness;
}

bool IsLod(vec3 worldPosition)
{
	return (abs(worldPosition.x) > terrainChunkSize * 0.5 || abs(worldPosition.z) > terrainChunkSize * 0.5);
}

#endif