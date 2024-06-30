#version 460 core
layout (vertices=3) out

in vec2 UV[];
in vec3 Normal[];
in vec3 FragmentPosition[];
in vec4 Color[];

out vec2 tUV[];
out vec3 tNormal[];
out vec3 tFragmentPosition[];
out vec4 tColor[];

//in gl_PerVertex
//{
//    vec4 gl_Position;
//    float gl_PointSize;
//    float gl_ClipDistance[]'
//} gl_in[gl_MaxPatchVertices];

void main()
{
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    tUV[gl_InvocationID] = UV[gl_InvocationID];
    tNormal[gl_InvocationID] = Normal[gl_InvocationID];
    tFragmentPosition[gl_InvocationID] = FragmentPosition[gl_InvocationID];
    tColor[gl_InvocationID] = Color[gl_InvocationID];

    if (gl_InvocationID == 0)
    {
        gl_TessLevelOuter[0] = 16;
        gl_TessLevelOuter[1] = 16;
        gl_TessLevelOuter[2] = 16;
        //gl_TessLevelOuter[3] = 16;

        gl_TessLevelInner[0] = 16;
        //gl_TessLevelInner[1] = 16;
    }
}