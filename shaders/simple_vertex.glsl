#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 aNorm;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform int countRoot;
uniform int countRootRoot;
uniform vec3 viewPos;

out vec2 UV;
out vec3 Norm;
out vec3 FragPos;
out vec3 ranCol;

int gap = 10;

float random (vec2 st)
{
    return fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43758.5453123);
}

void main()
{
    int x = gl_InstanceID / countRoot;
    int y = gl_InstanceID / countRootRoot - (x * countRootRoot);
    int z = gl_InstanceID % countRootRoot;

    //if (dot(viewPos - vec3(x, y, z) * gap, viewPos - vec3(x, y, z) * gap) > 1000000) return ;

    float ran1 = random(vec2(float(gl_InstanceID), float(x + y + z)));
	float ran2 = random(vec2(float(gl_InstanceID), ran1));
	float ran3 = random(vec2(ran2, ran1));
	ranCol = vec3(ran1, ran2, ran3);
    //int z = gl_InstanceID - (x * 100);
    gl_Position = projection * view * model * vec4(aPos + vec3(x, y, z) * gap + ranCol * gap, 1.0);
    FragPos = (model * vec4(aPos + vec3(x, y, z) * gap + ranCol * gap, 1.0)).xyz;
	UV = aUV;
	Norm =  (model * vec4(aNorm, 0.0)).xyz;
	//Norm =  aNorm;
}