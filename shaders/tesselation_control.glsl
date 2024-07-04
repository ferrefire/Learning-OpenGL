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
uniform mat4 view;
uniform mat4 projection;

uniform vec3 viewPosition;

uniform float far;
uniform float near;

#include "culling.glsl"

float NegativePow(float val)
{
    return (1.0 - pow(1.0 - val, 4.0));
}

float TessellationFactor (vec3 p0, vec3 p1)
{
    float edgeLength = distance(p0, p1);
    vec3 edgeCenter = (p0 + p1) * 0.5;
    float viewDistance = distance(edgeCenter, viewPosition);
    return edgeLength * 900.0 / (5.0 * viewDistance);
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
        float disSqr = dot(viewPosition - center, viewPosition - center);
        if (disSqr > 10000 &&
            InView(center, 0.1) == 0 && 
            InView(gl_in[0].gl_Position.xyz, 0.1) == 0 &&
            InView(gl_in[1].gl_Position.xyz, 0.1) == 0 &&
            InView(gl_in[2].gl_Position.xyz, 0.1) == 0)
        {
            gl_TessLevelOuter[0] = 0;
            gl_TessLevelOuter[1] = 0;
            gl_TessLevelOuter[2] = 0;
            gl_TessLevelInner[0] = 0;
            return ;
        }

        //const int MIN_TESS_LEVEL = 4;
        //const int MAX_TESS_LEVEL = 64;
        //const float MIN_DISTANCE = 10;
        //const float MAX_DISTANCE = 5000;
//
        //vec4 eyeSpacePos1 = projection * view * model * gl_in[0].gl_Position;
        //vec4 eyeSpacePos2 = projection * view * model * gl_in[1].gl_Position;
        //vec4 eyeSpacePos3 = projection * view * model * gl_in[2].gl_Position;
//
        //float distance1 = clamp((abs(eyeSpacePos1.z)-MIN_DISTANCE) / (MAX_DISTANCE-MIN_DISTANCE), 0.0, 1.0);
        //float distance2 = clamp((abs(eyeSpacePos2.z)-MIN_DISTANCE) / (MAX_DISTANCE-MIN_DISTANCE), 0.0, 1.0);
        //float distance3 = clamp((abs(eyeSpacePos3.z)-MIN_DISTANCE) / (MAX_DISTANCE-MIN_DISTANCE), 0.0, 1.0);
//
        //float tessLevel1 = mix(MAX_TESS_LEVEL, MIN_TESS_LEVEL, NegativePow(min(distance2, distance3)));
        //float tessLevel2 = mix(MAX_TESS_LEVEL, MIN_TESS_LEVEL, NegativePow(min(distance3, distance1)));
        //float tessLevel3 = mix(MAX_TESS_LEVEL, MIN_TESS_LEVEL, NegativePow(min(distance1, distance2)));

        float tessLevel1 = TessellationFactor(gl_in[1].gl_Position.xyz, gl_in[2].gl_Position.xyz);
        float tessLevel2 = TessellationFactor(gl_in[2].gl_Position.xyz, gl_in[0].gl_Position.xyz);
        float tessLevel3 = TessellationFactor(gl_in[0].gl_Position.xyz, gl_in[1].gl_Position.xyz);

        gl_TessLevelOuter[0] = tessLevel1;
        gl_TessLevelOuter[1] = tessLevel2;
        gl_TessLevelOuter[2] = tessLevel3;

        gl_TessLevelInner[0] = (tessLevel1 + tessLevel2 + tessLevel3) * (1.0 / 3.0);
    }
}