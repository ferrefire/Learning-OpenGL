#version 460 core

#define VERTEX_STAGE

layout (location = 0) in vec3 iPosition;

out vec2 UV;
//out vec3 Normal;
//out vec3 FragmentPosition;
//out vec4 Color;

uniform vec4 color;

#include "variables.glsl"
#include "transformation.glsl"
#include "LOD.glsl"
#include "heightmap.glsl"

void main()
{
    UV = ObjectToUV(iPosition);
    vec3 position = iPosition;

    //position.y = textureLod(heightMap, UV, 0).r * heightMapHeight;
	//position.y = SampleArray(UV) * heightMapHeight;
	position.y = SampleDynamic(ObjectToWorld(position)) * heightMapHeight;

    gl_Position = vec4(position, 1.0);
}