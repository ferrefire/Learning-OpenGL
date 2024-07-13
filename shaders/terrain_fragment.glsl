#version 460 core

#define FRAGMENT_STAGE

in vec2 fUV;
//in vec3 fNormal;
in vec3 fFragmentPosition;
//in vec4 fColor;

out vec4 oFragmentColor;

uniform vec4 color;

#include "variables.glsl"
#include "depth.glsl"
#include "LOD.glsl"
#include "transformation.glsl"
#include "heightmap.glsl"

void main()
{
    //oFragmentColor = texture(heightMap, fUV);
    //return ;

    float depth = GetDepth(gl_FragCoord.z, near, far);
    //float loddepth = pow(1.0 - depth, 2);
    
	//vec3 normal = fNormal;
	//vec4 Color = fColor;
	//if (false && depth < 0.1)
	//{
	//	normal = normalize(normal);
	//}
	//else
	//{
	//	normal = SampleNormal(fUV, 1);
	//	float steepness = GetSteepness(normal);
    //	steepness = pow(steepness, 2);
    //	Color = mix(color, vec4(0.25, 0.25, 0.25, 1), steepness);
	//}

	
	//vec3 normal = SampleNormal(fUV, 0.25);
	bool lod = IsLod(fFragmentPosition);
	//vec3 normal = SampleNormalDynamic(fFragmentPosition, lod ? 0.1 : 0.25);
	vec3 normal = SampleNormalV2(fFragmentPosition.xz, 0.25);
	//vec3 normal = SampleArrayNormal(fUV, lod ? 0.1 : 0.25);
	//vec3 normal = SampleArrayNormal(fUV, 0.1);
	//vec3 normal = SampleNormalDynamic(fFragmentPosition, 0.25);
	float steepness = GetSteepnessDynamic(fFragmentPosition, normal);

    //float power = mix(0.25, lod ? 0.5 : 1.0, 1.0 - pow(1.0 - depth, 4));
    float power = mix(0.25, 1.0, 1.0 - pow(1.0 - depth, 4));
	normal = SampleNormalV2(fFragmentPosition.xz, power);
	//normal = SampleArrayNormalUnNorm(fUV);
	//vec3 steepnessNormal = normal;
	//normal.xz *= power;
	//normal = normalize(normal);
	//steepnessNormal.xz *= 0.5;
	//steepnessNormal = normalize(steepnessNormal);

    //oFragmentColor = vec4(vec3(steepness), 1.0);
    //return ;

	
    steepness = 1.0 - pow(1.0 - steepness, 15);
    vec4 Color = mix(color, vec4(0.25, 0.25, 0.25, 1), steepness);

    
    //vec4 Color = fColor;

    //float loddepth = 1.0 - depth;
    //int lod = int(ceil((loddepth) * 10.0));
    //vec3 normal = GenerateNoiseNormal(fUV, lod, 0.001);
    //float steepness = GetSteepness(normal);
    //steepness = pow(steepness, 2);
    //vec4 Color = mix(color, vec4(0.25, 0.25, 0.25, 1), steepness);
    
    //vec3 lightDirection = normalize(lightPosition - fFragmentPosition);
    vec3 lightDirection = normalize(lightDirection);
    

	float diffuse = dot(normal, lightDirection);
	diffuse = diffuse * 0.5 + 0.5;
    diffuse = diffuse * diffuse;
    //vec3 diff = vec3(max(dot(norm, lightDir), 0.0));

    vec3 viewDirection = normalize(viewPosition - fFragmentPosition);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float specular = pow(max(dot(viewDirection, reflectDirection), 0.0), 128);
    vec3 specularColor = vec3(1.0, 0.75, 0.0) * (0.1 * specular);

    oFragmentColor = mix(vec4(Color.xyz * (vec3(0.25) + diffuse) + specularColor, 1.0), vec4(1.0), depth);
}