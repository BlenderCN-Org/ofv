#include <iostream>
#include "pmlib.h"
using namespace std;

int main()
{
	int steps = 20;

	float step = 2 * pi / steps;

	float radius = 5;
	float a = 0;

	vector origin;
	vector helper;

	int vertices[3];

	vertices[0] = vertex(origin);
	int last;

	for (int i = 0; i < steps; i++)
	{
		helper.x = cos(a) * radius;
		helper.y = 0;
		helper.z = sin(a) * radius;

		if (i > 0)
		{
			vertices[1] = vertices[2];
			vertices[2] = vertex(helper);
			face(vertices, 3);
		}
		else
		{
			vertices[2] = vertex(helper);
			last = vertices[2];
		}

		a += step;
	}
	
	vertices[1] = vertices[2];
	vertices[2] = last;
	face(vertices, 3);
	
	closeFile();
}
