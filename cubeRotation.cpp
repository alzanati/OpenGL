#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>


#define WIDTH 680
#define HEIGHT 480

float angleCube = 0.f;

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
  float aspect = WIDTH / HEIGHT;
  glViewport(0, 0, WIDTH, HEIGHT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(45.f, aspect, 0.1f, 100.f);
}

void Timer(int value)
{
  glutPostRedisplay();
  glutTimerFunc(15, Timer, 0);
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);

  // Render a color-cube consisting of 6 quads with different colors
  glLoadIdentity();
  glTranslatef(1.f, 1.0f, -7.0f);
  glRotatef(angleCube, 1.0f, 1.0f, 0.0f);

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

  angleCube += 0.2;
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
  glInit();
  glutTimerFunc(0, Timer, 0);
  glutMainLoop();
}
