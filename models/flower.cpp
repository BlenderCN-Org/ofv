#include "rtpmlib.h"
#define DEG2RAD 0.017453292

int petals = 8;
float petalWidth = 0.5;
float petalLength = 1;
float petalHeight = .25;
int petalResZ = 6;
int petalResX = 5;
float radius = 0.1;

int ballResH = 12;
int ballResV = 3;
float ballHeight = 0.1;
float ballOffset = -0.025;

float ballMaxRadiusMultiplier = 2;
float ballMinRadiusMultiplier = 1.5;
float minPetalAngle = -7;
float maxPetalAngle = 20;

float rdm()
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}
float rdm(float min, float max)
{
	return rdm() * (max - min) + min;
}

void petal(vec3 origin, vec3 dir, vec3 localUp, float width, float length, float height, int zRes, int xRes)
{
	vec3 localRight = dir * localUp;
	if (localRight == z)
		localRight = r;
	vec3 localForward = dir.normalized();
	localRight = localRight.normalized();
	localUp = localUp.normalized();

	float fZ = 0;
	float zStep = 1.0 / zRes;
	float fX = -0.5;
	float xStep = 1.0 / xRes;

	int vertices[zRes][xRes];
	int quad[4];

	for (int i = 0; i < zRes; i++)
	{
		fX = -0.5;
		for (int j = 0; j < xRes; j++)
		{
			vec3 localPos(localRight * (fX * (-pow(fZ * 1.1 - 0.4, 2) + 0.5) * 2) * width + 
									localUp * (-pow(fX, 2) + pow(abs(fX), 2.5) + pow(fZ, 2) + sin(fZ * 20) / 20) * height - localUp +
									localForward * (fZ) * length);

			vertices[i][j] = vertex(localPos + origin + localUp);

			if (j > 0 && i > 0)
			{
				quad[0] = vertices[i-1][j];
				quad[1] = vertices[i][j];
				quad[2] = vertices[i][j-1];
				quad[3] = vertices[i-1][j-1];
				face(quad, 4);
			}

			fX += xStep;
		}
		fZ += zStep;
	}
}

void flower(vec3 origin, vec3 localUp, int petals)
{
	float angleStep = 2 * pi / petals;
	float theta = 0;

	vec3 localRight = localUp * u;
	if (localRight == z)
		localRight = r;
	vec3 localBack = localRight * localUp;
	localUp = localUp.normalized();
	localRight = localRight.normalized();
	localBack = localBack.normalized();

	for (int i = 0; i < petals; i++)
	{
		vec3 dir = localRight * cos(theta) - localBack * sin(theta);	
		float angle = rdm(minPetalAngle, maxPetalAngle);
		petal(origin + dir * radius, dir * cos(angle*DEG2RAD) + localUp * sin(angle*DEG2RAD), localUp, petalWidth, petalLength, petalHeight, petalResZ, petalResX);
		theta += angleStep;
	}

	//ball
	float ballRadiusMultiplier = rdm(ballMinRadiusMultiplier, ballMaxRadiusMultiplier);
	theta = 0;
	float phi = 0;
	float thStep = 2 * pi / ballResH;
	float phStep = pi / ballResV / 2;

	int topVert = vertex(origin + localUp * (ballHeight + ballOffset));

	int vertices[ballResH][ballResV];
	int cQuad[4];
	int tri[3];

	for (int i = 0; i < ballResH; i++)
	{
		phi = 0;
		for (int j = 0; j < ballResV; j++)
		{
			vertices[i][j] = vertex(origin + localRight * cos(theta) * cos(phi) * radius * ballRadiusMultiplier +
											localBack * sin(theta) * cos(phi) * radius * ballRadiusMultiplier +
											localUp * (sin(phi) * ballHeight + ballOffset));
			if (i > 0 && j > 0)
			{
				cQuad[0] = vertices[i - 1][j - 1];
				cQuad[1] = vertices[i][j - 1];
				cQuad[2] = vertices[i][j];
				cQuad[3] = vertices[i - 1][j];
				face(cQuad, 4, true);
			}

			phi += phStep;
		}
		if (i > 0)
		{
			tri[0] = vertices[i - 1][ballResV - 1];
			tri[1] = vertices[i][ballResV - 1];
			tri[2] = topVert;
			face(tri, 3, true);
		}
		theta += thStep;
	}
	for (int j = 1; j < ballResV; j++)
	{
		cQuad[0] = vertices[0][j - 1];
		cQuad[1] = vertices[ballResH - 1][j - 1];
		cQuad[2] = vertices[ballResH - 1][j];
		cQuad[3] = vertices[0][j];
		face(cQuad, 4);
	}
	tri[0] = vertices[0][ballResV - 1];
	tri[1] = vertices[ballResH - 1][ballResV - 1];
	tri[2] = topVert;
	face(tri, 3);
}

void stem(vec3 origin, vec3 direction, float stepLength, float radius, int sides, float dirMultiplier, int steps)
{
	direction = direction.normalized();

	vec3 current = u;
	vec3 currentOrigin = origin;

	int vertices[steps][sides];
	int cQuad[4];

	for (int i = 0; i < steps; i++)
	{
		vec3 localRight = current * (r * u);
		if (localRight == z)
			localRight = r;
		vec3 localBack = localRight * current;
		localRight = localRight.normalized();
		localBack = localBack.normalized();

		float theta = 0;
		float thStep = 2 * pi / sides;
		for (int j = 0; j < sides; j++)
		{
			vertices[i][j] = vertex(currentOrigin + localRight * cos(theta) * radius +
													localBack * sin(theta) * radius);

			if (j > 0 && i > 0)
			{
				cQuad[0] = vertices[i-1][j-1];
				cQuad[1] = vertices[i-1][j];
				cQuad[2] = vertices[i][j];
				cQuad[3] = vertices[i][j-1];
				face(cQuad, 4);
			}

			theta -= thStep;
		}
		if (i > 0)
		{
			cQuad[0] = vertices[i-1][sides - 1];
			cQuad[1] = vertices[i-1][0];
			cQuad[2] = vertices[i][0];
			cQuad[3] = vertices[i][sides - 1];
			face(cQuad, 4);
		}

		currentOrigin += current * stepLength;
		current = u + (direction * dirMultiplier * (i/2.0));
		current = current.normalized();
	}
	flower(currentOrigin - current*stepLength, current, petals);
}

void generateModel()
{
	srand(time(0));

	float size = 15;

	for (int i = 0; i < 30; i++)
	{
		vec3 origin(rdm() * size - size / 2,
					0,
					rdm() * size - size / 2);

		vec3 dir(rdm(-1, 1),
				rdm(0.5, 1),
				rdm(-1, 1));


		stem(origin, dir, rdm(0.3, 0.4), rdm(0.04, 0.06), 6, rdm(0.2, 0.4), rand() % 15 + 10);
	}


	//closeFile();
	//flower(b + u*5, 15, 15, petals);
}