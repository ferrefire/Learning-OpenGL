#ifndef LIGHTING_INCLUDED
#define LIGHTING_INCLUDED

#include "variables.glsl"

vec3 DiffuseLighting(vec3 normal, vec3 color)
{
	float diffuseStrength = max(dot(normal, lightDirection), 0.0);
	vec3 diffuse = color * diffuseStrength;

	return diffuse;
}

vec3 SpecularLighting(vec3 normal, vec3 viewDirection, float shininess)
{
	vec3 halfwayDirection = normalize(lightDirection + viewDirection);
	float specular = pow(max(dot(normal, halfwayDirection), 0.0), shininess);
	vec3 specularColor = lightColor * specular;

	return specularColor;
}

vec3 Fog(vec3 color, float depth)
{
	return mix(color, vec3(1), depth);	
}

vec3 NormalPower(vec3 normal, float power)
{
	normal.xz *= power;
	return (normalize(normal));
}

#endif