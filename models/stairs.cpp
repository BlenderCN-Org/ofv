#include <iostream>
#include "pmlib/pmlib.h"
using namespace std;

void spiral(int steps, float stepHeight, float thickness, float innerRadius, float outerRadius, float stepAngle)
{
	float currentAngle = 0;

	int outerSide[7]; //tris
	int innerSide[7]; //tris
	int bottom[4]; // quads
	int vertices[4]; //quads

	vec3 pos(cos(currentAngle) * innerRadius,
			0,
			sin(currentAngle) * innerRadius);

	vertices[0] = vertex(pos);
	innerSide[1] = innerSide[3] = innerSide[5] = vertices[0];
	bottom[0] = vertex(pos - u * thickness);
	innerSide[0] = bottom[0];
	pos.x = cos(currentAngle) * outerRadius;
	pos.z = sin(currentAngle) * outerRadius;
	vertices[1] = vertex(pos);
	outerSide[6] = outerSide[2] = vertices[1];
	bottom[3] = vertex(pos - u * thickness);
	outerSide[0] = bottom[3];

	for (int i = 0; i < steps; i++)
	{
		pos += u * stepHeight;
		vertices[2] = vertex(pos);
		outerSide[5] = vertices[2];
		pos.x = cos(currentAngle) * innerRadius;
		pos.z = sin(currentAngle) * innerRadius;
		vertices[3] = vertex(pos);
		innerSide[6] = vertices[3];
		face(vertices, 4);

		currentAngle -= stepAngle;
		vertices[0] = vertices[3];
		vertices[1] = vertices[2];
		pos.x = cos(currentAngle) * outerRadius;
		pos.z = sin(currentAngle) * outerRadius;
		vertices[2] = vertex(pos);
		bottom[2] = vertex(pos - u * thickness);
		outerSide[1] = outerSide[3] = bottom[2];
		outerSide[4] = vertices[2];
		pos.x = cos(currentAngle) * innerRadius;
		pos.z = sin(currentAngle) * innerRadius;
		vertices[3] = vertex(pos);
		innerSide[4] = vertices[3];
		bottom[1] = vertex(pos - u * thickness);
		innerSide[2] = bottom[1];
		face(vertices, 4);
		face(bottom, 4);
		face(outerSide, 3);
		face(outerSide, 3, 2);
		face(outerSide, 3, 4);
		face(innerSide, 3);
		face(innerSide, 3, 2);
		face(innerSide, 3, 4);

		vertices[0] = vertices[3];
		vertices[1] = vertices[2];

		bottom[0] = bottom[1];
		bottom[3] = bottom[2];

		outerSide[6] = outerSide[2] = outerSide[4];
		outerSide[0] = outerSide[1];

		innerSide[0] = innerSide[2];
		innerSide[1] = innerSide[3] = innerSide[5] = innerSide[4];

		pos.x = cos(currentAngle) * outerRadius;
		pos.z = sin(currentAngle) * outerRadius;
	}
}

void ushape(vec3 origin, vec3 dir, int steps, float stepHeight, float stepSize, float stairsWidth, float midSize, float thickness, int stepsPerSegment)
{
	dir.y = 0;
	vec3 localRight = dir * u;
	dir = dir.normalized();
	localRight = localRight.normalized();

	int quad[4];
	int thQuad[4];
	int sideTri[3];
	int midVertices[20];

	quad[0] = vertex(origin);
	quad[1] = vertex(origin + localRight * stairsWidth);

	thQuad[0] = vertex(origin - u * thickness);
	thQuad[1] = vertex(origin + localRight * stairsWidth - u * thickness);

	for (int i = 1; i <= steps; i++)
	{
		origin += u * stepHeight;
		quad[2] = vertex(origin + localRight * stairsWidth);
		quad[3] = vertex(origin);

		sideTri[2] = quad[1];
		int leftSideTemp = quad[0];

		face(quad, 4);

		quad[0] = quad[3];
		quad[1] = quad[2];

		origin += dir * stepSize;
		quad[2] = vertex(origin + localRight * stairsWidth);
		quad[3] = vertex(origin);

		thQuad[2] = vertex(origin + localRight * stairsWidth - u * thickness);
		thQuad[3] = vertex(origin - u * thickness);

		face(quad, 4);
		face(thQuad, 4, true);

		//right side

		sideTri[0] = thQuad[1];
		sideTri[1] = thQuad[2];
		face(sideTri, 3);

		sideTri[0] = sideTri[1];
		sideTri[1] = quad[2];
		face(sideTri, 3);

		sideTri[0] = sideTri[1];
		sideTri[1] = quad[1];
		face(sideTri, 3);

		//left side

		sideTri[2] = leftSideTemp;
		sideTri[0] = thQuad[0];
		sideTri[1] = thQuad[3];
		face(sideTri, 3, true);

		sideTri[0] = sideTri[1];
		sideTri[1] = quad[3];
		face(sideTri, 3, true);

		sideTri[0] = sideTri[1];
		sideTri[1] = quad[0];
		face(sideTri, 3, true);

		if (i % stepsPerSegment == 0) // mid
		{
			midVertices[0] = midVertices[12] = thQuad[3];
			midVertices[1] = midVertices[18] = quad[3];

			midVertices[5] = midVertices[2] = midVertices[17] = vertex(origin + dir * midSize);
			midVertices[4] = midVertices[3] = midVertices[13] = vertex(origin + dir * midSize - u * thickness);

			origin += localRight * stairsWidth * 2;
			localRight = -localRight;
			dir = -dir;
			quad[0] = vertex(origin);
			quad[1] = vertex(origin + localRight * stairsWidth);
			thQuad[0] = vertex(origin - u * thickness);
			thQuad[1] = vertex(origin + localRight * stairsWidth - u * thickness);

			midVertices[10] = midVertices[19] = quad[0];
			midVertices[11] = midVertices[15] = thQuad[0];

			midVertices[9] = midVertices[6] = midVertices[16] = vertex(origin - dir * midSize);
			midVertices[8] = midVertices[7] = midVertices[14] = vertex(origin - dir * midSize - u * thickness);

			faceSeq(midVertices, 20, 4);
		}
		else
		{
			quad[0] = quad[3];
			quad[1] = quad[2];
			thQuad[0] = thQuad[3];
			thQuad[1] = thQuad[2];
		}
	}
}

int main()
{
	//spiral(60, 0.2, 0.3, 0.5, 4, 2 * pi / 50);
	ushape(z, -b, 40, 0.1, 0.2, 1, 1, 0.1, 15);
	closeFile();
}
