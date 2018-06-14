#include <math.h>
#include <GL/glut.h>

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
//#include <glm/glm.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/matrix_inverse.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "tdlib.h"

#define NEAR_CLIPPING_PLANE 1
#define FAR_CLIPPING_PLANE 100
#define FOV 40
#define ZOOM_SENSITIVITY 0.5
#define MAX_CAMERA_DISTANCE 50
#define MIN_CAMERA_DISTANCE 2
#define X_ROT_SENSITIVITY 0.01
#define Y_ROT_SENSITIVITY 0.01
#define _360_DEG 6.283185307
#define _90_DEG 1.57
using namespace std;

struct vertex
{
	string id;
	vec3 pos;
};
struct face
{
	vector<vertex> verts;
	vec3 normal;
};
std::ostream& operator<<(std::ostream& os, const vertex& v)
{
    return os << "id: " << v.id << ", pos: " << v.pos;
}

bool invertNormals = false;

// loaded from file
vector<face> faces;
vector<vertex> vertices;

// camera orbit
float cameraDistance = 11;
int lastScreenCoor[2]; //
bool orbit = false;
vec3 cameraTarget;
vec3 cameraPosition;
float theta = 0, phi = 0;

// centering camera
vec3 maxPos;
vec3 minPos;

//scene
GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};	/* White diffuse light. */
GLfloat light_position[] = {0.5, 0.5, 1, 0};
GLfloat ambient_light[] = {.8, .8, .8, 1};

void calculateCameraPosition()
{
	cameraPosition.x = cameraTarget.x + cameraDistance * sin(theta) * cos(phi);
	cameraPosition.y = cameraTarget.y + cameraDistance * sin(phi);
	cameraPosition.z = cameraTarget.z + cameraDistance * cos(theta) * cos(phi);
}

vertex findVertex(string theid)
{
	for (auto i : vertices)
	{
		if (i.id.compare(theid) == 0)
		{
			return i;
		}
	}
}

void drawModel()
{
	for (auto i : faces)
	{
		glBegin(GL_POLYGON);
			glNormal3f(i.normal.x, i.normal.y, i.normal.z);
			for (auto h : i.verts)
			{
				glVertex3f(h.pos.x, h.pos.y, h.pos.z);
			}
		glEnd();
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawModel();

	calculateCameraPosition();

	glLoadIdentity();
	gluLookAt(cameraPosition.x, cameraPosition.y, cameraPosition.z,
				cameraTarget.x, cameraTarget.y, cameraTarget.z,
				0, 1, 0);

	glutSwapBuffers();
}

void init()
{
	/* Enable a single OpenGL light. */
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);

	/* Set background color */
	glClearColor(.95, .95, .95, 1);

	/* Use depth buffering for hidden surface elimination. */
	glEnable(GL_DEPTH_TEST);

	/* Setup the view of the cube. */
	glMatrixMode(GL_PROJECTION);
	gluPerspective(
		/* field of view in degree */ FOV,
		/* aspect ratio */ 1.0,
		/* Z near */ NEAR_CLIPPING_PLANE,
		/* Z far */ FAR_CLIPPING_PLANE);

	glMatrixMode(GL_MODELVIEW);

	calculateCameraPosition();
	glLoadIdentity();
	gluLookAt(cameraPosition.x, cameraPosition.y, cameraPosition.z,
			cameraTarget.x, cameraTarget.y, cameraTarget.z,
			0, 1, 0);
}

void mouseMotion(int x, int y)
{
	if (orbit)
	{
		int dX = x - lastScreenCoor[0];
		int dY = y - lastScreenCoor[1];

		theta -= dX * X_ROT_SENSITIVITY;
		phi += dY * Y_ROT_SENSITIVITY;

		// clamp
		if (theta > _360_DEG)
			theta = 0;
		else if (theta < 0)
			theta = _360_DEG;
		if (phi > _90_DEG)
			phi = _90_DEG;
		else if (phi < -_90_DEG)
			phi = -_90_DEG;

		lastScreenCoor[0] = x;
		lastScreenCoor[1] = y;
		glutPostRedisplay();
	}
}
void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) 
	{ 
		if (state == GLUT_DOWN)
		{
			orbit = true;
			lastScreenCoor[0] = x;
			lastScreenCoor[1] = y;
		}
		else if (state == GLUT_UP)
		{
			orbit = false;
		}
	}
	else if (button == 3) // zoom in
	{
		cameraDistance -= ZOOM_SENSITIVITY;
		if (cameraDistance < MIN_CAMERA_DISTANCE)
			cameraDistance = MIN_CAMERA_DISTANCE;
		glutPostRedisplay();
	}
	else if (button == 4)
	{
		cameraDistance += ZOOM_SENSITIVITY; // zoom out
		if (cameraDistance > MAX_CAMERA_DISTANCE)
			cameraDistance = MAX_CAMERA_DISTANCE;
		glutPostRedisplay();
	}
}

void reshape(int width, int height)
{
		if (height == 0 || width == 0)
			return;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(FOV, (GLdouble)width/(GLdouble)height, NEAR_CLIPPING_PLANE, FAR_CLIPPING_PLANE);
		glMatrixMode(GL_MODELVIEW);
		glViewport(0, 0, width,height);
}

void readFile(char** argv)
{
	ifstream ifs (argv[1], ifstream::in);
	string str;
	while (getline(ifs, str))
	{
		if (str[0] == 'v')
		{
			vertex vtx;
			int i = 1;
			while (str[i] != '[')
				i++;

			vtx.id = str.substr(0, i);

			i++;
			int j = i + 1;
			int p = 0;
			while (str[j] != ']')
			{
				if (str[j] == ',')
				{
					if (p == 0)
						vtx.pos.x = stof(str.substr(i, j - i));
					else if (p == 1)
						vtx.pos.y = stof(str.substr(i, j - i));
					p++;
					i = j + 1;
				}
				j++;
			}
			vtx.pos.z = stof(str.substr(i, j - i));

			vertices.push_back(vtx);

			// centering view
			if (vtx.pos.x > maxPos.x)
				maxPos.x = vtx.pos.x;
			if (vtx.pos.y > maxPos.y)
				maxPos.y = vtx.pos.y;
			if (vtx.pos.z > maxPos.z)
				maxPos.z = vtx.pos.z;

			if (vtx.pos.x < minPos.x)
				minPos.x = vtx.pos.x;
			if (vtx.pos.y < minPos.y)
				minPos.y = vtx.pos.y;
			if (vtx.pos.z < minPos.z)
				minPos.z = vtx.pos.z;
		}
		else if (str[0] == 'f')
		{
			face theFace;
			int i = 1;
			while (str[i] != '[')
				i++;
			i++;
			int j = i;
			while (str[j] != ']')
			{
				if (str[j] == ',')
				{
					theFace.verts.push_back(findVertex(str.substr(i, j - i)));
					i = j + 1;
				}
				j++;
			}
			theFace.verts.push_back(findVertex(str.substr(i)));

			//calculate normal
			vec3 a = theFace.verts[1].pos - theFace.verts[0].pos;
			vec3 b = theFace.verts[2].pos - theFace.verts[1].pos;
			if (invertNormals)
			{
				theFace.normal = b * a; // cross prod
			}
			else
			{
				theFace.normal = a * b; // cross prod
			}
			theFace.normal = theFace.normal.normalized();
			faces.push_back(theFace);
		}
	}
}

int main(int argc, char** argv)
{
	minPos.x = minPos.y = minPos.z = 0;
	maxPos.x = maxPos.y = maxPos.z = 0;

	if (argc == 1)
	{
		cout << "Invalid arguments" << endl;
		return -1;
	}
	if (argc > 2)
	{
		invertNormals = true;
	}

	readFile(argv);

	// calculate camera target position
	cameraTarget = (maxPos + minPos) * 0.5;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glEnable(GL_MULTISAMPLE);

	glutInitWindowSize(600, 600);
	glutCreateWindow("wawawawa");
	glutDisplayFunc(display);

	glutMotionFunc(mouseMotion);
	glutMouseFunc(mouse);

	glutReshapeFunc(reshape);

	init();
	glutMainLoop();

	return 0;
}