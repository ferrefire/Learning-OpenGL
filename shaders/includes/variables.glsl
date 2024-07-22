#ifndef VARIABLES_INCLUDED
#define VARIABLES_INCLUDED

uniform float time;

uniform float near;
uniform float nearMult;
uniform float far;
uniform float farMult;

uniform float width;
uniform float height;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model = mat4(1.0);

uniform vec3 viewPosition;
uniform vec3 viewDirection;

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