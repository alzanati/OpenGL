/**
@AUTHORS: Mohamed Hosny Ahmed
**/

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>


#define WIDTH 680
#define HEIGHT 480

int m_width = 300;
int m_height = 300;

float angleCube = 0.f;
float xRotation = 0.f;
float yRotation = 0.f;
float zRotation = 0.f;
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

void display()
{
  // attach our framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER_EXT, fb);

  glViewport(0, 0, m_width, m_height);
  glClearColor(0 , 0, 0, 0);
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glDisable(GL_TEXTURE_2D);
  glDisable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glPushMatrix();
    glColor3f(0.4f, 1.0f, 0.0f);
    glTranslatef(-0.5, -0.5, -5);
    glutWireSphere(0.8, 20, 20);
  glPopMatrix();

  // attach default frame buffer
  glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
  glClearColor(0.1f, 0.1f, 0.1f, 0.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, WIDTH, HEIGHT);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, color);

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
  glTranslatef(-0.1, -0.1, -2.0);

  glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(1.0, 0.0, 0.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 1.0, 0.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(0.0, 1.0, 0.0);
  glEnd();
  
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
  printf("ARB FBO: %i\n",glewIsSupported("GL_ARB_framebuffer_object"));
  printf("EXT FBO: %i\n",glewIsSupported("GL_EXT_framebuffer_object"));

  glInit();
  initFBO();

  glutTimerFunc(0, Timer, 0);
  glutMainLoop();
}
