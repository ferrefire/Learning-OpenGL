#version 460 core

#define TESS_EVALUATION_STAGE

layout (triangles, fractional_odd_spacing, ccw) in;

in vec2 tUV[];
//in vec3 tNormal[];
//in vec3 tFragmentPosition[];
//in vec4 tColor[];

out vec2 fUV;
//out vec3 fNormal;
out vec3 fFragmentPosition;
//out vec4 fColor;

uniform mat4 model;

uniform vec4 color;

uniform sampler2D heightMap;

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
    vec4 position = gl_in[0].gl_Position * gl_TessCoord[0] + gl_in[1].gl_Position * 
		gl_TessCoord[1] + gl_in[2].gl_Position * gl_TessCoord[2];
    fUV = BARYCENTRIC_INTERPOLATE(tUV);
    int lod = GetLodLevel(position.xyz, far);

    //position.y = GenerateNoise(fUV, lod) * noiseHeight;
    position.y = texture(heightMap, fUV).r * heightMapHeight;
    //position.y = 0;

    fFragmentPosition = (model * position).xyz;

    //fNormal = GenerateNoiseNormal(fUV, lod, 0.0025);
	gl_Position = projection * view * model * position;
	//float dis = gl_Position.z * farMult;
	//if (false && dis < 0.1)
	//{
	//	fNormal = SampleNormal(fUV, 1);
    //	float steepness = GetSteepness(fNormal);
    //	steepness = pow(steepness, 2);
    //	fColor = mix(color, vec4(0.25, 0.25, 0.25, 1), steepness);
	//}
	//else
	//{
	//	fNormal = vec3(0);
    //	float steepness = 0;
    //	//steepness = pow(steepness, 2);
    //	fColor = vec4(0);
	//}
    
    //fColor = vec4(vec3(gl_Position.z / far), 1);

    //fNormal = vec3(0);
    //fColor = vec4(0);
    
    
}