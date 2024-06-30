#version 460 core

layout (triangles, fractional_odd_spacing, ccw) in;

in vec2 tUV[];
in vec3 tNormal[];
in vec3 tFragmentPosition[];
in vec4 tColor[];

out vec2 UV;
out vec3 Normal;
out vec3 FragmentPosition;
out vec4 Color;

#define BARYCENTRIC_INTERPOLATE(fieldName) \
		        fieldName[0] * gl_TessCoord.x + \
		        fieldName[1] * gl_TessCoord.y + \
		        fieldName[2] * gl_TessCoord.z

void main()
{
    UV
}