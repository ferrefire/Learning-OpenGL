#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 aNorm;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform int id;

out vec2 UV;
out vec3 Norm;
out vec3 FragPos;
out vec3 ranCol;

float random (vec2 st)
{
    return fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43758.5453123);
}

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = (model * vec4(aPos, 1.0)).xyz;
	UV = aUV;
	Norm =  (model * vec4(aNorm, 0.0)).xyz;
	//Norm =  aNorm;
	float ran1 = random(vec2(id, 1));
	float ran2 = random(vec2(1.0f, ran1));
	float ran3 = random(vec2(ran2, ran1));
	ranCol = vec3(ran1, ran2, ran3);
}