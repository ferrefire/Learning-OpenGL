#ifndef FUNCTIONS_INCLUDED
#define FUNCTIONS_INCLUDED

#include "variables.glsl"

float SquaredDistance(vec3 p1, vec3 p2)
{
	vec3 difference = p2 - p1;
	return (dot(difference, difference));
}

float SquaredDistanceVec2(vec2 p1, vec2 p2)
{
	vec2 difference = p2 - p1;
	return (dot(difference, difference));
}

float SquaredDistanceToViewPosition(vec3 p)
{
	return (SquaredDistance(p, viewPosition));
}

int ValidUV(vec2 uv)
{
	return (abs(uv.x - 0.5) <= 0.5 && abs(uv.y - 0.5) <= 0.5) ? 1 : 0;
}

#endif