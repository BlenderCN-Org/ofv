#include "rtpmlib.h"

int n = 5;
float radius = 1;

void generateModel()
{
float angle = pi * 2 / n;
float theta = 0;
float phi = 0;
	int vertices[n];

	for (int j = 0; j < n * (n - 1) / 2; j++)
	{
		for (int i = 0; i < n; i++)
		{
			vertices[i] = vertex(cos(theta) * cos(phi) * radius,
								sin(phi) * radius,
								sin(theta) * cos(phi) * radius);

			theta -= angle;
		}
		face(vertices, n);
		phi -= angle;
	}
}