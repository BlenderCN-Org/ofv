#include <GL/glut.h>

#include <iostream>
#include <string>
#include <fstream>

#include "cppmodel.h"

int viewport_window, parameter_window;

#define PM_WINDOW_MARGIN_V 10
#define PM_WINDOW_MARGIN_RIGHT 80
#define PM_WINDOW_MARGIN_LEFT 20
#define PM_WINDOW_ROW_HEIGHT 40

struct parameter
{
	std::string name;
	char type;
	void* variable;
	float helper;
	float sensitivity;
	float max;
	float min;

	parameter(std::string name, char type, void* variable, float sensitivity, float min, float max)
	{
		this->name = name;
		this->type = type;
		this->variable = variable;
		this->sensitivity = sensitivity;
		this->max = max;
		this->min = min;

		if (type == 'i')
			this->helper = (float) *((int*) variable);
	}

	void modify(int dX) // delta X in pixels
	{
		if (this->type == 'f')
		{
			float* pointer = (float*) this->variable;
			*pointer += dX * this->sensitivity;

			// clamp
			if (this->min != this->max)
			{
				if (*pointer > this->max)
					*pointer = this->max;
				if (*pointer < this->min)
					*pointer = this->min;
			}
		}
		else if (this->type == 'i')
		{
			this->helper += dX * this->sensitivity;

			// clamp
			if (this->min != this->max)
			{
				if (this->helper > this->max)
					this->helper = this->max;
				if (this->helper < this->min)
					this->helper = this->min;
			}

			int* pointer = (int*) this->variable;
			*pointer = (int) this->helper;
		}
		else
		{
			bool* asdf = (bool*) this->variable;
			*asdf = !*asdf;
		}
	}
};
std::vector<parameter> parametersVector;
bool manipulatingParameter = false;
int paramWindowManipulatingIndex;
int parameterWindowLastPosX;

int parameterWindowSize[2];

void parameterWindowDrawText(const char* text, int length, int x, int y)
{
	glMatrixMode(GL_PROJECTION);
	double* matrix = new double[16];
	glGetDoublev(GL_PROJECTION_MATRIX, matrix);
	glLoadIdentity();
	glOrtho(0, parameterWindowSize[0], 0, parameterWindowSize[1], -5, 5);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glLoadIdentity();
	glRasterPos2i(x, y);
	for (int i = 0; i < length; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text[i]);
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(matrix);
	glMatrixMode(GL_MODELVIEW);
}

void parameterWindowDisplay()
{ 
    glClearColor( 0.3, 0.3, 0.3, 1 );  // (In fact, this is the default.)
    glClear( GL_COLOR_BUFFER_BIT );

    int currentY = parameterWindowSize[1] - PM_WINDOW_MARGIN_V - PM_WINDOW_ROW_HEIGHT * 0.3;

    for (auto i : parametersVector)
    {
    	parameterWindowDrawText(i.name.c_str(), i.name.length(), PM_WINDOW_MARGIN_LEFT, currentY);
    	if (i.type == 'f')
    	{
    		float* var = (float*) i.variable;
    		std::string s = std::to_string(*var);
	    	parameterWindowDrawText(s.c_str(), s.length(), PM_WINDOW_MARGIN_LEFT, currentY - PM_WINDOW_ROW_HEIGHT * 0.4);
    	}
    	else if (i.type == 'i')
    	{
    		int* var = (int*) i.variable;
    		std::string s = std::to_string(*var);
	    	parameterWindowDrawText(s.c_str(), s.length(), PM_WINDOW_MARGIN_LEFT, currentY - PM_WINDOW_ROW_HEIGHT * 0.4);
    	}
    	else
    	{
    		bool* var = (bool*) i.variable;
	    	parameterWindowDrawText(*var ? "true" : "false", *var ? 4 : 5, PM_WINDOW_MARGIN_LEFT, currentY - PM_WINDOW_ROW_HEIGHT * 0.4);
    	}

    //std::cout << currentY << std::endl;
    	currentY -= PM_WINDOW_ROW_HEIGHT;
    }
    
    glutSwapBuffers(); // Required to copy color buffer onto the screen.
 
}
void parameterWindowMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) 
	{ 
		if (state == GLUT_DOWN)
		{
			//std::cout << x << ", " << y << std::endl;

			y -= PM_WINDOW_MARGIN_V; // margin

			y = (y < 0 ? 0 : y);
			int index = y / PM_WINDOW_ROW_HEIGHT;
			index = (index >= parametersVector.size() ? parametersVector.size() - 1 : index);

			//std::cout << parametersVector[index].name << " pressed" << std::endl;

			if (parametersVector[index].type == 'b')
			{
				parametersVector[index].modify(0);

				glutPostRedisplay();
				glutSetWindow(viewport_window);
				glutPostRedisplay();
			}
			else // float and int
			{
				manipulatingParameter = true;
				paramWindowManipulatingIndex = index;
				parameterWindowLastPosX = x;
			}
		}
		else if (state == GLUT_UP)
		{
			manipulatingParameter = false;
		}
	}
}

void parameterWindowMouseMotion(int x, int y)
{
	if (!manipulatingParameter)
		return;

	int dX = x - parameterWindowLastPosX;

	if (parametersVector[paramWindowManipulatingIndex].type != 'b')
	{
		parametersVector[paramWindowManipulatingIndex].modify(dX);
		glutPostRedisplay();
		glutSetWindow(viewport_window);
		glutPostRedisplay();
	}

	/*if (parametersVector)
	{
		float* pointer = (float*) parametersVector[paramWindowManipulatingIndex].variable;
		*pointer += dX * parametersVector[paramWindowManipulatingIndex].sensitivity;
		glutPostRedisplay();
	}
	else if (parametersVector[paramWindowManipulatingIndex].type == 'i')
	{
		parametersVector[paramWindowManipulatingIndex].helper += dX * parametersVector[paramWindowManipulatingIndex].sensitivity;
		int* pointer = (int*) parametersVector[paramWindowManipulatingIndex].variable;
		*pointer = (int) parametersVector[paramWindowManipulatingIndex].helper;
		glutPostRedisplay();
	}*/

	parameterWindowLastPosX = x;
}

void GetParameterWindowSize(int* size)
{
	size[1] = parametersVector.size() * PM_WINDOW_ROW_HEIGHT + PM_WINDOW_MARGIN_V * 2;
	size[0] = 3;

	for (auto i : parametersVector)
	{
		if (i.name.length() > size[0])
			size[0] = i.name.length();
	}

	size[0] *= 9;
	size[0] += PM_WINDOW_MARGIN_RIGHT + PM_WINDOW_MARGIN_LEFT;

	std::cout << size[0] << ", " << size[1] << std::endl;
}

#include "binder.h"

#define CENTER_CAMERA 1
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
	if (CENTER_CAMERA)
		cameraTarget = (maxCameraTargetPos + minCameraTargetPos) * 0.5;

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

int main(int argc, char** argv)
{
	bindParameters();
	GetParameterWindowSize(parameterWindowSize);

	minCameraTargetPos.x = minCameraTargetPos.y = minCameraTargetPos.z = 0;
	maxCameraTargetPos.x = maxCameraTargetPos.y = maxCameraTargetPos.z = 0;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glEnable(GL_MULTISAMPLE);

	glutInitWindowSize(600, 600);
	viewport_window = glutCreateWindow("viewport");
	glutDisplayFunc(display);

	glutMotionFunc(mouseMotion);
	glutMouseFunc(mouse);

	glutReshapeFunc(reshape);

	init();

	glutInitDisplayMode(GLUT_SINGLE);    // Use single color buffer and no depth buffer.
    glutInitWindowSize(parameterWindowSize[0], parameterWindowSize[1]);         // Size of display area, in pixels.
    glutInitWindowPosition(100, 100);     // Location of window in screen coordinates.
    parameter_window = glutCreateWindow("parameters"); // Parameter is window title.
    glutDisplayFunc(parameterWindowDisplay);            // Called when the window needs to be redrawn.
    
	glutMouseFunc(parameterWindowMouse);
	glutMotionFunc(parameterWindowMouseMotion);

	glutMainLoop();

	return 0;
}