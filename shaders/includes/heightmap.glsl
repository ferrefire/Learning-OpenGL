#ifndef HEIGHTMAP_INCLUDED
#define HEIGHTMAP_INCLUDED

uniform float sizeMultiplier;

vec3 SampleNormal(vec2 uv, float stepSize)
{
    float left = texture(heightMap, uv - vec2(stepSize * sizeMultiplier, 0)).r;
    float right = texture(heightMap, uv + vec2(stepSize * sizeMultiplier, 0)).r;
    float down = texture(heightMap, uv - vec2(0, stepSize * sizeMultiplier)).r;
    float up = texture(heightMap, uv + vec2(0, stepSize * sizeMultiplier)).r;

    vec3 normalTS = vec3((left - right) / (stepSize * sizeMultiplier * 2), (down - up) / (stepSize * sizeMultiplier * 2), 1);
    normalTS.xy *= 0.5;

    return (normalize(normalTS).xzy);
}

#endif