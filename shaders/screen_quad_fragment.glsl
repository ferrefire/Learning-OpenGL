#version 460 core

#define FRAGMENT_STAGE

in vec2 UV;

out vec4 fragmentColor;

uniform sampler2D quadTexture;

void main()
{
    fragmentColor = vec4(vec3(textureLod(quadTexture, UV, 0).r), 1.0);
    //fragmentColor = vec4(0.0, 0.0, 0.0, 1.0);
}