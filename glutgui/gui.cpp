 
#include <GL/gl.h>
//#include <GL/freeglut.h>
#include <GL/glut.h>   // freeglut.h might be a better alternative, if available.
#include <string>
#include <vector>
#include <iostream>

#define PM_WINDOW_MARGIN_V 10
#define PM_WINDOW_MARGIN_RIGHT 80
#define PM_WINDOW_MARGIN_LEFT 20
#define PM_WINDOW_ROW_HEIGHT 40

float fVar;
int iVar;
bool bVar17;
bool bVar16;
bool bVar15;
bool bVar14;
bool bVar13;
bool bVar12;
bool bVar11;
bool bVar10;
bool bVar9;
bool bVar8;
bool bVar7;
bool bVar6;
bool bVar5;
bool bVar4;
bool bVar3;
bool bVar2;
bool bVar1;
bool bVar0;

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
				//bool* pointer = (bool*) parametersVector[index].variable;
				//*pointer = !*pointer;
				glutPostRedisplay();
				//std::cout << parametersVector[index].name << " = " << *pointer << std::endl;
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

int main( int argc, char** argv )
{
	parametersVector.push_back(parameter("fVar", 'f', &fVar, 0.0001, -1, 1));
	parametersVector.push_back(parameter("iVar", 'i', &iVar, 0.1, 0, 0));
	parametersVector.push_back(parameter("bVar", 'b', &bVar17, 0.1, 0, 0));
	parametersVector.push_back(parameter("bVar", 'b', &bVar16, 0.1, 0, 0));
	parametersVector.push_back(parameter("bVar", 'b', &bVar15, 0.1, 0, 0));
	parametersVector.push_back(parameter("bVar", 'b', &bVar14, 0.1, 0, 0));
	parametersVector.push_back(parameter("bVar", 'b', &bVar13, 0.1, 0, 0));
	parametersVector.push_back(parameter("bVar", 'b', &bVar12, 0.1, 0, 0));
	parametersVector.push_back(parameter("bVar", 'b', &bVar11, 0.1, 0, 0));
	parametersVector.push_back(parameter("bVar", 'b', &bVar10, 0.1, 0, 0));
	parametersVector.push_back(parameter("bVar", 'b', &bVar9, 0.1, 0, 0));
	parametersVector.push_back(parameter("bVar", 'b', &bVar8, 0.1, 0, 0));
	parametersVector.push_back(parameter("bVar", 'b', &bVar7, 0.1, 0, 0));
	parametersVector.push_back(parameter("bVar", 'b', &bVar6, 0.1, 0, 0));
	parametersVector.push_back(parameter("bVar", 'b', &bVar5, 0.1, 0, 0));
	parametersVector.push_back(parameter("bVar", 'b', &bVar4, 0.1, 0, 0));
	parametersVector.push_back(parameter("bVar", 'b', &bVar3, 0.1, 0, 0));
	parametersVector.push_back(parameter("bVar", 'b', &bVar2, 0.1, 0, 0));
	parametersVector.push_back(parameter("bVar", 'b', &bVar1, 0.1, 0, 0));
	parametersVector.push_back(parameter("bVar", 'b', &bVar0, 0.1, 0, 0));

	GetParameterWindowSize(parameterWindowSize);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);    // Use single color buffer and no depth buffer.
    glutInitWindowSize(parameterWindowSize[0], parameterWindowSize[1]);         // Size of display area, in pixels.
    glutInitWindowPosition(100, 100);     // Location of window in screen coordinates.
    glutCreateWindow("parameters"); // Parameter is window title.
    glutDisplayFunc(parameterWindowDisplay);            // Called when the window needs to be redrawn.
    
	glutMouseFunc(parameterWindowMouse);
	glutMotionFunc(parameterWindowMouseMotion);
    glutMainLoop(); // Run the event loop!  This function does not return.
                    // Program ends when user closes the window.
    return 0;

}