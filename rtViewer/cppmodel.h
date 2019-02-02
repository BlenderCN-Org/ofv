#include <iostream>
#include "pmlib/pmlib.h"
using namespace std;

int main()
{
	int steps = 200;

	float step = 2 * pi / steps;

	float radius = 1;
	float a = 0;

	vec3 helper;

	int vertices[3];

	vertices[0] = vertex(0, 0, 0);
	//int last;

	float height = 0;

	for (int i = 0; i < steps; i++)
	{
		helper.x = cos(a) * radius;
		helper.y = height;
		helper.z = sin(a) * radius;

		height += 0.005;

		if (i > 0)
		{
			vertices[1] = vertices[2];
			vertices[2] = vertex(helper);
			face(vertices, 3);
		}
		else
		{
			vertices[2] = vertex(helper);
			//last = vertices[2];
		}

		a -= step;
	}
	
	//vertices[1] = vertices[2];
	//vertices[2] = last;
	//face(vertices, 3);
	
	closeFile();
}
