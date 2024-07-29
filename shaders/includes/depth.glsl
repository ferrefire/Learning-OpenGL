#ifndef DEPTH_INCLUDED
#define DEPTH_INCLUDED

#include "variables.glsl"
#include "transformation.glsl"

layout(r32f, binding = 9) uniform image2D depthMapImage;

uniform int newFrame = 0;

float GetDepth(float z, float near, float far)
{
    float depth = z;

    depth = depth * 2.0 - 1.0;
    depth = (2.0 * near * far) / (far + near - depth * (far - near));
    depth = depth / far;
	depth = clamp(depth, 0.0, 1.0);

    return (depth);
}

float GetDepth(float z)
{
    return (GetDepth(z, near, far));
}

float GetWorldDepth(vec3 position)
{
	return (WorldToClip(position).z * farMult);
}

void SetDepth(ivec2 coordinates, float value)
{
	if (imageLoad(depthMapImage, coordinates).r < value) imageStore(depthMapImage, coordinates, vec4(value));
}

float GetDepthMap(ivec2 coordinates)
{
	return imageLoad(depthMapImage, coordinates).r;
}

#endif