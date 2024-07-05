#version 460 core

#define TESS_CONTROL_STAGE

layout (vertices = 3) out;

in vec2 UV[];
//in vec3 Normal[];
//in vec3 FragmentPosition[];
//in vec4 Color[];

out vec2 tUV[];
//out vec3 tNormal[];
//out vec3 tFragmentPosition[];
//out vec4 tColor[];

uniform mat4 model;

uniform sampler2D heightMap;

#include "variables.glsl"
#include "culling.glsl"
#include "heightmap.glsl"

float NegativePow(float val)
{
    return (1.0 - pow(1.0 - val, 4.0));
}

float TessellationFactor (vec3 p0, vec3 p1)
{
    float edgeLength = distance(p0, p1);
    vec3 edgeCenter = (p0 + p1) * 0.5;
    float viewDistance = distance(edgeCenter, viewPosition);
    return (edgeLength * 900.0 * (1.0 / (15.0 * viewDistance)));
}

void main()
{
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    tUV[gl_InvocationID] = UV[gl_InvocationID];
    //tNormal[gl_InvocationID] = Normal[gl_InvocationID];
    //tFragmentPosition[gl_InvocationID] = FragmentPosition[gl_InvocationID];
    //tColor[gl_InvocationID] = Color[gl_InvocationID];

    if (gl_InvocationID == 0)
    {
        vec3 center = (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position).xyz * (1.0 / 3.0);
        //vec2 uv = (tUV[0] + tUV[1] + tUV[2]) * (1.0 / 3.0);
        //vec2 uv = UV[gl_InvocationID];
        float disSqr = dot(viewPosition - center, viewPosition - center);
		float tolerance = pow(1.0 - disSqr * (farMult * farMult), 2);
		//vec3 normal = SampleNormal(uv, 1);
		//float angled = 1.0 - pow(1.0 - NormalToViewDot(viewDirection, normal), 3);
		//float angled = PositionToViewDot(gl_in[0].gl_Position.xyz, gl_in[1].gl_Position.xyz, gl_in[2].gl_Position.xyz);
        if ((InView(center, tolerance) == 0 && 
            InView(gl_in[0].gl_Position.xyz, tolerance) == 0 &&
            InView(gl_in[1].gl_Position.xyz, tolerance) == 0 &&
            InView(gl_in[2].gl_Position.xyz, tolerance) == 0))
        {
            gl_TessLevelOuter[0] = 0;
            gl_TessLevelOuter[1] = 0;
            gl_TessLevelOuter[2] = 0;
            gl_TessLevelInner[0] = 0;
            return ;
        }

        float tessLevel1 = TessellationFactor(gl_in[1].gl_Position.xyz, gl_in[2].gl_Position.xyz);
        float tessLevel2 = TessellationFactor(gl_in[2].gl_Position.xyz, gl_in[0].gl_Position.xyz);
        float tessLevel3 = TessellationFactor(gl_in[0].gl_Position.xyz, gl_in[1].gl_Position.xyz);

        gl_TessLevelOuter[0] = tessLevel1;
        gl_TessLevelOuter[1] = tessLevel2;
        gl_TessLevelOuter[2] = tessLevel3;

        gl_TessLevelInner[0] = (tessLevel1 + tessLevel2 + tessLevel3) * (1.0 / 3.0);
    }
}