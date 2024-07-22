#version 460 core

#define FRAGMENT_STAGE

in vec2 UV;
in vec3 normal;
in vec3 terrainNormal;
in vec3 worldPosition;
in vec4 Color;
in float shadow;

out vec4 fragmentColor;

#include "variables.glsl"
#include "depth.glsl"
#include "lighting.glsl"

void main()
{
    vec3 terrainNormal = normalize(terrainNormal);
    vec3 normal = normalize(normal);
    if (!gl_FrontFacing) normal *= -1;

    vec3 viewDirection = normalize(viewPosition - worldPosition);
    vec3 bladeColor = mix(Color.xyz * 0.5, Color.xyz, UV.y) * shadow;
    
	float depth = GetDepth(gl_FragCoord.z, near, far);
	vec3 diffuse = DiffuseLighting(terrainNormal, bladeColor);
	vec3 bladeSpecular = SpecularLighting(normal, viewDirection, 8);
	vec3 terrainSpecular = SpecularLighting(terrainNormal, viewDirection, 16);
	vec3 endColor = diffuse + bladeSpecular * terrainSpecular * shadow;
	
	fragmentColor = vec4(endColor, 1);
}