#ifndef HEIGHTMAP_INCLUDED
#define HEIGHTMAP_INCLUDED

uniform float sizeMultiplier;
uniform float stepSizeMult;
uniform float heightMapHeight;

uniform int chunksLength;
uniform float chunksLengthMult;

uniform sampler2D heightMap;
uniform sampler2DArray heightMapArray;

uniform float sampleStepSize;
uniform float sampleStepSizeMult;
uniform float sampleArrayStepSize;
uniform float sampleArrayStepSizeMult;
uniform float worldSampleSize;
uniform float worldSampleSizeMult;

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

vec3 SampleArrayNormal(vec2 uvPosition, float power)
{
	float left = SampleArray(uvPosition - vec2(sampleArrayStepSize, 0));
    float right = SampleArray(uvPosition + vec2(sampleArrayStepSize, 0));
    float down = SampleArray(uvPosition - vec2(0, sampleArrayStepSize));
    float up = SampleArray(uvPosition + vec2(0, sampleArrayStepSize));

    vec3 normalTS = vec3((left - right) * sampleArrayStepSizeMult, (down - up) * sampleArrayStepSizeMult, 1);
    normalTS.xy *= power;

    return (normalize(normalTS).xzy);
}

vec3 SampleArrayNormalUnNorm(vec2 uvPosition)
{
	float left = SampleArray(uvPosition - vec2(sampleArrayStepSize, 0));
    float right = SampleArray(uvPosition + vec2(sampleArrayStepSize, 0));
    float down = SampleArray(uvPosition - vec2(0, sampleArrayStepSize));
    float up = SampleArray(uvPosition + vec2(0, sampleArrayStepSize));

    vec3 normalTS = vec3((left - right) * sampleArrayStepSizeMult, (down - up) * sampleArrayStepSizeMult, 1);
    //normalTS.xy *= power;

    return (normalTS.xzy);
}

float Sample(vec2 uv)
{
	return textureLod(heightMap, uv, 0).r;
}

vec3 SampleNormal(vec2 uv, float power)
{
    float left = Sample(uv - vec2(sampleStepSize, 0));
    float right = Sample(uv + vec2(sampleStepSize, 0));
    float down = Sample(uv - vec2(0, sampleStepSize));
    float up = Sample(uv + vec2(0, sampleStepSize));

    vec3 normalTS = vec3((left - right) * sampleStepSizeMult, (down - up) * sampleStepSizeMult, 1);
    normalTS.xy *= power;

    return (normalize(normalTS).xzy);
}

vec3 SampleNormalUnNorm(vec2 uv)
{
    float left = Sample(uv - vec2(sampleStepSize, 0));
    float right = Sample(uv + vec2(sampleStepSize, 0));
    float down = Sample(uv - vec2(0, sampleStepSize));
    float up = Sample(uv + vec2(0, sampleStepSize));

    vec3 normalTS = vec3((left - right) * sampleStepSizeMult, (down - up) * sampleStepSizeMult, 1);
    //normalTS.xy *= power;

    return (normalTS.xzy);
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

vec3 SampleNormalDynamic(vec3 worldPosition, float power)
{
	if (abs(worldPosition.x) <= terrainChunkSize * 0.5 && abs(worldPosition.z) <= terrainChunkSize * 0.5)
	{
		return (SampleNormal(worldPosition.xz * terrainChunkSizeMult + 0.5, power));
	}
	else
	{
		return (SampleArrayNormal(worldPosition.xz * terrainSizeMult + 0.5, power));
	}
}

vec3 SampleNormalUnNormDynamic(vec3 worldPosition)
{
	if (abs(worldPosition.x) <= terrainChunkSize * 0.5 && abs(worldPosition.z) <= terrainChunkSize * 0.5)
	{
		return (SampleNormalUnNorm(worldPosition.xz * terrainChunkSizeMult + 0.5));
	}
	else
	{
		return (SampleArrayNormalUnNorm(worldPosition.xz * terrainSizeMult + 0.5));
	}
}

vec3 SampleNormalV2(vec2 worldPosition, float power)
{
	float left = SampleDynamic(worldPosition - vec2(worldSampleSize, 0));
    float right = SampleDynamic(worldPosition + vec2(worldSampleSize, 0));
    float down = SampleDynamic(worldPosition - vec2(0, worldSampleSize));
    float up = SampleDynamic(worldPosition + vec2(0, worldSampleSize));

    vec3 normalTS = vec3((left - right) / worldSampleSizeMult, 1, (down - up) / worldSampleSizeMult);
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

float GetSteepnessDynamic(vec3 worldPosition, vec3 normal)
{
	//if (abs(worldPosition.x) > terrainChunkSize * 0.5 || abs(worldPosition.z) > terrainChunkSize * 0.5)
	//{
	//	normal.xz *= 0.25;
	//	normal = normalize(normal);
	//}

    return GetSteepness(normal);
}

bool IsLod(vec3 worldPosition)
{
	return (abs(worldPosition.x) > terrainChunkSize * 0.5 || abs(worldPosition.z) > terrainChunkSize * 0.5);
}

//float GetArraySteepness(vec3 normal)
//{
//    float steepness = dot(normal, vec3(0.0, 1.0, 0.0));
//    //steepness = steepness * steepness;
//    steepness = 1.0 - steepness;
//
//    return steepness;
//}

#endif