#ifndef DEPTH_INCLUDED
#define DEPTH_INCLUDED

#include "variables.glsl"

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

#endif