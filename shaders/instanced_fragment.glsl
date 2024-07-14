#version 460 core

#define FRAGMENT_STAGE

in vec2 UV;
in vec3 normal;
in vec3 terrainNormal;
in vec3 worldPosition;
in vec4 Color;

out vec4 fragmentColor;

#include "variables.glsl"
#include "depth.glsl"

void main()
{
    //vec3 lightDirection = normalize(lightPosition - worldPosition);
    vec3 lightDirection = normalize(lightDirection);
    vec3 terrainNormal = normalize(terrainNormal);
    vec3 normal = normalize(normal);
    if (!gl_FrontFacing) normal *= -1;

	//float diffuse = dot(terrainNormal, lightDirection);
	//diffuse = diffuse * 0.5f + 0.5f;
    //diffuse = diffuse * diffuse;
	float diffuseStrength = max(dot(terrainNormal, lightDirection), 0.0);
    vec3 diffuse = vec3(diffuseStrength);
	diffuse += vec3(max(dot(terrainNormal, vec3(0, 1, 0)), 0.0) * (1.0 - diffuseStrength) * 0.25);

    vec3 viewDirection = normalize(viewPosition - worldPosition);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    vec3 terrainReflectDirection = reflect(-lightDirection, terrainNormal);
    float specular = pow(max(dot(viewDirection, reflectDirection), 0.0), 8);
    float terrainSpecular = pow(max(dot(viewDirection, terrainReflectDirection), 0.0), 64);
    //vec3 specularColor = vec3(1.0, 0.75, 0.0) * (1 * (specular * terrainSpecular));
    vec3 specularColor = vec3(1.0) * (1 * (specular * terrainSpecular));
    vec3 bladeColor = mix(Color.xyz * 0.5, Color.xyz, UV.y);
    

    fragmentColor = mix(vec4(bladeColor * diffuse + specularColor, 1.0), vec4(1.0), GetDepth(gl_FragCoord.z, near, far));
	//oFragmentColor = vec4(bladeColor * (vec3(0.25) + diffuse + specularColor), 1.0);
}