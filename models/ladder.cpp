#include <iostream>
#include "pmlib/pmlib.h"
using namespace std;

void cylinder(vec3 from, vec3 to, float radius, int polygons, bool caps)
{
	vec3 dir = to - from;

	vec3 localRight = dir * u;
	if (localRight == z)
	{
		localRight = r;
	}
	vec3 localUp = localRight * dir;

	localRight = localRight.normalized();
	localUp = localUp.normalized();

	float angleStep = 2 * pi / polygons;
	float currentAngle = 0;

	int cap1[polygons];
	int cap2[polygons];

	int quad[4];

	for (int i = 0; i < polygons; i++)
	{
		cap1[i] = vertex(from + localRight * cos(currentAngle) * radius + localUp * sin(currentAngle) * radius);
		cap2[i] = vertex(to + localRight * cos(currentAngle) * radius + localUp * sin(currentAngle) * radius);

		if (i > 0)
		{
			quad[0] = cap1[i];
			quad[1] = cap1[i-1];
			quad[2] = cap2[i-1];
			quad[3] = cap2[i];
			face(quad, 4);
		}

		currentAngle += angleStep;
	}
	quad[1] = quad[0];
	quad[2] = quad[3];
	quad[0] = cap1[0];
	quad[3] = cap2[0];
	face(quad, 4);
	if (caps)
	{
		face(cap1, polygons);
		face(cap2, polygons, true); // true for inverting normal
	}
}

int main()
{
	int steps = 8;
	float stepHeight = 0.5;
	float radius = 0.06;
	int polygons = 12;
	float width = 1;

	cylinder(r * (width / 2), r * (width / 2) + u * (stepHeight * (steps + 1)), radius, polygons, true);
	cylinder(-r * (width / 2), -r * (width / 2) + u * (stepHeight * (steps + 1)), radius, polygons, true);

	for (float i = stepHeight; i < stepHeight * (steps + 1); i += stepHeight)
	{
		cylinder(-r * width * 0.5 + u * i, r * width * 0.5 + u * i, radius, polygons, false);
	}
}