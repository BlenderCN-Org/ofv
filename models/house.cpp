#include "rtpmlib.h"

int seed;
float minSideSize = 20;
float maxSideSize = 25;

float minRoomSize = 4;
float maxRoomSize = 8;

float wallThickness = 0.1;
float wallHeight = 2.4;

float doorWidth = 0.7;
float doorHeight = 2.1;

float rdm()
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

float rdm(float min, float max)
{
	return rdm() * (max - min) + min;
}

void wall(vec3 a, vec3 d)
{
	vec3 localRight = (d - a) * u;
	localRight = localRight.normalized();
	int vertices[8];
	vertices[0] = vertex(a + localRight * wallThickness * 0.5);
	vertices[1] = vertex(d + localRight * wallThickness * 0.5);
	vertices[2] = vertex(d + localRight * wallThickness * 0.5 + u * wallHeight);
	vertices[3] = vertex(a + localRight * wallThickness * 0.5 + u * wallHeight);
	vertices[4] = vertex(a - localRight * wallThickness * 0.5);
	vertices[5] = vertex(d - localRight * wallThickness * 0.5);
	vertices[6] = vertex(d - localRight * wallThickness * 0.5 + u * wallHeight);
	vertices[7] = vertex(a - localRight * wallThickness * 0.5 + u * wallHeight);
	face(vertices, 4, 0, false);
	face(vertices, 4, 4, true);
}

void generateModel()
{
	srand(seed);

	vec3 cursor = z;
	int vertices[4];
	vertices[0] = vertex(cursor);
	cursor.x += rdm(minSideSize, maxSideSize);
	vertices[1] = vertex(cursor);
	cursor.z -= rdm(minSideSize, maxSideSize);
	vertices[2] = vertex(cursor);
	cursor.x = 0;
	vertices[3] = vertex(cursor);

	face(vertices, 4);

	vec3 frc = z + r * rdm(minRoomSize, maxRoomSize) - b * rdm(minRoomSize, maxRoomSize);
	vec3 frcd = frc + b * doorWidth;
	vec3 zrc = frc;
	zrc.z = 0;
	vec3 zfc = frc;
	zfc.x = 0;
	wall(z, zrc);
	wall(zrc, frcd);
	wall(frc, zfc);
	wall(z, zfc);
}