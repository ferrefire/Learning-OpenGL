#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 aNorm;
//layout (location = 2) in vec3 aColor;

//uniform float upsideDown = 1;
//uniform float leftsideRight = 1;
//uniform vec2 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform int id;

//out vec3 outColor;
out vec2 UV;
out vec3 Norm;
out vec3 ranCol;

float random (vec2 st)
{
    return fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43758.5453123);
}

void main()
{
    //gl_Position = vec4(aPos.x * leftsideRight + transform.x, aPos.y * upsideDown + transform.y, aPos.z, 1.0);
    //gl_Position = transform * vec4(aPos, 1.0f);
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    //gl_Position = vec4(aPos, 1.0f);
	//outColor = aColor;
	UV = aUV;
	Norm =  (model * vec4(aNorm, 0.0)).xyz;
	float ran1 = random(vec2(id, 1));
	float ran2 = random(vec2(1.0f, ran1));
	float ran3 = random(vec2(ran2, ran1));
	ranCol = vec3(ran1, ran2, ran3);
}