#ifndef NOISE_INCLUDED
#define NOISE_INCLUDED

uniform int noiseLayers;
uniform float noiseScale;
uniform float noiseSampleDistance;
uniform float noiseSampleDistanceMult;

//float steepnessStepSize = 0.0003;

vec3 mod289(vec3 x)
{
    return x - floor(x / 289.0) * 289.0;
}

vec2 mod289(vec2 x)
{
    return x - floor(x / 289.0) * 289.0;
}

vec3 permute(vec3 x)
{
    return mod289((x * 34.0 + 1.0) * x);
}

vec3 taylorInvSqrt(vec3 r)
{
    return 1.79284291400159 - 0.85373472095314 * r;
}

float snoise(vec2 v)
{
    const vec4 C = vec4( 0.211324865405187,
                             0.366025403784439,
                            -0.577350269189626,
                             0.024390243902439);
    vec2 i  = floor(v + dot(v, C.yy));
    vec2 x0 = v -   i + dot(i, C.xx);

    vec2 i1;
    i1.x = step(x0.y, x0.x);
    i1.y = 1.0 - i1.x;

    vec2 x1 = x0 + C.xx - i1;
    vec2 x2 = x0 + C.zz;

    i = mod289(i);
    vec3 p = permute(permute(i.y + vec3(0.0, i1.y, 1.0)) + i.x + vec3(0.0, i1.x, 1.0));

    vec3 m = max(0.5 - vec3(dot(x0, x0), dot(x1, x1), dot(x2, x2)), 0.0);
    m = m * m;
    m = m * m;

    vec3 x = 2.0 * fract(p * C.www) - 1.0;
    vec3 h = abs(x) - 0.5;
    vec3 ox = floor(x + 0.5);
    vec3 a0 = x - ox;

    m *= taylorInvSqrt(a0 * a0 + h * h);

    vec3 g;
    g.x = a0.x * x0.x + h.x * x0.y;
    g.y = a0.y * x1.x + h.y * x1.y;
    g.z = a0.z * x2.x + h.z * x2.y;
    return (130.0 * dot(m, g)) * 0.5 + 0.5;
}

float InvLerp(float a, float b, float v)
{
    return (v - a) / (b - a);
}

float GenerateNoise(vec2 uv, int layers)
{
    float noise = 0;
    float maxNoise = 0;
    float weight = 1;
    float scale = 1;
	float erosion = 0;

    for (int i = 0; i < layers; i++)
    {
        float point0 = snoise(uv * noiseScale * scale) * weight;
        float point1 = snoise((uv + vec2(noiseSampleDistance, 0)) * noiseScale * scale) * weight;
        float point2 = snoise((uv + vec2(0, noiseSampleDistance)) * noiseScale * scale) * weight;
        //float left = snoise((uv + vec2(-noiseSampleDistance, 0)) * noiseScale * scale) * weight;
        //float right = snoise((uv + vec2(noiseSampleDistance, 0)) * noiseScale * scale) * weight;
        //float down = snoise((uv + vec2(0, -noiseSampleDistance)) * noiseScale * scale) * weight;
        //float up = snoise((uv + vec2(0, noiseSampleDistance)) * noiseScale * scale) * weight;

        vec2 derivative = vec2((point1 - point0), (point2 - point0));
        //vec3 derivative = vec3((left - right) / (steepnessStepSize * 2), 1.0, (down - up) / (steepnessStepSize * 2));
        float steepness = length(derivative) * (noiseSampleDistanceMult);
        
        erosion += steepness;
        float erodeSum = 1.0 / (1.0 + erosion * 0.25);
        //float point = mix(point0, (left + right + down + up + point0) * 0.2, 1.0 - pow(1.0 - steep, 8));
        //float erodeSum = 1.0 / pow(1.0 / erosion, 2);

        noise += point0 * erodeSum;
        //noise += point0;
        maxNoise += weight;

        weight *= 0.4;
        scale *= 2.5;
    }

	noise = pow(noise, 0.5);
    noise = InvLerp(0.0, maxNoise, noise);
    noise = noise * noise * noise;

    return (noise);
}

#endif