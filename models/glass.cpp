#include "rtpmlib.h"

float radius = 2;
float height = 4;
int sides = 6;

float scaleFactor = 1.3;

int vRes = 60;
int hRes = 60;

void generateModel()
{
	float vStep = height / vRes;

	float angleStep = pi * 2 / hRes;

	float sideAngle = pi * 2 / sides;

	float cAngle = 0;
	float cHeight = 0;

	int vertices[vRes][hRes];
	int cQuad[4];

	float trans = 0;
	int mid = vRes / 3.5;
	float transStep = 1.0 / mid;

	int innerEnd = vRes / 4;
	float innerRadiusFactor = 0.92;


	for (int i = 0; i < mid; i++) // outer bottom
	{
		cAngle = 0;
		for (int j = 0; j < hRes; j++)
		{
			float angleDif = cAngle;
			while (angleDif < -sideAngle/2)
				angleDif += sideAngle;
			while (angleDif > sideAngle/2)
				angleDif -= sideAngle;
			float distance = 1 / cos(angleDif);

			vertices[i][j] = vertex((1-trans) * cos(cAngle) * distance * radius + trans * cos(cAngle) * radius * scaleFactor,
									cHeight,
									(1-trans) * sin(cAngle) * distance * radius + trans * sin(cAngle) * radius * scaleFactor);

			if (i > 0 && j > 0)
			{
				cQuad[0] = vertices[i-1][j-1];
				cQuad[1] = vertices[i-1][j];
				cQuad[2] = vertices[i][j];
				cQuad[3] = vertices[i][j-1];
				face(cQuad, 4);
			}

			cAngle -= angleStep;
		}

		if (i > 0)
		{
			cQuad[0] = vertices[i-1][hRes-1];
			cQuad[1] = vertices[i-1][0];
			cQuad[2] = vertices[i][0];
			cQuad[3] = vertices[i][hRes-1];
			face(cQuad, 4);
		}

		cHeight += vStep;
		trans += transStep;
	}

	for (int i = mid; i < vRes; i++) // outer top
	{
		cAngle = 0;
		for (int j = 0; j < hRes; j++)
		{
			vertices[i][j] = vertex(cos(cAngle) * radius * scaleFactor,
									cHeight,
									sin(cAngle) * radius * scaleFactor);

			if (i > 0 && j > 0)
			{
				cQuad[0] = vertices[i-1][j-1];
				cQuad[1] = vertices[i-1][j];
				cQuad[2] = vertices[i][j];
				cQuad[3] = vertices[i][j-1];
				face(cQuad, 4);
			}

			cAngle -= angleStep;
		}

		if (i > 0)
		{
			cQuad[0] = vertices[i-1][hRes-1];
			cQuad[1] = vertices[i-1][0];
			cQuad[2] = vertices[i][0];
			cQuad[3] = vertices[i][hRes-1];
			face(cQuad, 4);
		}

		cHeight += vStep;
	}

	// bottom cap
	face(vertices[0], hRes, true);

	//inner surf
	int innerVerts[vRes - innerEnd][hRes];

	cHeight = height - vStep;
	for (int i = 0; i < vRes - innerEnd; i++)
	{
		cAngle = 0;
		for (int j = 0; j < hRes; j++)
		{
			innerVerts[i][j] = vertex(cos(cAngle) * radius * scaleFactor * innerRadiusFactor,
									cHeight,
									sin(cAngle) * radius * scaleFactor * innerRadiusFactor);

			if (i > 0 && j > 0)
			{
				cQuad[0] = innerVerts[i-1][j-1];
				cQuad[1] = innerVerts[i-1][j];
				cQuad[2] = innerVerts[i][j];
				cQuad[3] = innerVerts[i][j-1];
				face(cQuad, 4);
			}

			cAngle -= angleStep;
		}

		if (i > 0)
		{
			cQuad[0] = innerVerts[i-1][hRes-1];
			cQuad[1] = innerVerts[i-1][0];
			cQuad[2] = innerVerts[i][0];
			cQuad[3] = innerVerts[i][hRes-1];
			face(cQuad, 4);
		}
		cHeight -= vStep;
	}

	for (int i = 1; i < hRes; i++) // border
	{
		cQuad[0] = vertices[vRes - 1][i - 1];
		cQuad[1] = vertices[vRes - 1][i];
		cQuad[2] = innerVerts[0][i];
		cQuad[3] = innerVerts[0][i - 1];
		face(cQuad, 4);
	}

	//border last face
	cQuad[0] = vertices[vRes - 1][hRes - 1];
	cQuad[1] = vertices[vRes - 1][0];
	cQuad[2] = innerVerts[0][0];
	cQuad[3] = innerVerts[0][hRes - 1];
	face(cQuad, 4);

	//inner cap
	face(innerVerts[vRes - innerEnd - 1], hRes);
}
