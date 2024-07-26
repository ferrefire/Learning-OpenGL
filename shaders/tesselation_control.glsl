#version 460 core

#define TESS_CONTROL_STAGE

layout (vertices = 3) out;

//in vec2 UV[];
//in vec3 Normal[];
//in vec3 FragmentPosition[];
//in vec4 Color[];
//in vec3 worldPosition[];

//out vec2 tUV[];
//out vec3 tNormal[];
//out vec3 tFragmentPosition[];
//out vec4 tColor[];

#include "variables.glsl"
#include "culling.glsl"
#include "heightmap.glsl"

uniform float tesselationFactor = 10;

float NegativePow(float val)
{
    return (1.0 - pow(1.0 - val, 4.0));
}

float TessellationFactor (vec3 p0, vec3 p1)
{
    float edgeLength = distance(p0, p1);
    vec3 edgeCenter = (p0 + p1) * 0.5;
    float viewDistance = distance(edgeCenter, viewPosition);
    return (edgeLength * height * (1.0 / (tesselationFactor * viewDistance)));
}

void main()
{
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    //tUV[gl_InvocationID] = UV[gl_InvocationID];
    //tNormal[gl_InvocationID] = Normal[gl_InvocationID];
    //tFragmentPosition[gl_InvocationID] = FragmentPosition[gl_InvocationID];
    //tColor[gl_InvocationID] = Color[gl_InvocationID];

    if (gl_InvocationID == 0)
    {
        vec3 p0 = (gl_in[0].gl_Position).xyz;
        vec3 p1 = (gl_in[1].gl_Position).xyz;
        vec3 p2 = (gl_in[2].gl_Position).xyz;
		//p0.y = SampleDynamic(p0.xz);
		//p1.y = SampleDynamic(p1.xz);
		//p2.y = SampleDynamic(p2.xz);
        vec3 center = (p0 + p1 + p2) * (1.0 / 3.0);
        //vec2 uv = (tUV[0] + tUV[1] + tUV[2]) * (1.0 / 3.0);
        //vec2 uv = UV[gl_InvocationID];
        float disSqr = dot(viewPosition - center, viewPosition - center);
		float tolerance = 1.0 - disSqr * (farMult * farMult);
		if (disSqr > pow(terrainChunkSize * 0.5, 2)) tolerance = pow(tolerance, 16);
		else tolerance = pow(tolerance, 128);

        if (disSqr > 10000 && (
			(InView(center, tolerance) == 0 && 
            InView(p0, tolerance) == 0 &&
            InView(p1, tolerance) == 0 &&
            InView(p2, tolerance) == 0)))
        {
            gl_TessLevelOuter[0] = 0;
            gl_TessLevelOuter[1] = 0;
            gl_TessLevelOuter[2] = 0;
            gl_TessLevelInner[0] = 0;
        }
		else
		{
			float tessLevel1 = TessellationFactor(p1, p2);
        	float tessLevel2 = TessellationFactor(p2, p0);
        	float tessLevel3 = TessellationFactor(p0, p1);

        	gl_TessLevelOuter[0] = tessLevel1;
        	gl_TessLevelOuter[1] = tessLevel2;
        	gl_TessLevelOuter[2] = tessLevel3;

        	gl_TessLevelInner[0] = (tessLevel1 + tessLevel2 + tessLevel3) * (1.0 / 3.0);
		}
    }
}