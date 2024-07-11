#version 460 core

#define VERTEX_STAGE

layout (location = 0) in vec3 iPosition;

out vec2 UV;
//out vec3 Normal;
//out vec3 FragmentPosition;
//out vec4 Color;

uniform mat4 model;

uniform vec4 color;

uniform sampler2D heightMap;
uniform sampler2DArray heightMapArray;

#include "variables.glsl"
#include "LOD.glsl"
#include "heightmap.glsl"

void main()
{
    UV = iPosition.xz * 0.0001 + 0.5;
    vec3 position = iPosition;
    //position.y = GenerateNoise(UV, 4) * noiseHeight;
    position.y = textureLod(heightMap, UV, 0).r * heightMapHeight;
    //position.y = 0;

    gl_Position = vec4(position, 1.0);
}