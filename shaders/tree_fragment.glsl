#version 460 core

#define FRAGMENT_STAGE

in vec2 UV;
in vec3 normal;
in vec3 worldPosition;

out vec4 fragmentColor;

#include "variables.glsl"
#include "depth.glsl"
#include "lighting.glsl"

void main()
{
    vec3 normal = normalize(normal);

    vec3 viewDirection = normalize(viewPosition - worldPosition);
    vec3 treeColor = vec3(0.18, 0.14, 0.08);
    
	float depth = GetDepth(gl_FragCoord.z, near, far);
	vec3 diffuse = DiffuseLighting(normal, treeColor);
	//vec3 bladeSpecular = SpecularLighting(normal, viewDirection, 8);
	//vec3 terrainSpecular = SpecularLighting(terrainNormal, viewDirection, 16);
	vec3 endColor = Fog(diffuse, depth);

	if (newFrame == 1 && depth < 0.25)
	{
		ivec2 coordinates = ivec2(0);
		coordinates.x = int(floor(gl_FragCoord.x));
		coordinates.y = int(floor(gl_FragCoord.y));
		//if (imageLoad(depthMapImage, coordinates).r < (1.0 - depth)) imageStore(depthMapImage, coordinates, vec4(1.0 - depth));
		SetDepth(coordinates, 1.0 - depth);
	}
	
	//fragmentColor = vec4(endColor, 1);
	fragmentColor = vec4(normal * 0.5 + 0.5, 1);
}