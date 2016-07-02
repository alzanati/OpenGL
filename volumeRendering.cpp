#include <iostream>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>

#define WIDTH 680
#define HEIGHT 480

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

/* macro to draw slice becuase it's not changing over code */
#define SLICE() \
glTexCoord2f(0.0, 0.0); glVertex2f(0.0, 0.0);\
glTexCoord2f(1.0, 0.0); glVertex2f(1.0, 0.0);\
glTexCoord2f(1.0, 1.0); glVertex2f(1.0, 1.0);\
glTexCoord2f(0.0, 1.0); glVertex2f(0.0, 1.0);


GLfloat dOrthoSize = 1.0f;
GLfloat dViewPortSize = 1.0f;

GLuint mu3DTex, sliceList, mVolTexureID, Slice, framebuffer, depth_rb;
GLuint pbo[2];

int m_volumeDepth = 256;
int m_volumeWidth = 256;
int m_volumeHeight = 256;

/* plane equations */
GLdouble rightPlaneX[4] = { 1.0, 0.0, 0.0, 0.0};
GLdouble leftPlaneX [4] = {-1.0, 0.0, 0.0, 1.0};
GLdouble rightPlaneY[4] = {0.0,  1.0, 0.0, 0.0};
GLdouble leftPlaneY [4] = {0.0, -1.0, 0.0, 1.0};
GLdouble rightPlaneZ[4] = {0.0, 0.0,  1.0, 0.0};
GLdouble leftPlaneZ [4] = {0.0, 0.0, -1.0, 1.0};

/* texture coordinates */
GLfloat xCoord[] = { 1.f, 0.f, 0.f, 0.f };
GLfloat yCoord[] = { 0.f, 1.f, 0.f, 0.f };
GLfloat zCoord[] = { 0.f, 0.f, 1.f, 0.f };

bool initGL()
{
    /* clear colors then enabling depth test */
    glClearColor(0.1f, 0.1f, 0.1f, 0.f);
    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.f);
    glDepthFunc(GL_LEQUAL);

    /* check glew cabability */
    if( glewInit() != GLEW_OK )
        std::cerr << "GLEW NOT INSTALLED" << std::endl;

    if( GL_TRUE != glewGetExtension("GL_EXT_texture3D") )
    {
        std::cerr << "3D texture not supported " <<
                     __LINE__ << ":" << __FUNCTION__ << std::endl;
        return false;
    }
    else
        return true;
}

void reshape( const int t_width, const int t_height )
{
    GLdouble AspectRatio = ( GLdouble )(t_width) / ( GLdouble )(t_height );
    glViewport( 0, 0, t_width, t_height );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    /* Set the orthographic projection. */
    if( t_width <= t_height )
    {
        glOrtho( -dOrthoSize, dOrthoSize, -( dOrthoSize / AspectRatio ) ,
        dOrthoSize / AspectRatio, 2.0f*-dOrthoSize, 2.0f*dOrthoSize );
    }
    else
    {
        glOrtho( -dOrthoSize * AspectRatio, dOrthoSize * AspectRatio,
        -dOrthoSize, dOrthoSize, 2.0f*-dOrthoSize, 2.0f*dOrthoSize );
    }

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}

void CHECK_FRAMEBUFFER_STATUS()
{  
  GLenum status;
  status = glCheckFramebufferStatus(GL_FRAMEBUFFER_EXT); 
  switch(status) 
  {
    case GL_FRAMEBUFFER_COMPLETE:
        printf("%s\n", "frame buffer is created successfully");
        break;

    case GL_FRAMEBUFFER_UNSUPPORTED:
        printf("%s\n", "frame buffer is not supported try different formate!!!");
        break;

    default:
      /* programming error; will fail on all hardware */
      fputs("Framebuffer Error\n", stderr);
      exit(-1);
  }                                                     
}

bool initTextures3D( char const* volumePath )
{
    char *chBuffer;
    FILE *file;

    /* create to buffers to store data from .raw file then applying blending in another buffer */
    chBuffer = new char[ m_volumeWidth * m_volumeHeight * m_volumeDepth ];

    file = fopen(volumePath, "rb");
    if (file == NULL)
        return EXIT_FAILURE;
    fread(chBuffer, m_volumeWidth * m_volumeHeight * m_volumeDepth, 1, file);

    /* spaceify texture parameters */
    glGenTextures(1, &mu3DTex );
    glBindTexture( GL_TEXTURE_3D, mu3DTex );
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


    int size = m_volumeDepth * m_volumeHeight * m_volumeWidth;
    /* store data to bounded 3d texture */
    glTexImage3D( GL_TEXTURE_3D, 0, GL_LUMINANCE, m_volumeWidth, m_volumeHeight, 
                  m_volumeDepth, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, (GLvoid *) chBuffer );

    glBindTexture( GL_TEXTURE_3D, 0 );

    fclose(file);
    delete[] chBuffer;
    return true;
}

void initFBO()
{
  glGenTextures(1, &Slice);
  glBindTexture( GL_TEXTURE_2D, Slice );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
  glTexImage2D( GL_TEXTURE_2D, 0, GL_LUMINANCE, m_volumeWidth, m_volumeHeight, 
                0, GL_LUMINANCE, GL_UNSIGNED_BYTE, NULL ); // data is null to fill it with frame buffer data

  glGenFramebuffersEXT( 1, &framebuffer );
  glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, framebuffer );
  glFramebufferTexture2D( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, 
                          GL_TEXTURE_2D, Slice, 0 );

  glGenRenderbuffersEXT(1, &depth_rb);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depth_rb);
  glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, m_volumeHeight, m_volumeWidth);
  glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depth_rb);

  CHECK_FRAMEBUFFER_STATUS();
}

void display()
{
    /* bind our framebuffer to update texture with slice data */
    glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, framebuffer );

    /* start rendering to this framebuffer */
    glClear( GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT );
    /* adujst the view to put data from index (0, 0) to width and height */
    /* so when read back we can read from (0, 0) to width and height of volume*/
    glViewport(-128, -128, 512, 512);

    glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, pbo[0]);
    glBufferData(GL_PIXEL_PACK_BUFFER_ARB, m_volumeWidth * m_volumeHeight, NULL, GL_STREAM_READ);

    /* enable 3d texture */
    glEnable( GL_TEXTURE_3D );
    glBindTexture( GL_TEXTURE_3D,  mu3DTex );

    /* enable automatic texture generation, this helpful to avoid multiple using of glTexCoord*/
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glEnable(GL_TEXTURE_GEN_R);

    /* load projection matrix to convert from 3d to 2d screen */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    /* load projection matrix to map object to word coordinates and perform transformations */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /* save state and perform transformation on texture to put it in (0, 0) with clipp planes */
    glPushMatrix();

        /*around with 270 get z normal slice, around x with 90 get y normal slice*/
        glRotatef(0, 1.f, 0.f, 0.f); 
        glTranslatef(-0.5, -0.5, -0.5); // translate 0.5 to get center at 0

        /* this will generate textures in eye coordinates which help us in projection */
        glTexGenfv(GL_S, GL_EYE_PLANE, xCoord); 
        glTexGenfv(GL_T, GL_EYE_PLANE, yCoord);
        glTexGenfv(GL_R, GL_EYE_PLANE, zCoord);

        /* really here we have slice but with carbage data from rendering so we need to clip them
           so clip planes are defind accodring to projection in eye plane */

        /* Define the main six clip planes to cut boundaries that don't belong to slice */
        glClipPlane(GL_CLIP_PLANE0, rightPlaneX);
        glClipPlane(GL_CLIP_PLANE1, leftPlaneX);
        glClipPlane(GL_CLIP_PLANE2, rightPlaneY);
        glClipPlane(GL_CLIP_PLANE3, leftPlaneY);
        glClipPlane(GL_CLIP_PLANE4, rightPlaneZ);
        glClipPlane(GL_CLIP_PLANE5, leftPlaneZ);
    glPopMatrix();

    /* to ensure that frame buffer does not contain redundancy of data */
    /* clip planes according to current view in projection eye */
    glEnable(GL_CLIP_PLANE0);
    glEnable(GL_CLIP_PLANE1);
    glEnable(GL_CLIP_PLANE2);
    glEnable(GL_CLIP_PLANE3);
    glEnable(GL_CLIP_PLANE4);
    glEnable(GL_CLIP_PLANE5);
    /* now we have extracted slice :) with remove carbage */

    /* draw extracted slice to ensure correct algorithm 
       but the already saved in framebuffer color buffer */
    glBegin( GL_QUADS );
        glVertex3f(-dViewPortSize,-dViewPortSize, 0);
        glVertex3f(dViewPortSize,-dViewPortSize, 0);
        glVertex3f(dViewPortSize,dViewPortSize, 0);
        glVertex3f(-dViewPortSize,dViewPortSize, 0);
    glEnd();

    freopen("output.txt","w",stdout);
    // size_t size = sizeof(unsigned char) * m_volumeHeight * m_volumeWidth;
    // unsigned char* ptr = (unsigned char*) malloc( size );
    // glReadPixels(0, 0, m_volumeHeight, m_volumeWidth, GL_LUMINANCE, GL_UNSIGNED_BYTE, ptr);
    // for(int i = 0; i < 256 * 256; i++)
    //     std::cout << (int)ptr[i] << std::endl;

    glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, pbo[0]);
    glReadPixels(0, 0, m_volumeWidth, m_volumeHeight, GL_LUMINANCE, GL_UNSIGNED_BYTE, BUFFER_OFFSET(0));
    unsigned char* ptr = (unsigned char*)glMapBufferARB(GL_PIXEL_PACK_BUFFER_ARB, GL_READ_ONLY);
    if(ptr)
    {
        for(int i = 0; i < 256 * 256; i++)
        std::cout << (int)ptr[i] << std::endl;
    }

    /* back to default framebuffer to render extracted slice */
    glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
    glClearColor( 0.1f, 0.1f, 0.1f, 0.f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glViewport(-WIDTH/2, -HEIGHT/2, 2*WIDTH, 2*HEIGHT);

    /* bind our texture that represent slice */
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, Slice );

    /* neccassry transformations to see extracted slice */
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glTranslatef( -0.5, -0.5, 0.0 );

    /* mapping texture coordinates to see if slice is correct or not */
    glBegin(GL_QUADS);
        SLICE()
    glEnd();

    /* swap buffers to get update buffer to render seen */
    glutSwapBuffers();

    /* Disable texturing */ 
    glDisable(GL_TEXTURE_2D); 
    glDisable(GL_TEXTURE_3D);
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_GEN_R);
    
    /* Unloading clip-planes */ 
    glDisable(GL_CLIP_PLANE0);
    glDisable(GL_CLIP_PLANE1);
    glDisable(GL_CLIP_PLANE2);
    glDisable(GL_CLIP_PLANE3);
    glDisable(GL_CLIP_PLANE4);
    glDisable(GL_CLIP_PLANE5);
}

void initPBO()
{
    glGenBuffers(2, pbo);
}

void unitTest( int argc, char** argv )
{
    glutInit(&argc, argv);
    glutInitWindowSize( WIDTH, HEIGHT );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE );
    glutCreateWindow( "Yalla Render" );
    glutReshapeFunc( reshape );

    glewInit();
    printf("ARB FBO: %i\n",glewIsSupported("GL_ARB_framebuffer_object"));
    printf("EXT FBO: %i\n",glewIsSupported("GL_EXT_framebuffer_object"));
    
    initGL();
    initTextures3D( "/home/mohamed/workspace/OpenGL/foot.raw" );
    initPBO();
    initFBO();

    glutDisplayFunc( display );
    glutMainLoop();
}

int main( int argc, char** argv )
{
    /* test opengl implementations */
    unitTest(argc, argv);

    return EXIT_SUCCESS;
}
