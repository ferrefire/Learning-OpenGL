#version 460 core

#define FRAGMENT_STAGE

in vec2 UV;
in vec3 Normal;
in vec3 TerrainNormal;
in vec3 FragmentPosition;
in vec4 Color;

out vec4 oFragmentColor;

uniform sampler2D texture;

#include "variables.glsl"
#include "depth.glsl"

void main()
{
    //vec3 lightDirection = normalize(lightPosition - FragmentPosition);
    vec3 lightDirection = normalize(lightDirection);
    vec3 terrainNormal = normalize(TerrainNormal);
    vec3 normal = normalize(Normal);
    if (!gl_FrontFacing) normal *= -1;

	float diffuse = dot(terrainNormal, lightDirection);
	diffuse = diffuse * 0.5f + 0.5f;
    diffuse = diffuse * diffuse;
    //vec3 diff = vec3(max(dot(norm, lightDir), 0.0));

    vec3 viewDirection = normalize(viewPosition - FragmentPosition);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    vec3 terrainReflectDirection = reflect(-lightDirection, terrainNormal);
    float specular = pow(max(dot(viewDirection, reflectDirection), 0.0), 4);
    float terrainSpecular = pow(max(dot(viewDirection, terrainReflectDirection), 0.0), 128);
    vec3 specularColor = vec3(1.0, 0.75, 0.0) * (1 * (specular * terrainSpecular));
    vec3 bladeColor = mix(Color.xyz * 0.5, Color.xyz, UV.y);
    

    oFragmentColor = mix(vec4(bladeColor * (vec3(0.25) + diffuse) + specularColor, 1.0), vec4(1.0), GetDepth(gl_FragCoord.z, near, far));
	//oFragmentColor = vec4(bladeColor * (vec3(0.25) + diffuse + specularColor), 1.0);
}