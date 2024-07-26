#ifndef HEIGHTMAP_INCLUDED
#define HEIGHTMAP_INCLUDED

uniform float heightMapHeight;

uniform int chunksLength;
uniform float chunksLengthMult;

uniform sampler2D heightMapLod0;
uniform sampler2D heightMapLod0Normal;
uniform sampler2D heightMapLod1;
uniform sampler2D heightMapLod1Normal;
uniform sampler2DArray heightMapArray;
uniform sampler2DArray heightMapArrayNormal;

uniform float worldSampleDistance;
uniform float worldSampleDistanceMult;

uniform vec2 terrainOffsetLod0;
uniform vec2 terrainOffsetLod1;

uniform int lod0Ready = 1;
uniform int lod1Ready = 1;

float SampleArray(vec2 uvPosition)
{
	vec2 chunkUV = vec2(ceil(uvPosition.x * chunksLength), ceil(uvPosition.y * chunksLength));
	chunkUV = chunkUV * chunksLengthMult - chunksLengthMult * 0.5;

	vec2 indexUV = vec2(floor(chunkUV.x * chunksLength), floor(chunkUV.y * chunksLength));

	uvPosition -= chunkUV;
	uvPosition = (uvPosition * chunksLength * 2.0) * 0.5 + 0.5; //+ uv offset

	return textureLod(heightMapArray, vec3(uvPosition, indexUV.x * chunksLength + indexUV.y), 0).r;
}

//vec3 SampleArrayNormal(vec2 uvPosition)
//{
//	vec2 chunkUV = vec2(ceil(uvPosition.x * chunksLength), ceil(uvPosition.y * chunksLength));
//	chunkUV = chunkUV * chunksLengthMult - chunksLengthMult * 0.5;
//
//	vec2 indexUV = vec2(floor(chunkUV.x * chunksLength), floor(chunkUV.y * chunksLength));
//
//	uvPosition -= chunkUV;
//	uvPosition = (uvPosition * chunksLength * 2.0) * 0.5 + 0.5; //+ uv offset
//
//	return textureLod(heightMapArrayNormal, vec3(uvPosition, indexUV.x * chunksLength + indexUV.y), 0).rgb;
//}

vec3 SampleArrayNormal(vec3 worldPosition, float power)
{
	float left = SampleArray((worldPosition.xz - vec2(worldSampleDistance, 0)) * terrainSizeMult + 0.5);
    float right = SampleArray((worldPosition.xz + vec2(worldSampleDistance, 0)) * terrainSizeMult + 0.5);
    float down = SampleArray((worldPosition.xz - vec2(0, worldSampleDistance)) * terrainSizeMult + 0.5);
    float up = SampleArray((worldPosition.xz + vec2(0, worldSampleDistance)) * terrainSizeMult + 0.5);
    vec3 normalTS = vec3((left - right) / worldSampleDistanceMult, 1, (down - up) / worldSampleDistanceMult);

	if (power == 1) return (normalTS);

    normalTS.xz *= power;
    return (normalize(normalTS));
}

float Sample(vec2 uv, int lod)
{
	if (lod == 0) return textureLod(heightMapLod0, uv, 0).r;
	else if (lod == 1) return textureLod(heightMapLod1, uv, 0).r;
}

vec3 SampleNormal(vec2 uv, int lod)
{
	if (lod == 0) return textureLod(heightMapLod0Normal, uv, 0).rgb;
	else if (lod == 1) return textureLod(heightMapLod1Normal, uv, 0).rgb;
}

float SampleDynamic(vec2 worldPosition)
{
	vec2 worldUV = (worldPosition + terrainWorldOffset) * terrainSizeMult;
	if (abs(worldUV.x) > 0.5 || abs(worldUV.y) > 0.5) return 0;
	if (lod0Ready == 1 && abs(worldPosition.x - terrainOffsetLod0.x) < terrainLod0Size * 0.5 && abs(worldPosition.y - terrainOffsetLod0.y) < terrainLod0Size * 0.5)
	{
		//return 0.05;
		return (Sample((worldPosition - terrainOffsetLod0) * terrainLod0SizeMult + 0.5, 0));
	}
	else if (lod1Ready == 1 && abs(worldPosition.x - terrainOffsetLod1.x) < terrainLod1Size * 0.5 && abs(worldPosition.y - terrainOffsetLod1.y) < terrainLod1Size * 0.5)
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

vec3 SampleNormalDynamic(vec3 worldPosition, float power)
{
	//vec3 normalTS;
	//vec2 worldUV = (worldPosition + terrainWorldOffset) * terrainSizeMult;
	//if (abs(worldUV.x) > 0.5 || abs(worldUV.y) > 0.5) return vec3(0);
	//if (abs(worldPosition.x - terrainOffsetLod0.x) < terrainLod0Size * 0.5 && abs(worldPosition.y - terrainOffsetLod0.y) < terrainLod0Size * 0.5)
	//{
	//	//return 0.05;
	//	normalTS = SampleNormal((worldPosition - terrainOffsetLod0) * terrainLod0SizeMult + 0.5, 0);
	//}
	//else if (abs(worldPosition.x - terrainOffsetLod1.x) < terrainLod1Size * 0.5 && abs(worldPosition.y - terrainOffsetLod1.y) < terrainLod1Size * 0.5)
	//{
	//	//return 0.1;
	//	normalTS = SampleNormal((worldPosition - terrainOffsetLod1) * terrainLod1SizeMult + 0.5, 1);
	//}
	//else
	//{
	//	//return 0.15;
	//	normalTS = SampleArrayNormal(worldUV + 0.5);
	//}
	//normalTS.xz *= power;
	//return (normalize(normalTS));

	//float minDis = terrainLod1Size * 0.5;
	//if (lod1Ready == 0) minDis = terrainLod0Size * 0.5;
	//if (lod0Ready == 0) minDis = 0;

	//bool useArray = abs(worldPosition.x - terrainOffsetLod1.x) >= terrainLod1Size * 0.5 ||
	//	abs(worldPosition.y - terrainOffsetLod1.y) >= terrainLod1Size * 0.5;
	//if (!useArray && lod1Ready == 0) useArray = abs(worldPosition.x - terrainOffsetLod0.x) >= terrainLod0Size * 0.5 ||
	//	abs(worldPosition.y - terrainOffsetLod0.y) >= terrainLod0Size * 0.5;
	//if (!useArray && lod0Ready == 0) useArray = true;	
	//if (useArray)
	//{
	//	vec2 worldUV = (worldPosition + terrainWorldOffset) * terrainSizeMult + 0.5;
	//	vec3 normal_TS = SampleArrayNormal(worldUV);
	//	normal_TS.xz *= power;
	//	return (normalize(normal_TS));
	//}

	float left = SampleDynamic(worldPosition.xz - vec2(worldSampleDistance, 0));
    float right = SampleDynamic(worldPosition.xz + vec2(worldSampleDistance, 0));
    float down = SampleDynamic(worldPosition.xz - vec2(0, worldSampleDistance));
    float up = SampleDynamic(worldPosition.xz + vec2(0, worldSampleDistance));
    vec3 normalTS = vec3((left - right) / worldSampleDistanceMult, 1, (down - up) / worldSampleDistanceMult);

	//float center = worldPosition.y * terrainHeightMult;
    //float right = SampleDynamic(worldPosition.xz + vec2(worldSampleDistance, 0));
    //float up = SampleDynamic(worldPosition.xz + vec2(0, worldSampleDistance));
    //vec3 normalTS = vec3((right - center) / (worldSampleDistanceMult * 0.5), 1.0, (up - center) / (worldSampleDistanceMult * 0.5));

	if (power == 1) return (normalTS);

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