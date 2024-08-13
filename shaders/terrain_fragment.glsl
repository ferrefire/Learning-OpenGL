#version 460 core

in vec3 worldPosition;

out vec4 fragmentColor;

uniform vec4 color;

#include "variables.glsl"
#include "depth.glsl"
#include "transformation.glsl"
#include "heightmap.glsl"
#include "lighting.glsl"

void main()
{
    float depth = GetDepth(gl_FragCoord.z, near, far);
    float normalDepth = GetDepth(gl_FragCoord.z, near, 25000.0);

	vec3 normal = SampleNormalDynamic(worldPosition, 1.0);

	vec3 steepnessNormal = NormalPower(normal, 0.5);
	//float steepness = GetSteepness(steepnessNormal);
	float steepness = 1.0 - pow(1.0 - GetSteepness(steepnessNormal), 15.0);
    vec4 terrainColor = mix(color, vec4(0.25, 0.25, 0.25, 1.0), steepness);

    float power = mix(0.25, 1.0, 1.0 - pow(1.0 - normalDepth, 4.0));
	normal = NormalPower(normal, power);

	vec3 diffuse = DiffuseLighting(normal, terrainColor.xyz);
	vec3 endColor = Fog(diffuse, depth);

	if (newFrame == 1 && depth < 0.5)
	{
		ivec2 coordinates = ivec2(0);
		coordinates.x = int(floor(gl_FragCoord.x));
		coordinates.y = int(floor(gl_FragCoord.y));
		SetDepth(coordinates, 1.0 - depth);
	}

    fragmentColor = vec4(endColor, 1.0);
}