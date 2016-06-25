#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <SOIL/SOIL.h>

#define WIDTH 680
#define HEIGHT 480

float angleCube = 0.f;
float xRotation = 0.f;
float yRotation = 0.f;
float zRotation = 0.f;
bool toggle = false;

GLuint raw_texture_load( const char *filename, int width, int height )
{
  GLuint texture;
  unsigned char *data;
  FILE *file;

  // open texture data
  file = fopen(filename, "rb");
  if (file == NULL)
    return EXIT_FAILURE;

  // allocate buffer
  data = (unsigned char*) malloc(width * height * 4);

  // read texture data
  fread(data, width * height * 4, 1, file);
  fclose(file);

  // allocate a texture name
  glGenTextures(1, &texture);

  // select our current texture
  glBindTexture(GL_TEXTURE_2D, texture);

  // select modulate to mix texture with color for shading
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_DECAL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_DECAL);

  // when texture area is small, bilinear filter the closest mipmap
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

  // when texture area is large, bilinear filter the first mipmap
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // texture should tile
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // build our texture mipmaps
  gluBuild2DMipmaps( GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data );

  // free buffer
  free(data);

  return texture;
}

GLuint soil_texture_load( const char *filename )
{
  GLuint texture;

  int width, height;
  unsigned char* image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGB);

  // allocate a texture name
  glGenTextures(1, &texture);

  // select our current texture
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

  // free buffer
  SOIL_free_image_data(image);

  return texture;
}

void initGL()
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
  glViewport(0.f, 0.f, WIDTH, HEIGHT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // glOrtho(-3.f, 3.f, -3.f, 3.f, -10.f, 10.f);
  gluPerspective(45.f, aspect, 1.0f, 100.f);
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

  GLfloat xPlane[] = { 1.0f, 0.0f, 0.0f, 0.0f };
  GLfloat yPlane[] = { 0.0f, 1.0f, 0.0f, 0.0f };
  GLfloat zPlane[] = { 0.0f, 0.0f, 1.0f, 0.0f };

  GLuint mDiaplayList;
  mDiaplayList = glGenLists(1);

  glNewList(mDiaplayList, GL_COMPILE);
  glBegin(GL_QUADS);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(1.0f, 0.0f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(0.0f, 1.0f);
  glEnd();
  glEndList();

  glEnable( GL_TEXTURE_2D );
  GLuint texture = soil_texture_load("/home/prof/workspace/OpenGL/seg3.png");

  // glMatrixMode(GL_PROJECTION);
  // glPushMatrix();
  //   glLoadIdentity();
  //   gluOrtho2D(0.0f, 1.0f, 0.0f, 1.0f);
  //   glMatrixMode(GL_MODELVIEW);
    
  //   glDisable(GL_TEXTURE_GEN_S);
  //   glDisable(GL_TEXTURE_GEN_T);
    
  //   glBegin(GL_QUADS);
  //     glTexCoord2f(0.0f, 0.0f);
  //     glVertex2f(0.0f, 0.0f);

  //     glTexCoord2f(1.0f, 0.0f);
  //     glVertex2f(1.0f, 0.0f);

  //     glTexCoord2f(1.0f, 1.0f);
  //     glVertex2f(1.0f, 1.0f);

  //     glTexCoord2f(0.0f, 1.0f);
  //     glVertex2f(0.0f, 1.0f);
  //   glEnd();
  //   glMatrixMode(GL_PROJECTION);
  // glPopMatrix();

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  // Turn texgen
  glEnable(GL_TEXTURE_GEN_S);
  glEnable(GL_TEXTURE_GEN_T);

    // Save the matrix state and do the rotations
  glPushMatrix();
    glTranslatef(0.0f, 0.0f, -1.0f);
    glTexGenfv(GL_S, GL_EYE_PLANE, xPlane);
    glTexGenfv(GL_T, GL_EYE_PLANE, yPlane);
    
    glCallList(mDiaplayList);
  glPopMatrix();

  
  // glBindTexture( GL_TEXTURE_2D, texture );

  // glColor3f(1.f, 1.f, 1.f);
  // glTexCoord2d(0.f, 0.f);
  // glVertex2d(-1.f, -1.f);
  // glTexCoord2d(1.f, 0.f);
  // glVertex2d(+1.f, -1.f);
  // glTexCoord2d(1.f, 1.f);
  // glVertex2d(+1.f, +1.f);
  // glTexCoord2d(0.f, 1.f);
  // glVertex2d(-1.f, +1.f);

  /*
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
    glColor3f(0.6f, 0.3f, 0.0f);     // Red
    glTexCoord2d(0.f, 0.f);
    glVertex3f( 1.0f,  1.0f, 1.0f);
    glTexCoord2d(1.f, 0.f);
    glVertex3f(-1.0f,  1.0f, 1.0f);
    glTexCoord2d(1.f, 1.f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glTexCoord2d(0.f, 1.f);
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
  */

  angleCube += 0.2;
  glutSwapBuffers();
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE );
  glutInitWindowPosition(50, 50);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow(argv[1]);
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutSpecialFunc(keyboard);
  glutTimerFunc(0, Timer, 0);
  initGL();
  glutMainLoop();
  return EXIT_SUCCESS;
}
