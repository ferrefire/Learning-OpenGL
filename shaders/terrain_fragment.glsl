#version 460 core

#define FRAGMENT_STAGE

in vec2 fUV;
in vec3 fNormal;
in vec3 fFragmentPosition;
in vec4 fColor;

out vec4 oFragmentColor;

uniform vec4 color;

uniform mat4 model;

uniform sampler2D heightMap;

#include "variables.glsl"
#include "noise.glsl"
#include "depth.glsl"
#include "LOD.glsl"
#include "transformation.glsl"

void main()
{
    //oFragmentColor = texture(heightMap, fUV);
    //return ;

    float depth = GetDepth(gl_FragCoord.z, near, far);
    //float loddepth = pow(1.0 - depth, 2);
    

    vec3 normal = normalize(fNormal);
    //vec4 Color = fColor;

    //float loddepth = 1.0 - depth;
    //int lod = int(ceil((loddepth) * 10.0));
    //vec3 normal = GenerateNoiseNormal(fUV, lod, 0.001);
    //float steepness = GetSteepness(normal);
    //steepness = pow(steepness, 2);
    //vec4 Color = mix(color, vec4(0.25, 0.25, 0.25, 1), steepness);
    
    vec3 lightDirection = normalize(lightPosition - fFragmentPosition);
    

	float diffuse = dot(normal, lightDirection);
	diffuse = diffuse * 0.5f + 0.5f;
    diffuse = diffuse * diffuse;
    //vec3 diff = vec3(max(dot(norm, lightDir), 0.0));

    vec3 viewDirection = normalize(viewPosition - fFragmentPosition);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    //float specular = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
    float specular = 0;
    vec3 specularColor = vec3(0.5 * specular);

    oFragmentColor = mix(vec4(fColor.xyz * (vec3(0.25) + diffuse + specularColor), 1.0), vec4(1.0), depth);
}