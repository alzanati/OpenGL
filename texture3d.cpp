#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 680
#define HEIGHT 480

float zAmt = -1.0f/256;
float zPos = 0.5;
float angle = 0.f;
float yRotation = 0.f;
float xRotation = 0.f;

GLfloat* backData = (GLfloat*) malloc(sizeof(GLfloat) * 256 * 256);

GLuint raw_texture_load( const char *filename, int width, int height )
{
  GLuint texture;
  unsigned char *data;
  FILE *file;

  file = fopen(filename, "rb");
  if (file == NULL)
    return EXIT_FAILURE;

  data = (unsigned char*) malloc(width * height * 4);

  fread(data, width * height * 4, 1, file);
  fclose(file);

  // allocate a texture name
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_DECAL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_DECAL);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  gluBuild2DMipmaps( GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data );

  free(data);
  return texture;
}

GLuint raw_texture3d_load( const char *filename, int width, int height, int depth )
{
  GLuint texture;
  unsigned char *data;
  FILE *file;

  file = fopen(filename, "rb");
  if (file == NULL)
    return EXIT_FAILURE;

  data = (unsigned char*) malloc(width * height * depth * 4);

  fread(data, width * height * depth * 4, 1, file);
  fclose(file);

  // allocate a texture name
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_3D, texture);

  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  // gluBuild3DMipmaps( GL_TEXTURE_3D, 4, width, height, depth, GL_RGBA, GL_UNSIGNED_BYTE, data );
  glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, width, height, depth, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  free(data);
  return texture;
}

void initGL()
{
  glClearColor(0.1f, 0.1f, 0.1f, 0.f);
  glEnable(GL_DEPTH_TEST);
  glClearDepth(1.f);
  glDepthFunc(GL_LEQUAL);
  glShadeModel(GL_SMOOTH);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void reshape(int w, int h)
{
  float ratio = (float) w / (float) h;
  glViewport( 0, 0, WIDTH, HEIGHT );

  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();

  gluPerspective( 45.f, ratio, 1.f, 100.f );
}

void Timer(int value)
{
  glutPostRedisplay();
  glutTimerFunc( 15, Timer, 0 );
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
      angle += 0.1;
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
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();

  gluLookAt( 2.f, 2.f, 2.5f,
             0.f, 0.f, 0.f,
             0.f ,1.f, 0.f );

  // draw axies
  glBegin( GL_LINES );
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
    glVertex3f(0.f, 0.f, -50.f);
    glVertex3f(0.f, 0.f, 50.f);
  glEnd();


  glEnable( GL_TEXTURE_3D );
  GLuint texture = raw_texture3d_load("skull.raw", 256, 256, 256);
  glBindTexture( GL_TEXTURE_3D, texture );

  zPos += zAmt/2.0f;

  glRotatef(angle, 0.f, 1.f, 0.f);
  glTranslatef(xRotation, yRotation, 1.4f);
  glBegin(GL_QUADS);
    zPos += zAmt / 2.0f;
    for(int i = 100-1; i >= 0; i--)
    {
      float tex = (((float)i)/256);
      glTexCoord3f(0, 0, tex); glVertex3f(0, 0, zPos);
      glTexCoord3f(0, 0.15, tex); glVertex3f(0,  1, zPos);
      glTexCoord3f(0.15, 0.15, tex); glVertex3f( 1,  1, zPos);
      glTexCoord3f(0.15, 0, tex); glVertex3f( 1, 0, zPos);
      zPos += zAmt;
    }
  glEnd();

  // create a frame buffer object
  // bind it to screen
  // read it by glReadPixles to specific target

  glPushMatrix();
  glTranslatef(-2.f, 0.f, 0.4f);
  glBegin(GL_QUADS);
    glColor3f(1.f, 0.f, 0.f);
    glVertex3f(1.f, 1.f, 0.f);
    glVertex3f(-1.f, 1.f, 0.f);
    glVertex3f(-1.f, -1.f, 0.f);
    glVertex3f(1.f, -1.f, 0.f);
  glEnd();
  glPopMatrix();

  glutSwapBuffers();
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitWindowSize( WIDTH, HEIGHT );
  glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE );
  glutCreateWindow(argv[1]);
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutSpecialFunc(keyboard);
  initGL();
  //glutTimerFunc(0, Timer, 0);
  glutMainLoop();

  return EXIT_SUCCESS;
}
