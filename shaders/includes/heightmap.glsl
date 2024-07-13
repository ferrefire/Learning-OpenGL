#ifndef HEIGHTMAP_INCLUDED
#define HEIGHTMAP_INCLUDED

uniform float heightMapHeight;

uniform int chunksLength;
uniform float chunksLengthMult;

uniform sampler2D heightMap;
uniform sampler2DArray heightMapArray;

uniform float worldSampleDistance;
uniform float worldSampleDistanceMult;

uniform vec2 terrainOffset;

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

float Sample(vec2 uv)
{
	return textureLod(heightMap, uv, 0).r;
}

float SampleDynamic(vec2 worldPosition)
{
	if (abs(worldPosition.x - terrainOffset.x) <= terrainChunkSize * 0.5 && abs(worldPosition.y - terrainOffset.y) <= terrainChunkSize * 0.5)
	{
		return (Sample((worldPosition - terrainOffset) * terrainChunkSizeMult + 0.5));
	}
	else
	{
		return (SampleArray(worldPosition * terrainSizeMult + 0.5));
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