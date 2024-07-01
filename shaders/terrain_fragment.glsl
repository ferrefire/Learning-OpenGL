#version 460 core

in vec2 fUV;
in vec3 fNormal;
in vec3 fFragmentPosition;
in vec4 fColor;

out vec4 oFragmentColor;

uniform sampler2D texture;
uniform float time;

uniform vec3 lightPosition;
uniform vec3 viewPosition;
uniform float near;
uniform float far;

#include "depth.glsl"

void main()
{
    vec3 lightDirection = normalize(lightPosition - fFragmentPosition);
    vec3 normal = normalize(fNormal);

	float diffuse = dot(normal, lightDirection);
	diffuse = diffuse * 0.5f + 0.5f;
    diffuse = diffuse * diffuse;
    //vec3 diff = vec3(max(dot(norm, lightDir), 0.0));

    vec3 viewDirection = normalize(viewPosition - fFragmentPosition);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    //float specular = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
    float specular = 0;
    vec3 specularColor = vec3(0.5 * specular);

    oFragmentColor = mix(vec4(fColor.xyz * (vec3(0.25) + diffuse + specularColor), 1.0), vec4(1.0), GetDepth());
}