#version 460 core

#define TESS_EVALUATION_STAGE

layout (triangles, fractional_odd_spacing, ccw) in;

//in vec2 tUV[];
//in vec3 tNormal[];
//in vec3 tFragmentPosition[];
//in vec4 tColor[];

//out vec2 fUV;
//out vec3 fNormal;
out vec3 worldPosition;
//out vec4 fColor;

uniform vec4 color;

#include "variables.glsl"
#include "LOD.glsl"
#include "transformation.glsl"
#include "heightmap.glsl"

#define BARYCENTRIC_INTERPOLATE(fieldName) \
		        fieldName[0] * gl_TessCoord[0] + \
		        fieldName[1] * gl_TessCoord[1] + \
		        fieldName[2] * gl_TessCoord[2]

void main()
{
    vec4 position = gl_in[0].gl_Position * gl_TessCoord[0] + gl_in[1].gl_Position * gl_TessCoord[1] + gl_in[2].gl_Position * gl_TessCoord[2];
    //fUV = BARYCENTRIC_INTERPOLATE(tUV);

	worldPosition = position.xyz;
	worldPosition.y = SampleDynamic(worldPosition.xz) * heightMapHeight;

	gl_Position = projection * view * vec4(worldPosition, 1.0);
}