#ifndef LOD_INCLUDED
#define LOD_INCLUDED

#include "transformation.glsl"

int GetLodLevel(vec3 position, float maxDistance)
{
    float z = 1.0 - WorldToClip(position).z / maxDistance;
    int lod = int(ceil(z * 10.0));

    return (lod);
}

#endif