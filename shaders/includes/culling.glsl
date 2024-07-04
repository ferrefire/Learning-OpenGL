#ifndef CULLING_INCLUDED
#define CULLING_INCLUDED

int InView(vec3 position, float tolerance)
{
    vec4 viewSpace = projection * view * vec4(position, 1.0);

    vec3 clipSpace = viewSpace.xyz;
    clipSpace /= -viewSpace.w;

    clipSpace.x = clipSpace.x * 0.5f + 0.5f;
    clipSpace.y = clipSpace.y * 0.5f + 0.5f;
    clipSpace.z = viewSpace.w;

    if (clipSpace.z <= 0.0 || clipSpace.z >= far) return (0);

    return (clipSpace.x < -tolerance || clipSpace.x > 1.0f + tolerance ||
        clipSpace.y < -tolerance || clipSpace.y > 1.0f + tolerance ||
        clipSpace.z <= -(tolerance * 0.5f)) ? 0 : 1;
}

#endif