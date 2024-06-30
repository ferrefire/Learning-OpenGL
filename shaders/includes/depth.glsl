#ifndef DEPTH_INCLUDED
#define DEPTH_INCLUDED

float GetDepth()
{
    float depth = gl_FragCoord.z;

    depth = depth * 2.0 - 1.0;
    depth = (2.0 * near * far) / (far + near - depth * (far - near));
    depth = depth / far;

    return (depth);
}

#endif