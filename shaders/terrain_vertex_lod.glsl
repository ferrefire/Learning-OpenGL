#version 460 core

#define VERTEX_STAGE

layout (location = 0) in vec3 iPosition;

//out vec2 UV;
//out vec3 Normal;
//out vec3 FragmentPosition;
//out vec4 Color;
out vec3 worldPosition;

#include "variables.glsl"
#include "transformation.glsl"
#include "heightmap.glsl"

void main()
{
    //UV = vec2(ObjectToUV(iPosition));

	worldPosition = ObjectToWorld(iPosition);
	worldPosition.y += SampleDynamic(worldPosition.xz) * heightMapHeight;

    gl_Position = WorldToView(worldPosition);
}