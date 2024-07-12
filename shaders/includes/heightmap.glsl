#ifndef HEIGHTMAP_INCLUDED
#define HEIGHTMAP_INCLUDED

uniform float sizeMultiplier;
uniform float stepSizeMult;
uniform float heightMapHeight;

uniform int chunksLength;
uniform float chunksLengthMult;

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

vec3 SampleNormal(vec2 uv, float power)
{
    float left = textureLod(heightMap, uv - vec2(sizeMultiplier, 0), 0).r;
    float right = textureLod(heightMap, uv + vec2(sizeMultiplier, 0), 0).r;
    float down = textureLod(heightMap, uv - vec2(0, sizeMultiplier), 0).r;
    float up = textureLod(heightMap, uv + vec2(0, sizeMultiplier), 0).r;

    vec3 normalTS = vec3((left - right) * stepSizeMult, (down - up) * stepSizeMult, 1);
    normalTS.xy *= power;

    return (normalize(normalTS).xzy);
}

vec3 SampleNormalUnNorm(vec2 uv)
{
    float left = textureLod(heightMap, uv - vec2(sizeMultiplier, 0), 0).r;
    float right = textureLod(heightMap, uv + vec2(sizeMultiplier, 0), 0).r;
    float down = textureLod(heightMap, uv - vec2(0, sizeMultiplier), 0).r;
    float up = textureLod(heightMap, uv + vec2(0, sizeMultiplier), 0).r;

    vec3 normalTS = vec3((left - right) * stepSizeMult, (down - up) * stepSizeMult, 1);
    //normalTS.xy *= power;

    return (normalTS.xzy);
}

float GetSteepness(vec3 normal)
{
    float steepness = dot(normal, vec3(0.0, 1.0, 0.0));
    //steepness = steepness * steepness;
    steepness = 1.0 - steepness;

    return steepness;
}

#endif