#include <iostream>
#include "pmlib/pmlib.h"
using namespace std;

int main()
{
	float innerRadius = .5;
	float outerRadius = 4;
	int steps = 20;
	float stepHeight = 0.2;
	float thickness = 0.3;
	float stepAngle = 2 * pi / 50; //360 / steps
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
	
	closeFile();
}
