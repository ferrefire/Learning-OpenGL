#ifndef TRANSFORMATION_INCLUDED
#define TRANSFORMATION_INCLUDED

vec3 WorldToClip(vec3 position)
{
    vec4 viewSpace = view * vec4(position, 1.0);

    vec3 clipSpace = viewSpace.xyz;
    clipSpace /= -viewSpace.w;

    clipSpace.x = clipSpace.x * 0.5f + 0.5f;
    clipSpace.y = clipSpace.y * 0.5f + 0.5f;
    clipSpace.z = viewSpace.w;

    return (clipSpace);
}

#endif