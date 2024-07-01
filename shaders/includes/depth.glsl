#ifndef DEPTH_INCLUDED
#define DEPTH_INCLUDED

#ifndef TESS_CONTROL
float GetDepth()
{
    float depth = gl_FragCoord.z;

    depth = depth * 2.0 - 1.0;
    depth = (2.0 * near * far) / (far + near - depth * (far - near));
    depth = depth / far;

    return (depth);
}
#endif

float GetDepth(float z)
{
    float depth = z;

    depth = depth * 2.0 - 1.0;
    depth = (2.0 * near * far) / (far + near - depth * (far - near));
    depth = depth / far;

    return (depth);
}

#endif