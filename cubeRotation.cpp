/**
@AUTHORS: Mohamed Hosny Ahmed
**/

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>


#define WIDTH 680
#define HEIGHT 480

float angleCube = 0.f;
float xRotation = 0.f;
float yRotation = 0.f;
float zRotation = 0.f;
bool toggle = false;


void glInit()
{
  glClearColor(0.f, 0.f, 0.f, 0.f);
  glEnable(GL_DEPTH_TEST);
  glClearDepth(1.f);
  glDepthFunc(GL_LEQUAL);
  glShadeModel(GL_SMOOTH);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void reshape(int w, int h)
{
  float aspect = (float) w / (float) h;
  glViewport(0, 0, WIDTH, HEIGHT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // glOrtho(-3.f, 3.f, -3.f, 3.f, -10.f, 10.f);
  gluPerspective(45.f, aspect, 1.f, 100.f);
}

void Timer(int value)
{
  glutPostRedisplay();
  glutTimerFunc(15, Timer, 0);
}

void keyboard(int key, int x, int y)
{
  switch (key)
  {
    case GLUT_KEY_LEFT:
      xRotation -= 0.1;
      break;
    case GLUT_KEY_RIGHT:
      xRotation += 0.1;
      break;
    case GLUT_KEY_UP:
      yRotation += 0.1;
      break;
    case GLUT_KEY_DOWN:
      yRotation -= 0.1;
      break;
  }

}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // camera transformations
  gluLookAt(2.5f, 2.5f, 2.5f,
            xRotation, yRotation, 0.f,
            0.f ,1.f, 0.f);

  // draw axies
  glBegin(GL_LINES);
    // x-axis
    glColor3f(1.f, 0.f, 0.f);
    glVertex3f(-50.f, 0.f, 0.f);
    glVertex3f(50.f, 0.f, 0.f);
    // y-axis
    glColor3f(0.f, 1.f, 0.f);
    glVertex3f(0.f, -50.f, 0.f);
    glVertex3f(0.f, 50.f, 0.f);
    // z-axis
    glColor3f(0.f, 0.f, 1.f);
    glVertex3f(0.f, 0.f, 50.f);
    glVertex3f(0.f, 0.f, -50.f);
  glEnd();

  // draw cube
  glPushMatrix();
  glRotatef(angleCube, 0.f, 0.f, 1.f);

  glBegin(GL_QUADS);
    glColor3f(0.0f, 1.0f, 0.0f);     // Green
    glVertex3f( 1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f,  1.0f);
    glVertex3f( 1.0f, 1.0f,  1.0f);

    // Bottom face (y = -1.0f)
    glColor3f(1.0f, 0.5f, 0.0f);     // Orange
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);

    // Front face  (z = 1.0f)
    glColor3f(1.0f, 0.0f, 0.0f);     // Red
    glVertex3f( 1.0f,  1.0f, 1.0f);
    glVertex3f(-1.0f,  1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f( 1.0f, -1.0f, 1.0f);

    // Back face (z = -1.0f)
    glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);

    // Left face (x = -1.0f)
    glColor3f(0.0f, 0.0f, 1.0f);     // Blue
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);

    // Right face (x = 1.0f)
    glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
    glVertex3f(1.0f,  1.0f, -1.0f);
    glVertex3f(1.0f,  1.0f,  1.0f);
    glVertex3f(1.0f, -1.0f,  1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
  glEnd();

  glPopMatrix();

  if( angleCube >= 100.f )
    toggle = true;
  else if( angleCube <= 0.f )
    toggle = false;

  if( toggle == false )
    angleCube += 0.2;
  else if( toggle == true )
    angleCube -= 0.2;

  glutSwapBuffers();
}

int main(int argc, char** argv)
{
  int mode = GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE;
  glutInit(&argc, argv);
  glutInitDisplayMode(mode);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow(argv[1]);
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutSpecialFunc(keyboard);
  glInit();
  glutTimerFunc(0, Timer, 0);
  glutMainLoop();
}
