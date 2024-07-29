#ifndef VARIABLES_INCLUDED
#define VARIABLES_INCLUDED

uniform float time;

uniform float near;
uniform float nearMult;
uniform float far;
uniform float farMult;

uniform float width;
uniform float height;

uniform mat4 view = mat4(1.0);
uniform mat4 projection = mat4(1.0);
uniform mat4 model = mat4(1.0);

uniform vec3 viewPosition = vec3(0);
uniform vec3 viewDirection = vec3(0);
uniform vec3 viewRight = vec3(0);
uniform vec3 viewUp = vec3(0);

uniform vec3 lightPosition;
uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform float lightStrength;

uniform float terrainSize;
uniform float terrainSizeMult;
uniform float terrainChunkSize;
uniform float terrainChunkSizeMult;
uniform float terrainHeight;
uniform float terrainHeightMult;
uniform float terrainOccludeSize;
uniform float terrainOccludeSizeMult;
uniform float terrainLod0Size;
uniform float terrainLod0SizeMult;
uniform float terrainLod1Size;
uniform float terrainLod1SizeMult;
uniform float terrainShadowSize;
uniform float terrainShadowSizeMult;

uniform vec2 terrainWorldOffset;

#endif