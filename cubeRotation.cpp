/**
@AUTHORS: Mohamed Hosny Ahmed
**/

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>


#define WIDTH 256
#define HEIGHT 256

int m_width = 300;
int m_height = 320;

float angleCube = 0.f;
float xRotation = 0.f;
float yRotation = 0.f;
float zRotation = 0.f;

float eyeX = 0.f;
float eyeY = 0.f;
float eyeZ = 0.f;

bool toggle = false;

GLuint fb, color, depth;

void glInit()
{
  glClearColor(0.f, 0.f, 0.f, 0.f);
  glEnable(GL_DEPTH_TEST);
  glClearDepth(1.f);
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

void CHECK_FRAMEBUFFER_STATUS()
{  
  GLenum status;
  status = glCheckFramebufferStatus(GL_FRAMEBUFFER_EXT); 
  switch(status) 
  {
    case GL_FRAMEBUFFER_COMPLETE:
      break;

    case GL_FRAMEBUFFER_UNSUPPORTED:
    /* choose different formats */
      break;

    default:
      /* programming error; will fail on all hardware */
      fputs("Framebuffer Error\n", stderr);
      exit(-1);
  }                                                     
}

void initFBO()
{
  glGenTextures(1, &color);
  glBindTexture(GL_TEXTURE_2D, color);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);

  glGenFramebuffers(1, &fb);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);
  glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color, 0);

  CHECK_FRAMEBUFFER_STATUS();
}

void drawCube()
{
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
}

void drawAxis()
{
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
}

void display()
{
  // attach our framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
  gluLookAt( 5.f, 5.f, 5.f, 1.f, 1.f, 1.f, 0.f ,1.f, 0.f );

  glClearColor(0 , 0, 0, 0);
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  int mUniDim = 256;
  glViewport(-(mUniDim / 2), -(mUniDim / 2), (mUniDim * 2), (mUniDim * 2));

  GLdouble equX[]  = {1.f, 0.f, 0.f, 0.f};
  GLdouble equX1[] = {1.f, 0.f, 0.f, -1.f};
  GLdouble equY[]  = {0.f, 1.f, 0.f, 0.f};
  GLdouble equY1[] = {0.f, -1.f, 0.f, 1.f};
  GLdouble equZ[]  = {0.f, 0.f, 1.f, 0.f};
  GLdouble equZ1[] = {0.f, 0.f, -1.f, 1.f};

  /* Loading identity to projection matrix */ 
  // glMatrixMode(GL_PROJECTION);
  // glLoadIdentity();

  /* Loading identity to model-view matrix */
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  drawAxis();

  glTranslatef(-1.f, -1.f, -1.f);
  glRotatef(angleCube, 0.f, 0.f, 1.f);
  glTranslatef(1.f, 1.f, 1.f);

  glPushMatrix();
  // glRotatef(-30.f, 0.f, 0.f, 1.f);
  // glRotatef(-60.f, 0.f, 1.f, 0.f);
  // glRotatef(-15.f, 1.f, 0.f, 0.f);
    glTranslatef(-1.f, -1.f, -1.f);
    glRotatef(65.f, 0.f, 1.f, 0.f);
    glTranslatef(-0.5f, -0.5f, -0.5f);
    drawCube();

    glClipPlane(GL_CLIP_PLANE0, equX);
    glClipPlane(GL_CLIP_PLANE1, equX1);
    glClipPlane(GL_CLIP_PLANE2, equY);
    glClipPlane(GL_CLIP_PLANE3, equY1);
    glClipPlane(GL_CLIP_PLANE4, equZ);
    glClipPlane(GL_CLIP_PLANE5, equZ1);
  glPopMatrix();

    glEnable(GL_CLIP_PLANE0);
    glEnable(GL_CLIP_PLANE1);
    glEnable(GL_CLIP_PLANE2);
    glEnable(GL_CLIP_PLANE3);
    glEnable(GL_CLIP_PLANE4);
    glEnable(GL_CLIP_PLANE5);
  
  angleCube += 0.3;
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

  glewInit();
  glInit();

  glutTimerFunc(0, Timer, 0);
  glutMainLoop();
}
