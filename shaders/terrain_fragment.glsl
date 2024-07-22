#version 460 core

#define FRAGMENT_STAGE

//in vec2 fUV;
//in vec3 fNormal;
in vec3 worldPosition;
//in float shadow;
//in vec3 terrainNormal;
//in vec4 fColor;

out vec4 fragmentColor;

uniform vec4 color;

#include "variables.glsl"
#include "depth.glsl"
#include "LOD.glsl"
#include "transformation.glsl"
#include "heightmap.glsl"
#include "lighting.glsl"
#include "shadow.glsl"

//uniform sampler2D occlusionMap;

void main()
{
    float depth = GetDepth(gl_FragCoord.z, near, far);

	vec3 normal = SampleNormalDynamic(worldPosition, 1.0);
	//vec3 normal = terrainNormal;

	vec3 steepnessNormal = NormalPower(normal, 0.5);
	float steepness = GetSteepness(steepnessNormal);
	steepness = 1.0 - pow(1.0 - steepness, 15);
    vec4 terrainColor = mix(color, vec4(0.25, 0.25, 0.25, 1), steepness);

    float power = mix(0.25, 1.0, 1.0 - pow(1.0 - depth, 4));
	normal = NormalPower(normal, power);

	vec3 diffuse = DiffuseLighting(normal, terrainColor.xyz);
	//diffuse *= shadow;
	vec3 endColor = Fog(diffuse, depth);

    fragmentColor = vec4(endColor, 1);
}