#version 460 core

#define FRAGMENT_STAGE

in vec2 UV;
in vec3 normal;
in vec3 worldPosition;

out vec4 fragmentColor;

#include "variables.glsl"
#include "depth.glsl"
#include "lighting.glsl"

void main()
{
    vec3 normal = normalize(normal);

    vec3 viewDirection = normalize(viewPosition - worldPosition);
    vec3 treeColor = vec3(0.18, 0.14, 0.08);
    
	float depth = GetDepth(gl_FragCoord.z, near, far);
	vec3 diffuse = DiffuseLighting(normal, treeColor);
	//vec3 bladeSpecular = SpecularLighting(normal, viewDirection, 8);
	//vec3 terrainSpecular = SpecularLighting(terrainNormal, viewDirection, 16);
	vec3 endColor = diffuse;
	
	fragmentColor = vec4(endColor, 1);
}