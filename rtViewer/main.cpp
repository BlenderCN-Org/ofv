#include <GL/glut.h>

#include <iostream>
#include <string>
#include <fstream>

#include "cppmodel.h"

struct parameter
{
	string name;
	char type;
	void* variable;
};

int currentParameter = 0;
std::vector<parameter> parameters;

#include "binder.h"

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

// camera orbit
float cameraDistance = 11;
int lastScreenCoor[2]; //
bool orbit = false;
vec3 cameraTarget;
vec3 cameraPosition;
float theta = 0, phi = 0;


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

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	clearModel();

	maxCameraTargetPos = minCameraTargetPos = z;
	generateModel(); // in cppmodel.h
	cameraTarget = (maxCameraTargetPos + minCameraTargetPos) * 0.5;

	//drawModel();

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

// parameter handling
void specialFunc(int key, int x, int y)
{
	switch(key)
	{
		case GLUT_KEY_UP:
			if (parameters[currentParameter].type == 'f')
			{
				float* p = (float*) parameters[currentParameter].variable;
				*p += 0.1;
			}
			else
			{
				int* p = (int*) parameters[currentParameter].variable;
				*p += 1;
			}
			break;	
		case GLUT_KEY_DOWN:
			if (parameters[currentParameter].type == 'f')
			{
				float* p = (float*) parameters[currentParameter].variable;
				*p -= 0.1;
			}
			else
			{
				int* p = (int*) parameters[currentParameter].variable;
				*p -= 1;
			}
			break;
		case GLUT_KEY_LEFT:
			currentParameter--;
			if (currentParameter == -1)
			{
				currentParameter = parameters.size() - 1;
			}
			break;
		case GLUT_KEY_RIGHT:
			currentParameter++;
			if (currentParameter == parameters.size())
			{
				currentParameter = 0;
			}
			break;
	}

	glutPostRedisplay();
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

int main(int argc, char** argv)
{
	bindParameters();

	minCameraTargetPos.x = minCameraTargetPos.y = minCameraTargetPos.z = 0;
	maxCameraTargetPos.x = maxCameraTargetPos.y = maxCameraTargetPos.z = 0;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glEnable(GL_MULTISAMPLE);
	glutSetSamples(16);

	glutInitWindowSize(600, 600);
	glutCreateWindow("wawawa");
	glutDisplayFunc(display);

	glutMotionFunc(mouseMotion);
	glutMouseFunc(mouse);
	glutSpecialFunc(specialFunc);

	glutReshapeFunc(reshape);

	init();	
	glutMainLoop();

	return 0;
}