#include <math.h>
#include <GL/glut.h>

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
//#include <glm/glm.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/matrix_inverse.hpp>

#include <iostream>
//#include "tdlib.h"

#define NEAR_CLIPPING_PLANE 1
#define FAR_CLIPPING_PLANE 100
#define FOV 40
#define ZOOM_SENSITIVITY 0.5
#define MAX_CAMERA_DISTANCE 50
#define MIN_CAMERA_DISTANCE 2
#define X_ROT_SENSITIVITY 0.01
#define Y_ROT_SENSITIVITY 0.01
#define _360_DEG 6.283185307
#define _90_DEG 1.570796327
using namespace std;

// camera orbit
float cameraDistance = 11;
int lastScreenCoor[2]; //
bool orbit = false;
float cameraTarget[3] = {0.0, 0.0, 0.0};
float cameraPosition[3];
float theta = 0, phi = 0;


//glm::mat4 projection;

GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};  /* White diffuse light. */
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
  cameraPosition[0] = cameraTarget[0] + cameraDistance * sin(theta) * cos(phi);
  cameraPosition[1] = cameraTarget[1] + cameraDistance * sin(phi);
  cameraPosition[2] = cameraTarget[2] + cameraDistance * cos(theta) * cos(phi);
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

  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
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