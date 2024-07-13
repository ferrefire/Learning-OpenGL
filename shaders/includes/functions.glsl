#ifndef FUNCTIONS_INCLUDED
#define FUNCTIONS_INCLUDED

#include "variables.glsl"

float SquaredDistance(vec3 p1, vec3 p2)
{
	vec3 difference = p2 - p1;
	return (dot(difference, difference));
}

float SquaredDistanceToViewPosition(vec3 p)
{
	return (SquaredDistance(p, viewPosition));
}

#endif