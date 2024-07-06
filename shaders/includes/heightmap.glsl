#ifndef HEIGHTMAP_INCLUDED
#define HEIGHTMAP_INCLUDED

uniform float sizeMultiplier;
uniform float stepSizeMult;
uniform float heightMapHeight;

vec3 SampleNormal(vec2 uv, float power)
{
    float left = texture(heightMap, uv - vec2(sizeMultiplier, 0)).r;
    float right = texture(heightMap, uv + vec2(sizeMultiplier, 0)).r;
    float down = texture(heightMap, uv - vec2(0, sizeMultiplier)).r;
    float up = texture(heightMap, uv + vec2(0, sizeMultiplier)).r;

    vec3 normalTS = vec3((left - right) * stepSizeMult, (down - up) * stepSizeMult, 1);
    normalTS.xy *= power;

    return (normalize(normalTS).xzy);
}

vec3 SampleNormalUnNorm(vec2 uv)
{
    float left = texture(heightMap, uv - vec2(sizeMultiplier, 0)).r;
    float right = texture(heightMap, uv + vec2(sizeMultiplier, 0)).r;
    float down = texture(heightMap, uv - vec2(0, sizeMultiplier)).r;
    float up = texture(heightMap, uv + vec2(0, sizeMultiplier)).r;

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