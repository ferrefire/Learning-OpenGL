#ifndef CURVE_INCLUDED
#define CURVE_INCLUDED

#define LOWLANDS vec2(0.0, 0.0), vec2(0.25, 0.15), vec2(1.0, 0.0), vec2(1.0, 1.0)

float CubicCurve(vec2 P1, vec2 P2, vec2 P3, vec2 P4, float t)
{
	vec2 r1 = pow(1.0 - t, 3) * P1;
	vec2 r2 = 3 * pow(1.0 - t, 2) * t * P2;
	vec2 r3 = 3 * (1.0 - t) * pow(t, 2) * P3;
	vec2 r4 = pow(t, 3) * P4;

	vec2 r = r1 + r2 + r3 + r4;

	return (r.y);
}

//vec2 GrassCurve(float power, float t)
//{
//	vec2 p1 = vec2(0.0, 0.0);
//	vec2 p2 = vec2(0.5, 0.5 - power * 0.5);
//	vec2 p3 = vec2(0.5, 1.0 - power * 2);
//	vec2 p4 = vec2(1.0, 1.0);
//
//	vec2 r1 = pow(1.0 - t, 3) * p1;
//	vec2 r2 = 3 * pow(1.0 - t, 2) * t * p2;
//	vec2 r3 = 3 * (1.0 - t) * pow(t, 2) * p3;
//	vec2 r4 = pow(t, 3) * p4;
//
//	vec2 r = r1 + r2 + r3 + r4;
//
//	return (r);
//}

#endif