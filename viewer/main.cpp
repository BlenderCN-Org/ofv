#include <GL/glut.h>

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
//#include <glm/glm.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/matrix_inverse.hpp>

#include <iostream>
#include "tdlib.h"

#define rad2deg 57.29577951
#define deg2rad 0.017453292

#define CAMERA_DISTANCE 11
#define NEAR_CLIPPING_PLANE 1
#define FAR_CLIPPING_PLANE 50
#define FOV 40
#define X_ROT_SENSITIVITY 0.3
#define Y_ROT_SENSITIVITY 0.1
using namespace std;

// camera orbit
int lastCoor[2];

vector cameraPos;
float cameraAngle;
bool orbit = false;


//glm::mat4 projection;

//GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};  /* White diffuse light. */
GLfloat light_position[] = {2, 2, 2, 0.0};
GLfloat n[6][3] = {  /* Normals for the 6 faces of a cube. */
  {-1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 0.0, 0.0},
  {0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0} };
GLint faces[6][4] = {  /* Vertex indices for the 6 faces of a cube. */
  {0, 1, 2, 3}, {3, 2, 6, 7}, {7, 6, 5, 4},
  {4, 5, 1, 0}, {5, 6, 2, 1}, {7, 4, 0, 3} };
GLfloat v[8][3];  /* Will be filled in with X,Y,Z vertexes. */


void calculateCameraPosition()
{
  cameraPos.y = sin(cameraAngle) * CAMERA_DISTANCE;
  cameraPos.z = 0;
  cameraPos.x = cos(cameraAngle) * CAMERA_DISTANCE;
}

void drawBox()
{
  for (int i = 0; i < 6; i++) // for each face
  {
    glBegin(GL_QUADS);
    glNormal3fv(&n[i][0]);
    glVertex3fv(&v[faces[i][0]][0]);
    glVertex3fv(&v[faces[i][1]][0]);
    glVertex3fv(&v[faces[i][2]][0]);
    glVertex3fv(&v[faces[i][3]][0]);
    glEnd();
  }
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  drawBox();
  glutSwapBuffers();
}

void init()
{
  /* Setup cube vertex data. */
  v[0][0] = v[1][0] = v[2][0] = v[3][0] = -1;
  v[4][0] = v[5][0] = v[6][0] = v[7][0] = 1;
  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -1;
  v[2][1] = v[3][1] = v[6][1] = v[7][1] = 1;
  v[0][2] = v[3][2] = v[4][2] = v[7][2] = 1;
  v[1][2] = v[2][2] = v[5][2] = v[6][2] = -1;

  /* Enable a single OpenGL light. */
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);

  //glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

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

  cameraAngle = 30 * deg2rad;
  calculateCameraPosition();
  gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z,  /* eye is at (0,0,5) */
            0.0, 0.0, 0.0,      /* center is at (0,0,0) */
            0.0, 1.0, 0.);      /* up is in positive Y direction */

  /* Adjust cube position to be asthetic angle. */
  //glTranslatef(0.0, 0.0, -1.0);
  //glRotatef(60, 1.0, 0.0, 0.0);
  //glRotatef(-20, 0.0, 0.0, 1.0);
}

void mouseMotion(int x, int y)
{
  if (orbit)
  {
    int dX = x - lastCoor[0];
    int dY = y - lastCoor[1];

    glMatrixMode(GL_MODELVIEW);
    glRotatef(dX * X_ROT_SENSITIVITY, 0, 1, 0);
    glMatrixMode(GL_PROJECTION);


    glMatrixMode(GL_PROJECTION);
    glRotatef(dY * Y_ROT_SENSITIVITY, 1, 0, 0);
    glTranslatef(0, -dY * Y_ROT_SENSITIVITY * 0.2, 0);

    /*cameraAngle += dY * Y_ROT_SENSITIVITY;
    calculateCameraPosition();
    gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z,
            0.0, 0.0, 0.0,
            0.0, 1.0, 0.);*/

    glutPostRedisplay();
    lastCoor[0] = x;
    lastCoor[1] = y;
  }
}
void mouse(int button, int state, int x, int y)
{
  if (button == GLUT_MIDDLE_BUTTON) 
  { 
    if (state == GLUT_DOWN)
    {
      orbit = true;
      lastCoor[0] = x;
      lastCoor[1] = y;
    }
    else if (state == GLUT_UP)
    {
      orbit = false;
    }
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
    glViewport(0,0,width,height);/*
  glViewport(0, 0, width, height);
  GLfloat aspectRatio = GLfloat(width)/height;
  projection = glm::perspective(45.0f, aspectRatio, 0.1f, 100.0f);
  glutPostRedisplay();*/
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
  glEnable(GL_MULTISAMPLE);

  glutInitWindowSize(600, 600);
  glutCreateWindow("ofv viewer");
  glutDisplayFunc(display);

  glutMotionFunc(mouseMotion);
  glutMouseFunc(mouse);

  glutReshapeFunc(reshape);

  init();
  glutMainLoop();

  return 0;
}