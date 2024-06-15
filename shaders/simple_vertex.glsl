#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
//layout (location = 2) in vec3 aColor;

//uniform float upsideDown = 1;
//uniform float leftsideRight = 1;
//uniform vec2 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//out vec3 outColor;
out vec2 UV;

void main()
{
    //gl_Position = vec4(aPos.x * leftsideRight + transform.x, aPos.y * upsideDown + transform.y, aPos.z, 1.0);
    //gl_Position = transform * vec4(aPos, 1.0f);
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    //gl_Position = vec4(aPos, 1.0f);
	//outColor = aColor;
	UV = aUV;
}