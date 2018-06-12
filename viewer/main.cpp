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
std::ostream& operator<<(std::ostream& os, const vertex& v)
{
    return os << "id: " << v.id << ", pos: " << v.pos;
}

vector<string> faces;
vector<vertex> vertices;

// camera orbit
float cameraDistance = 11;
int lastScreenCoor[2]; //
bool orbit = false;
float cameraTarget[3] = {0.0, 0.0, 0.0};
float cameraPosition[3];
float theta = 0, phi = 0;


//glm::mat4 projection;

GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};	/* White diffuse light. */
GLfloat light_position[] = {0.5, 0.5, 1, 0};
GLfloat ambient_light[] = {.8, .8, .8, 1};

void calculateCameraPosition()
{
	cameraPosition[0] = cameraTarget[0] + cameraDistance * sin(theta) * cos(phi);
	cameraPosition[1] = cameraTarget[1] + cameraDistance * sin(phi);
	cameraPosition[2] = cameraTarget[2] + cameraDistance * cos(theta) * cos(phi);
}

/*
void drawBox()
{
	for (int i = 0; i < 6; i++) // for each face
	{
		glColor3f(0.9f, 0.9f, 0.9f);
		glBegin(GL_QUADS);
		glNormal3fv(&n[i][0]);
		glVertex3fv(&v[faces[i][0]][0]);
		glVertex3fv(&v[faces[i][1]][0]);
		glVertex3fv(&v[faces[i][2]][0]);
		glVertex3fv(&v[faces[i][3]][0]);
		glEnd();
	}
}*/

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
		vector<vertex> faceVerts;
		int a = 0;
		for (int j = 0; j < i.length(); j++)
		{
			if (i[j] == ',')
			{
				faceVerts.push_back(findVertex(i.substr(a, j - a)));
				a = j + 1;
			}
		}
		faceVerts.push_back(findVertex(i.substr(a)));

		vec3 oneVector = faceVerts[1].pos - faceVerts[0].pos;
		vec3 anotherVector = faceVerts[2].pos - faceVerts[1].pos;
		vec3 normal = oneVector * anotherVector; //cross
		normal = normal.normalized();

		//cout << "polygon\n";
		glBegin(GL_POLYGON);
			glNormal3f(normal.x, normal.y, normal.z);
			for (auto h : faceVerts)
			{
				//cout << h << endl;
				glVertex3f(h.pos.x, h.pos.y, h.pos.z);
			}
		glEnd();
	}

	/*
	glBegin(GL_POLYGON);
		glNormal3f(0, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 1, 0);
		glVertex3f(0.5, 1, 0);
		glVertex3f(1, 0.5, 0);
		glVertex3f(1, 0, 0);
	glEnd();
	glBegin(GL_POLYGON);
		glNormal3f(0, 0, 1);
		glVertex3f(0, 0, 1);
		glVertex3f(0, 1, 1);
		glVertex3f(0.5, 1, 0);
		glVertex3f(1, 0.5, 0);
		glVertex3f(1, 0, 1);
	glEnd();
	*/
}

/*void drawSnowMan() {

	glColor3f(1.0f, 1.0f, 1.0f);

	glTranslatef(0.0f ,0.75f, 0.0f);
	glutSolidSphere(0.75f,20,20);

	glTranslatef(0.0f, 1.0f, 0.0f);
	glutSolidSphere(0.25f,20,20);

	glPushMatrix();
	glColor3f(0.0f,0.0f,0.0f);
	glTranslatef(0.05f, 0.10f, 0.18f);
	glutSolidSphere(0.05f,10,10);
	glTranslatef(-0.1f, 0.0f, 0.0f);
	glutSolidSphere(0.05f,10,10);
	glPopMatrix();

	glColor3f(1.0f, 0.5f , 0.5f);
	glutSolidCone(0.08f,0.5f,10,2);
}*/

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawModel();
	/*for(int i = -5; i < 5; i++)
	{
		for(int j=-5; j < 5; j++)
		{
			glPushMatrix();
			glTranslatef(i*10.0,-1,j * 10.0);
			drawSnowMan();
			glPopMatrix();
		}
	}*/

	calculateCameraPosition();
	//glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cameraPosition[0], cameraPosition[1], cameraPosition[2],
				cameraTarget[0], cameraTarget[1], cameraTarget[2],
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
	gluLookAt(cameraPosition[0], cameraPosition[1], cameraPosition[2],
						cameraTarget[0], cameraTarget[1], cameraTarget[2],
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
		}
		else if (str[0] == 'f')
		{
			int i = 1;
			while (str[i] != '[')
				i++;
			i++;
			int j = i + 1;
			while (str[j] != ']')
				j++;
			faces.push_back(str.substr(i, j - i));
		}
	}
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		cout << "Invalid arguments" << endl;
		return -1;
	}

	readFile(argv);

	/*
	cout << "vertices:\n";
	for (auto i : vertices)
	{
		cout << i << endl;
	}
	cout << "\nfaces:\n";
	for (auto i : faces)
	{
		cout << i << endl;
	}*/

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