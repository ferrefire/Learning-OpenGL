#version 460 core

#define TESS_EVALUATION_STAGE

layout (triangles, fractional_odd_spacing, ccw) in;

out vec3 worldPosition;

#include "variables.glsl"
#include "transformation.glsl"
#include "heightmap.glsl"

void main()
{
    vec4 position = gl_in[0].gl_Position * gl_TessCoord[0] + gl_in[1].gl_Position * gl_TessCoord[1] + gl_in[2].gl_Position * gl_TessCoord[2];

	worldPosition = position.xyz;
	worldPosition.y = ObjectToWorld(vec3(0)).y + SampleDynamic(worldPosition.xz) * heightMapHeight;

	//terrainNormal = SampleNormalDynamic(worldPosition, 1.0);

	//shadow = RayInShadow(worldPosition);

	gl_Position = projection * view * vec4(worldPosition, 1.0);
}