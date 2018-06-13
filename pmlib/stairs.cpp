#include <iostream>
#include "pmlib.h"
using namespace std;

int main()
{
	float innerRadius = .5;
	float outerRadius = 2;
	int steps = 15;
	float stepHeight = 0.2;

	float stepAngle = 2 * pi / 12; //360 / steps
	float currentAngle = 0;

	int vertices[4]; //quads

	vec3 pos(cos(currentAngle) * innerRadius,
			0,
			sin(currentAngle) * innerRadius);

	vertices[0] = vertex(pos);
	pos.x = cos(currentAngle) * outerRadius;
	pos.z = sin(currentAngle) * outerRadius;
	vertices[1] = vertex(pos);

	for (int i = 0; i < steps; i++)
	{
		pos += u * stepHeight;
		vertices[2] = vertex(pos);
		pos.x = cos(currentAngle) * innerRadius;
		pos.z = sin(currentAngle) * innerRadius;
		vertices[3] = vertex(pos);
		face(vertices, 4);

		currentAngle += stepAngle;
		vertices[0] = vertices[3];
		vertices[1] = vertices[2];
		pos.x = cos(currentAngle) * outerRadius;
		pos.z = sin(currentAngle) * outerRadius;
		vertices[2] = vertex(pos);
		pos.x = cos(currentAngle) * innerRadius;
		pos.z = sin(currentAngle) * innerRadius;
		vertices[3] = vertex(pos);
		face(vertices, 4);

		vertices[0] = vertices[3];
		vertices[1] = vertices[2];
		pos.x = cos(currentAngle) * outerRadius;
		pos.z = sin(currentAngle) * outerRadius;
	}
	
	//vertices[1] = vertices[2];
	//vertices[2] = last;
	//face(vertices, 3);
	
	closeFile();
}
