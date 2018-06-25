#include <iostream>
#include "pmlib/pmlib.h"
using namespace std;

int main()
{
	float zScale = 30;
	float xScale = 5;
	float yScale = 4;

	int nSteps = 60;
	int xSteps = 15;

	float yFactor = 1;
	float xFactor = 1;

	float nStep = 1.0 / nSteps;
	float xStep = 1.0 / xSteps;

	float n = 0;
	float x = 0;

	vec3 helper;

	int vertices[nSteps][xSteps];
	int currentQuad[4];

	for (int i = 0; i < nSteps; i++)
	{
		x = 0;
		yFactor = 1 + pow(0.8 * n, 4)*3;
		xFactor = 1 - pow(0.6 * n + 0.4, 7);
		for (int j = 0; j < xSteps; j++)
		{
			helper = -b * n * zScale + r * x * xScale * xFactor;
			helper.y = n * 0.5 * sqrt(x) + (1.0 - n * 0.5) * pow(x, 3);
			helper.y *= yScale * yFactor;

			//cout << n << ", " << x << endl;

			vertices[i][j] = vertex(helper);

			if (i > 0 && j > 0)
			{
				currentQuad[0] = vertices[i-1][j-1];
				currentQuad[1] = vertices[i][j-1];
				currentQuad[2] = vertices[i][j];
				currentQuad[3] = vertices[i-1][j];
				face(currentQuad, 4);
			}

			x += xStep;
		}
		n += nStep;
	}

	n = 0;
	for (int i = 0; i < nSteps; i++)
	{
		x = 0;
		yFactor = 1 + pow(0.8 * n, 4)*3;
		xFactor = 1 - pow(0.6 * n + 0.4, 7);
		for (int j = 0; j < xSteps; j++)
		{
			helper = -b * n * zScale - r * x * xScale * xFactor;
			helper.y = n * 0.5 * sqrt(x) + (1.0 - n * 0.5) * pow(x, 3);
			helper.y *= yScale * yFactor;

			//cout << n << ", " << x << endl;

			vertices[i][j] = vertex(helper);

			if (i > 0 && j > 0)
			{
				currentQuad[0] = vertices[i-1][j-1];
				currentQuad[1] = vertices[i][j-1];
				currentQuad[2] = vertices[i][j];
				currentQuad[3] = vertices[i-1][j];
				face(currentQuad, 4, true);
			}

			x += xStep;
		}
		n += nStep;
	}
}