#ifndef HEIGHTMAP_INCLUDED
#define HEIGHTMAP_INCLUDED

vec3 SampleNormal(vec2 uv, float stepSize)
{
    float left = texture(heightMap, uv - vec2(stepSize, 0)).r;
    float right = texture(heightMap, uv + vec2(stepSize, 0)).r;
    float down = texture(heightMap, uv - vec2(0, stepSize)).r;
    float up = texture(heightMap, uv + vec2(0, stepSize)).r;

    vec3 normalTS = vec3((left - right) / (stepSize * 2), (down - up) / (stepSize * 2), 1);
    normalTS.xy *= 0.25;

    return (normalize(normalTS).xzy);
}

#endif