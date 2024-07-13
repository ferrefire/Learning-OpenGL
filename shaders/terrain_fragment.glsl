#version 460 core

#define FRAGMENT_STAGE

//in vec2 fUV;
//in vec3 fNormal;
in vec3 worldPosition;
//in vec4 fColor;

out vec4 fragmentColor;

uniform vec4 color;

#include "variables.glsl"
#include "depth.glsl"
#include "LOD.glsl"
#include "transformation.glsl"
#include "heightmap.glsl"

void main()
{
    float depth = GetDepth(gl_FragCoord.z, near, far);

	vec3 normal = SampleNormalDynamic(worldPosition.xz, 0.5);
	float steepness = GetSteepness(normal);

	//only sample normal once

    float power = mix(0.25, 1.0, 1.0 - pow(1.0 - depth, 4));
	normal = SampleNormalDynamic(worldPosition.xz, power);

    steepness = 1.0 - pow(1.0 - steepness, 15);
    vec4 Color = mix(color, vec4(0.25, 0.25, 0.25, 1), steepness);
    
    //vec3 lightDirection = normalize(lightPosition - worldPosition);
    vec3 lightDirection = normalize(lightDirection);
    

	//float diffuse = dot(normal, lightDirection);
	//diffuse = diffuse * 0.5 + 0.5;
    //diffuse = diffuse * diffuse;
    vec3 diffuse = vec3(max(dot(normal, lightDirection), 0.0));

    //vec3 viewDirection = normalize(viewPosition - worldPosition);
    //vec3 reflectDirection = reflect(-lightDirection, normal);
    //float specular = pow(max(dot(viewDirection, reflectDirection), 0.0), 128);
    //vec3 specularColor = vec3(1.0, 0.75, 0.0) * (0.1 * specular);

    fragmentColor = mix(vec4(Color.xyz * (vec3(0.25) + diffuse), 1.0), vec4(1.0), depth);
}