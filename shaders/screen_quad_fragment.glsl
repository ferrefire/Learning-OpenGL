#version 460 core

#define FRAGMENT_STAGE

in vec2 UV;

out vec4 fragmentColor;

uniform sampler2D quadTexture;

#include "curve.glsl"

void main()
{
	float col = 0;
	//vec2 p1 = vec2(0.0, 0.0);
	//vec2 p2 = vec2(0.5, 0.5);
	//vec2 p3 = vec2(1.0, 1.0);
	//vec2 p4 = vec2(1.0, 1.0);
	//vec2 curveVal = GrassCurve(1, UV.x);
	//if ((abs(curveVal.y - UV.y)) < 0.01) col = 1;

    //fragmentColor = vec4(textureLod(quadTexture, UV, 0).rgb, 1.0);
    fragmentColor = vec4(vec3(col), 1.0);
}