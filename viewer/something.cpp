
#include <GL/glew.h>
#include <GL/glut.h>

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <iostream>

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

#define CAMERA_DISTANCE 11
#define NEAR_CLIPPING_PLANE 1
#define FAR_CLIPPING_PLANE 100
#define FOV 45
#define X_ROT_SENSITIVITY 0.3
#define Y_ROT_SENSITIVITY 0.1
using namespace std;

//GLMmodel* objModel;

GLuint programID;
GLuint modelID;
GLuint viewID;
GLuint projectionID;

glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;

GLuint vertexbuffer;
GLuint normalbuffer;

// camera orbit
float xrot = 0;
float yrot = 0;
float xdiff = 0;
float ydiff = 0;
bool mouseDown = false;

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


GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path)
{
  
  // Create the shaders
  GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
  GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
  
  // Read the Vertex Shader code from the file
  std::string VertexShaderCode;
  std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
  if(VertexShaderStream.is_open())
    {
      std::string Line = "";
      while(getline(VertexShaderStream, Line))
  VertexShaderCode += "\n" + Line;
      VertexShaderStream.close();
    }
  else
    {
      printf("Cannot open %s\n", vertex_file_path);
      return 0;
    }
  
  // Read the Fragment Shader code from the file
  std::string FragmentShaderCode;
  std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
  if(FragmentShaderStream.is_open())
    {
      std::string Line = "";
      while(getline(FragmentShaderStream, Line))
  FragmentShaderCode += "\n" + Line;
      FragmentShaderStream.close();
  }

  GLint Result = GL_FALSE;
  int InfoLogLength;

  // Compile Vertex Shader
  printf("Compiling shader : %s\n", vertex_file_path);
  char const * VertexSourcePointer = VertexShaderCode.c_str();
  glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
  glCompileShader(VertexShaderID);

  // Check Vertex Shader
  glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if ( InfoLogLength > 0 )
    {
      std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
      glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
      printf("%s\n", &VertexShaderErrorMessage[0]);
    }
  


  // Compile Fragment Shader
  printf("Compiling shader : %s\n", fragment_file_path);
  char const * FragmentSourcePointer = FragmentShaderCode.c_str();
  glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
  glCompileShader(FragmentShaderID);

  // Check Fragment Shader
  glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if ( InfoLogLength > 0 )
    {
      std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
      glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
      printf("%s\n", &FragmentShaderErrorMessage[0]);
    }

  // Link the program
  printf("Linking program\n");
  GLuint ProgramID = glCreateProgram();
  glAttachShader(ProgramID, VertexShaderID);
  glAttachShader(ProgramID, FragmentShaderID);
  glLinkProgram(ProgramID);

  // Check the program
  glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
  glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if ( InfoLogLength > 0 )
    {
      std::vector<char> ProgramErrorMessage(InfoLogLength+1);
      glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
      printf("%s\n", &ProgramErrorMessage[0]);
    }
  
  glDeleteShader(VertexShaderID);
  glDeleteShader(FragmentShaderID);

  return ProgramID;
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

  model = glm::mat4(1.0f);
  glm::mat4 xRotMat = glm::rotate(glm::mat4(1.0f), xrot, glm::normalize(glm::vec3(glm::inverse(model) * glm::vec4(1, 0, 0, 1))) );
  model = model * xRotMat;
  glm::mat4 yRotMat = glm::rotate(glm::mat4(1.0f), yrot, glm::normalize(glm::vec3(glm::inverse(model) * glm::vec4(0, 1, 0, 1))) );
  model = model * yRotMat;

  glm::mat4 modelViewMatrix = view * model;
  glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(modelViewMatrix)); // Normal Matrix

  // Use our shader
  glUseProgram(programID);
  // Send the model, view and projection matrices to the shader 
  glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);
  glUniformMatrix4fv(viewID, 1, GL_FALSE, &view[0][0]);
  glUniformMatrix4fv(projectionID, 1, GL_FALSE, &projection[0][0]);
  glUniformMatrix3fv( glGetUniformLocation(programID, "normalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);

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
  //glEnable(GL_LIGHT0);
  //glEnable(GL_LIGHTING);

  //glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  //glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  /* Set background color */
  glClearColor(.95, .95, .95, 1);

  /* Use depth buffering for hidden surface elimination. */
  glEnable(GL_DEPTH_TEST);
  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS);

  programID = LoadShaders( "shaders/vertShader.glsl", "shaders/fragShader.glsl" );

  // Get a handle for our model, view and projection uniforms
  modelID = glGetUniformLocation(programID, "model");
  viewID = glGetUniformLocation(programID, "view");
  projectionID = glGetUniformLocation(programID, "projection");

  glm::vec4 light_ambient = glm::vec4( 0.1, 0.1, 0.1, 0.5 );
  glm::vec4 light_diffuse = glm::vec4 ( 0.8, 1.0, 1.0, 1.0 );
  glm::vec4 light_specular =glm::vec4( 0.8, 1.0, 1.0, 1.0 );

  glUseProgram(programID);
  glUniform4fv( glGetUniformLocation(programID, "light_ambient"),1,&light_ambient[0]);
  glUniform4fv( glGetUniformLocation(programID, "light_diffuse"),1, &light_diffuse[0]);
  glUniform4fv( glGetUniformLocation(programID, "light_specular"),1, &light_specular[0]);

  projection = glm::perspective(FOV, 1, NEAR_CLIPPING_PLANE, FAR_CLIPPING_PLANE);
  view = glm::lookAt( glm::vec3(0,0,3), // Camera position in World Space
          glm::vec3(0,0,0), // and looks at the origin
          glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
          );
  model = glm::mat4(1.0f);

  // Initialize a light
  glm::vec4 lightPosition = glm::vec4(-20, -10, 0, 0);
  glUniform4fv( glGetUniformLocation(programID, "lightPos"),1, &lightPosition[0]); 
}

void mouseMotion(int x, int y)
{
  if (mouseDown)
  {
    yrot = x - xdiff;
    xrot = y + ydiff;
    glutPostRedisplay();
  }
}
void mouse(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON) 
  { 
    if (state == GLUT_DOWN)
    {
      mouseDown = true;
      xdiff = x - yrot;
      ydiff = -y + xrot;
    }
    else if (state == GLUT_UP)
    {
      mouseDown = false;
    }
  }
}

void reshape(int width, int height)
{
  glViewport(0, 0, width, height);
  GLfloat aspectRatio = GLfloat(width)/height;
  projection = glm::perspective((float) FOV, aspectRatio, (float) NEAR_CLIPPING_PLANE, (float) FAR_CLIPPING_PLANE);
  glutPostRedisplay();
}

void getGLinfo()
{
  cout << "GL Vendor   : " << glGetString(GL_VENDOR) << endl;
  cout << "GL Renderer : " << glGetString(GL_RENDERER) << endl;
  cout << "GL Version  : " << glGetString(GL_VERSION) << endl;
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
  glEnable(GL_MULTISAMPLE);

  glutInitWindowSize(600, 600);
  glutCreateWindow("ofv viewer");

  getGLinfo();
  init();

  glutDisplayFunc(display);

  glutReshapeFunc(reshape);

  glutMouseFunc(mouse);
  glutMotionFunc(mouseMotion);

  glutMainLoop();

  return 0;
}