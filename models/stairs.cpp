#include <iostream>
#include "pmlib/pmlib.h"
using namespace std;

float stepSize = 0.15;
float stepHeight = 0.07;
float stepAngle = 2 * pi / 30;
float thickness = 0.2;

bool createRailings = true;
bool createSideCap = true;

float railingHOffset = 0.06;
float railingVOffset = -0.03;

float railingWidth = 0.05;
float railingThickness = 0.05;

int railingPipeEachXSteps = 3;

float railingPipeHeight = 0.7;
int railingPipeRes = 1;
float railingPipeRadius = 0.01;
float railingPipeTwistAngle = 0;
int railingPipePolygonSides = 32;

void createRailingSeg(int* array, bool invert)
{
	int quad[4];
	if (invert)
	{
		quad[0] = array[4];
		quad[1] = array[7];
		quad[2] = array[3];
		quad[3] = array[0];
		face(quad, 4);

		quad[0] = array[6];
		quad[1] = array[2];
		quad[2] = array[3];
		quad[3] = array[7];
		face(quad, 4);

		quad[0] = array[2];
		quad[1] = array[6];
		quad[2] = array[5];
		quad[3] = array[1];
		face(quad, 4);

		quad[0] = array[4];
		quad[1] = array[0];
		quad[2] = array[1];
		quad[3] = array[5];
		face(quad, 4);
		return;
	}
	quad[0] = array[4];
	quad[1] = array[0];
	quad[2] = array[3];
	quad[3] = array[7];
	face(quad, 4);
	quad[0] = array[3];
	quad[1] = array[2];
	quad[2] = array[6];
	quad[3] = array[7];
	face(quad, 4);
	quad[0] = array[1];
	quad[1] = array[5];
	quad[2] = array[6];
	quad[3] = array[2];
	face(quad, 4);
	quad[0] = array[0];
	quad[1] = array[4];
	quad[2] = array[5];
	quad[3] = array[1];
	face(quad, 4);
}

void createTwistedRailingPipe(vec3 origin)
{
	float yStep = railingPipeHeight / railingPipeRes;

	int vertices[railingPipePolygonSides][railingPipeRes + 1];
	int quad[4];

	float angle = 0;

	for (int i = 0; i < railingPipePolygonSides; i++)
	{
		float currentAngle = angle;
		float currentHeight = 0;
		float angleStep = railingPipeTwistAngle / railingPipeRes;

		for (int j = 0; j < railingPipeRes + 1; j++)
		{
			vertices[i][j] = vertex(origin + r * cos(currentAngle) * railingPipeRadius -b * sin(currentAngle) * railingPipeRadius + u * currentHeight);

			if (i > 0 && j > 0)
			{
				quad[0] = vertices[i - 1][j - 1];
				quad[1] = vertices[i][j - 1];
				quad[2] = vertices[i][j];
				quad[3] = vertices[i - 1][j];
				face(quad, 4);
			}

			currentAngle += angleStep;
			currentHeight += yStep;
		}


		angle += pi * 2 / railingPipePolygonSides;
	}

	for (int j = 0; j < railingPipeRes + 1; j++)
	{
		if (j == 0)
			continue;

		quad[0] = vertices[railingPipePolygonSides - 1][j - 1];
		quad[1] = vertices[0][j - 1];
		quad[2] = vertices[0][j];
		quad[3] = vertices[railingPipePolygonSides - 1][j];
		face(quad, 4);
	}
}

void spiral(vec3 origin, int steps, float innerRadius, float outerRadius)
{
	float currentAngle = 0;

	int outerSide[7]; //tris
	int innerSide[7]; //tris
	int bottom[4]; // quads
	int vertices[4]; //quads

	vec3 pos(cos(currentAngle) * innerRadius,
			0,
			sin(currentAngle) * innerRadius);

	vertices[0] = vertex(origin + pos);
	innerSide[1] = innerSide[3] = innerSide[5] = vertices[0];
	bottom[0] = vertex(origin + pos - u * thickness);
	innerSide[0] = bottom[0];
	pos.x = cos(currentAngle) * outerRadius;
	pos.z = sin(currentAngle) * outerRadius;
	vertices[1] = vertex(origin + pos);
	outerSide[6] = outerSide[2] = vertices[1];
	bottom[3] = vertex(origin + pos - u * thickness);
	outerSide[0] = bottom[3];

	for (int i = 0; i < steps; i++)
	{
		pos += u * stepHeight;
		vertices[2] = vertex(origin + pos);
		outerSide[5] = vertices[2];
		pos.x = cos(currentAngle) * innerRadius;
		pos.z = sin(currentAngle) * innerRadius;
		vertices[3] = vertex(origin + pos);
		innerSide[6] = vertices[3];
		face(vertices, 4);

		currentAngle -= stepAngle;
		vertices[0] = vertices[3];
		vertices[1] = vertices[2];
		pos.x = cos(currentAngle) * outerRadius;
		pos.z = sin(currentAngle) * outerRadius;
		vertices[2] = vertex(origin + pos);
		bottom[2] = vertex(origin + pos - u * thickness);
		outerSide[1] = outerSide[3] = bottom[2];
		outerSide[4] = vertices[2];
		pos.x = cos(currentAngle) * innerRadius;
		pos.z = sin(currentAngle) * innerRadius;
		vertices[3] = vertex(origin + pos);
		innerSide[4] = vertices[3];
		bottom[1] = vertex(origin + pos - u * thickness);
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

void createUshapeBasis(vec3 origin, vec3 dir, int steps, float stairsWidth, float midSize, int stepsPerSegment)
{
	dir.y = 0;
	vec3 localRight = dir * u;
	dir = dir.normalized();
	localRight = localRight.normalized();

	int quad[4];
	int thQuad[4];
	int sideTri[3];

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

		if (createSideCap)
		{
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
		}

		if (i % stepsPerSegment == 0 && i != steps) // mid
		{
			int midVertices[18];
			midVertices[1] = thQuad[3];
			midVertices[13] = quad[3];

			midVertices[10] = midVertices[12] = vertex(origin + dir * midSize);
			midVertices[2] = midVertices[6] = vertex(origin + dir * midSize - u * thickness);

			midVertices[14] = midVertices[11] = midVertices[16] = quad[2];
			midVertices[0] = midVertices[5] = midVertices[8] = thQuad[2];

			origin += localRight * stairsWidth * 2;
			localRight = -localRight;
			dir = -dir;
			quad[0] = vertex(origin);
			quad[1] = quad[2]; //vertex(origin + localRight * stairsWidth);
			thQuad[0] = vertex(origin - u * thickness);
			thQuad[1] = thQuad[2]; //vertex(origin + localRight * stairsWidth - u * thickness);

			midVertices[17] = quad[0];
			midVertices[4] = thQuad[0];

			midVertices[15] = midVertices[9] = vertex(origin - dir * midSize);
			midVertices[7] = midVertices[3] = vertex(origin - dir * midSize - u * thickness);

			faceSeq(midVertices, 18, 3);

			if (createSideCap)
			{
				int midCapVertices[12];

				midCapVertices[0] = midVertices[1];
				midCapVertices[1] = midVertices[13];
				midCapVertices[3] = midCapVertices[6] = midVertices[2];
				midCapVertices[2] = midCapVertices[7] = midVertices[10];

				midCapVertices[8] = midVertices[17];
				midCapVertices[9] = midVertices[4];
				midCapVertices[4] = midCapVertices[11] = midVertices[15];
				midCapVertices[5] = midCapVertices[10] = midVertices[7];

				faceSeq(midCapVertices, 12, 4);
			}
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

void placeUshapeRailingPipes(vec3 origin, vec3 dir, int steps, float stairsWidth, float midSize, int stepsPerSegment)
{
	dir.y = 0;
	vec3 localRight = dir * u;
	dir = dir.normalized();
	localRight = localRight.normalized();

	origin += u * stepHeight + dir * stepSize * 0.5;

	for (int i = 1; i <= steps; i++)
	{
		if (i % railingPipeEachXSteps == 0 || (i - 1) % stepsPerSegment == 0)
		{
			createTwistedRailingPipe(origin + localRight * railingHOffset);
			createTwistedRailingPipe(origin + localRight * (stairsWidth - railingHOffset));
		}

		if (i % stepsPerSegment == 0 && i != steps) // mid
		{
			createTwistedRailingPipe(origin + localRight * railingHOffset + dir * (stepSize * 0.5 + midSize - railingHOffset));
			createTwistedRailingPipe(origin + localRight * (stairsWidth * 2 - railingHOffset) + dir * (stepSize * 0.5 + midSize - railingHOffset));

			origin += localRight * stairsWidth * 2 + u * stepHeight;
			localRight = -localRight;
			dir = -dir;
		}
		else
		{
			origin += u * stepHeight + dir * stepSize;
		}
	}
}

void createOuterRailing(vec3 origin, vec3 dir, int steps, float stairsWidth, float midSize, int stepsPerSegment)
{
	dir.y = 0;
	vec3 localRight = dir * u;
	dir = dir.normalized();
	localRight = localRight.normalized();

	int segments = (steps - 1) / stepsPerSegment;
	int lastSegmentSteps = (steps - 1) % stepsPerSegment + 1;
	cout << segments << ", " << lastSegmentSteps << endl;

	vec3 cursor = origin + u * (stepHeight * 0.5 + railingPipeHeight + railingVOffset) + localRight * (railingHOffset - railingWidth * 0.5);

	int railingSeg[8];
	railingSeg[0] = vertex(cursor);
	railingSeg[1] = vertex(cursor + localRight * railingWidth);
	railingSeg[2] = vertex(cursor + localRight * railingWidth + u * railingThickness);
	railingSeg[3] = vertex(cursor + u  * railingThickness);
	face(railingSeg, 4);

	vec3 stepDisplacement = u * stepHeight + dir * stepSize;

	for (int i = 0; i < segments; i++)
	{
		cursor += stepDisplacement * (stepsPerSegment - 0.5);
		railingSeg[4] = vertex(cursor);
		railingSeg[5] = vertex(cursor + localRight * railingWidth);
		railingSeg[6] = vertex(cursor + localRight * railingWidth + u * railingThickness);
		railingSeg[7] = vertex(cursor + u  * railingThickness);
		createRailingSeg(railingSeg, false);

		cursor += dir * (midSize + stepSize * 0.5 - railingHOffset + railingWidth * 0.5);
		railingSeg[0] = vertex(cursor);
		railingSeg[1] = vertex(cursor - dir * railingWidth + localRight * railingWidth);
		railingSeg[2] = vertex(cursor - dir * railingWidth + localRight * railingWidth + u * railingThickness);
		railingSeg[3] = vertex(cursor + u * railingThickness);
		createRailingSeg(railingSeg, true);

		cursor += localRight * (railingWidth - railingHOffset + stairsWidth * 2 - railingHOffset);
		railingSeg[4] = vertex(cursor);
		railingSeg[5] = vertex(cursor - localRight * railingWidth - dir * railingWidth);
		railingSeg[6] = vertex(cursor - localRight * railingWidth - dir * railingWidth + u * railingThickness);
		railingSeg[7] = vertex(cursor + u * railingThickness);
		createRailingSeg(railingSeg, false);

		cursor += dir * (-railingWidth * 0.5 + railingHOffset - midSize + stepSize * 0.5);
		railingSeg[0] = vertex(cursor);
		railingSeg[1] = vertex(cursor - localRight * railingWidth);
		railingSeg[2] = vertex(cursor - localRight * railingWidth + u * railingThickness);
		railingSeg[3] = vertex(cursor + u * railingThickness);
		createRailingSeg(railingSeg, true);

		stepDisplacement.x *= -1;
		stepDisplacement.z *= -1;

		cursor += stepDisplacement * 0.5;
		railingSeg[4] = vertex(cursor);
		railingSeg[5] = vertex(cursor - localRight * railingWidth);
		railingSeg[6] = vertex(cursor - localRight * railingWidth + u * railingThickness);
		railingSeg[7] = vertex(cursor + u * railingThickness);
		createRailingSeg(railingSeg, false);

		dir = -dir;
		localRight = -localRight;
		railingSeg[0] = railingSeg[4];
		railingSeg[1] = railingSeg[5];
		railingSeg[2] = railingSeg[6];
		railingSeg[3] = railingSeg[7];
		//vertex(cursor);
	}

	cursor += stepDisplacement * (lastSegmentSteps);
	railingSeg[4] = vertex(cursor);
	railingSeg[5] = vertex(cursor + localRight * railingWidth);
	railingSeg[6] = vertex(cursor + localRight * railingWidth + u *railingThickness);
	railingSeg[7] = vertex(cursor + u *railingThickness);
	createRailingSeg(railingSeg, false);

	face(railingSeg, 4, 4, true); //cap
}
void createInnerRailing(vec3 origin, vec3 dir, int steps, float stairsWidth, float midSize, int stepsPerSegment)
{
	dir.y = 0;
	vec3 localRight = dir * u;
	dir = dir.normalized();
	localRight = localRight.normalized();

	int segments = (steps - 1) / stepsPerSegment;
	int lastSegmentSteps = (steps - 1) % stepsPerSegment + 1;
	cout << segments << ", " << lastSegmentSteps << endl;

	vec3 cursor = origin + u * (stepHeight * 0.5 + railingPipeHeight + railingVOffset) + localRight * (stairsWidth - railingHOffset - railingWidth * 0.5);

	int railingSeg[8];
	railingSeg[0] = vertex(cursor);
	railingSeg[1] = vertex(cursor + localRight * railingWidth);
	railingSeg[2] = vertex(cursor + localRight * railingWidth + u * railingThickness);
	railingSeg[3] = vertex(cursor + u  * railingThickness);
	face(railingSeg, 4);

	vec3 stepDisplacement = u * stepHeight + dir * stepSize;

	for (int i = 0; i < segments; i++)
	{
		cursor += stepDisplacement * (stepsPerSegment);
		railingSeg[4] = vertex(cursor);
		railingSeg[5] = vertex(cursor + localRight * railingWidth);
		railingSeg[6] = vertex(cursor + localRight * railingWidth + u * railingThickness);
		railingSeg[7] = vertex(cursor + u  * railingThickness);
		createRailingSeg(railingSeg, false);
		face(railingSeg, 4, 4, true);

		cursor += localRight * (railingHOffset * 2 + railingWidth);

		stepDisplacement.x *= -1;
		stepDisplacement.z *= -1;

		dir = -dir;
		localRight = -localRight;

		railingSeg[0] = vertex(cursor);
		railingSeg[1] = vertex(cursor + localRight * railingWidth);
		railingSeg[2] = vertex(cursor + localRight * railingWidth + u * railingThickness);
		railingSeg[3] = vertex(cursor + u  * railingThickness);
		face(railingSeg, 4);
		//vertex(cursor);
	}

	cursor += stepDisplacement * (lastSegmentSteps);
	railingSeg[4] = vertex(cursor);
	railingSeg[5] = vertex(cursor + localRight * railingWidth);
	railingSeg[6] = vertex(cursor + localRight * railingWidth + u *railingThickness);
	railingSeg[7] = vertex(cursor + u *railingThickness);
	createRailingSeg(railingSeg, false);

	face(railingSeg, 4, 4, true); //cap
}

void ushape(vec3 origin, vec3 dir, int steps, float stairsWidth, float midSize, int stepsPerSegment)
{
	createUshapeBasis(origin, dir, steps, stairsWidth, midSize, stepsPerSegment);

	if (createRailings)
	{
		placeUshapeRailingPipes(origin, dir, steps, stairsWidth, midSize, stepsPerSegment);
		createOuterRailing(origin, dir, steps, stairsWidth, midSize, stepsPerSegment);
		createInnerRailing(origin, dir, steps, stairsWidth, midSize, stepsPerSegment);
	}
}

int main()
{
	//float aStep = 2 * pi / 5;
	//for (int i = 0; i < 5; i++)
	//{
//		ushape(-b * sin(i * aStep) * 5 + r * cos(i * aStep) * 5, b * sin(i * aStep) - r * cos(i * aStep), 45, 1, 1, 15);
//	}
	//spiral(-b * 10, 60, 0.1, 0.2, 0.5, 1.5, );
	ushape(z, -b, 63, 1, 1, 15);

	//twistedRailingPipe(z, 40, 4, 0.5, 0.2, 0.02);

	closeFile();
}
