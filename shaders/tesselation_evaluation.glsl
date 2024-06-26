#version 460 core

layout (triangles, fractional_odd_spacing, ccw) in;

in vec2 tUV[];
//in vec3 tNormal[];
//in vec3 tFragmentPosition[];
//in vec4 tColor[];

out vec2 fUV;
out vec3 fNormal;
out vec3 fFragmentPosition;
out vec4 fColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec4 color;

#include "noise.glsl"

#define BARYCENTRIC_INTERPOLATE(fieldName) \
		        fieldName[0] * gl_TessCoord[0] + \
		        fieldName[1] * gl_TessCoord[1] + \
		        fieldName[2] * gl_TessCoord[2]

void main()
{
    vec4 position = gl_in[0].gl_Position * gl_TessCoord[0] + gl_in[1].gl_Position * gl_TessCoord[1] + gl_in[2].gl_Position * gl_TessCoord[2];
    fUV = BARYCENTRIC_INTERPOLATE(tUV);
    position.y = GenerateNoise(fUV, noiseLayers) * noiseHeight;
    fNormal = GenerateNoiseNormal(fUV, noiseLayers, 0.001);
    fFragmentPosition = (model * position).xyz;;
    float steepness = GetSteepness(fNormal);
    steepness = pow(steepness, 2);
    fColor = mix(color, vec4(0.25, 0.25, 0.25, 1), steepness);
    
    gl_Position = projection * view * model * position;
}