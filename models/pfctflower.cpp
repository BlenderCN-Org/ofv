#include "rtpmlib.h"

float radius = 2;
float height = 2;
int hResolution = 4;
int vResolution = 60;
int sides = 8;

void generateModel()
{
	float currentRadius = radius;
	float angleStep = 2 * pi / hResolution;
	float yStep = height / vResolution;

	float currentAngle = 0;
	float currentHeight = 0;

	int vertices[vResolution][hResolution];
	int currentQuad[4];

	for (int i = 0; i < vResolution; i++)
	{
		currentAngle = 0;
		for (int j = 0; j < hResolution; j++)
		{
			vertices[i][j] = vertex((cos(currentAngle) - cos(currentAngle * sides) / sides) * currentRadius,
									currentHeight,
									(sin(currentAngle) - sin(currentAngle * sides) / sides) * currentRadius);

			if (j > 0 && i > 0)
			{
				currentQuad[0] = vertices[i - 1][j];
				currentQuad[1] = vertices[i][j];
				currentQuad[2] = vertices[i][j - 1];
				currentQuad[3] = vertices[i - 1][j - 1];
				face(currentQuad, 4);
			}
			currentAngle -= angleStep;
		}

		if (i > 0)
		{
			currentQuad[0] = vertices[i - 1][0];
			currentQuad[1] = vertices[i][0];
			currentQuad[2] = vertices[i][hResolution - 1];
			currentQuad[3] = vertices[i - 1][hResolution - 1];
			face(currentQuad, 4);
		}

		currentRadius /= 1.07;
		currentHeight -= yStep;
	}
}