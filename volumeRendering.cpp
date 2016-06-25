#include <iostream>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>

#define WIDTH 680
#define HEIGHT 480

GLfloat dOrthoSize = 1.0f;
GLfloat dViewPortSize = 1.0f;

GLuint* m_puTextureIDs;
GLuint mu3DTex, mDiaplayList, mVolTexureID;

int m_volumeDepth = 256;
int m_volumeWidth = 256;
int m_volumeHeight = 256;

float angle = 0.f;
int mUniDim = 256;

float mXrot = 5.f;
float mYrot = 5.f;
float mZrot = 5.f;

#define MAP_2DTEXT( TexIndex ) \
    glTexCoord2f(0.0f, 0.0f);  \
    glVertex3f(-dOrthoSize, -dOrthoSize,(TexIndex *2*dOrthoSize/fFrameCount)-1.0f);\
    glTexCoord2f(1.0f, 0.0f); \
    glVertex3f(dOrthoSize, -dOrthoSize,(TexIndex *2*dOrthoSize/fFrameCount)-1.0f);\
    glTexCoord2f(1.0f, 1.0f); \
    glVertex3f(dOrthoSize, dOrthoSize,(TexIndex *2*dOrthoSize/fFrameCount)-1.0f);\
    glTexCoord2f(0.0f, 1.0f); \
    glVertex3f(-dOrthoSize, dOrthoSize,(TexIndex *2*dOrthoSize/fFrameCount)-1.0f);

#define MAP_3DTEXT( TexIndex ) \
    glTexCoord3f(0.0f, 0.0f, ((float)TexIndex+1.0f)/2.0f);  \
    glVertex3f(-dViewPortSize,-dViewPortSize,TexIndex);\
    glTexCoord3f(1.0f, 0.0f, ((float)TexIndex+1.0f)/2.0f);  \
    glVertex3f(dViewPortSize,-dViewPortSize,TexIndex);\
    glTexCoord3f(1.0f, 1.0f, ((float)TexIndex+1.0f)/2.0f);  \
    glVertex3f(dViewPortSize,dViewPortSize,TexIndex);\
    glTexCoord3f(0.0f, 1.0f, ((float)TexIndex+1.0f)/2.0f);  \
    glVertex3f(-dViewPortSize,dViewPortSize,TexIndex);

bool initGL()
{
    glClearColor(0.1f, 0.1f, 0.1f, 0.f);
    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.f);
    glDepthFunc(GL_LEQUAL);

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

    //Set the orthographic projection.
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

bool initTextures2D( char const* volumePath )
{
    m_puTextureIDs = new GLuint[ m_volumeDepth ];
    FILE *file;

    file = fopen( volumePath, "rb");
    if (file == NULL)
        return EXIT_FAILURE;

    // Holds the luminance buffer
    char* chBuffer = new char[ m_volumeWidth * m_volumeHeight ];
    char* chRGBABuffer = new char[ m_volumeWidth * m_volumeHeight * 4 ];

    glGenTextures( m_volumeDepth, m_puTextureIDs );

    // Read each frames and construct the texture
    for( int nIndx = 0; nIndx < m_volumeDepth; ++nIndx )
    {
        // Read the frame
        fread(chBuffer, m_volumeWidth * m_volumeHeight, 1, file);

        // Set the properties of the texture.
        glBindTexture( GL_TEXTURE_2D, m_puTextureIDs[nIndx] );
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        for( int nIndx = 0; nIndx < m_volumeWidth*m_volumeWidth; ++nIndx )
        {
            chRGBABuffer[nIndx*4] = chBuffer[nIndx];
            chRGBABuffer[nIndx*4+1] = chBuffer[nIndx];
            chRGBABuffer[nIndx*4+2] = chBuffer[nIndx];
            chRGBABuffer[nIndx*4+3] = 255;

            if( chBuffer[nIndx] < 25 )
                chRGBABuffer[nIndx*4+3] = 0;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_volumeWidth, m_volumeHeight, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE,(GLvoid *) chRGBABuffer );
        glBindTexture( GL_TEXTURE_2D, 0 );
    }

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    delete[] chBuffer;
    fclose(file);
    return true;
}

bool initTextures3D( char const* volumePath )
{
    char *chBuffer, *chRGBABuffer;
    FILE *file;

    chBuffer = new char[ m_volumeWidth * m_volumeHeight * m_volumeDepth ];
    chRGBABuffer = new char[ m_volumeWidth * m_volumeHeight * m_volumeDepth * 4 ];

    file = fopen(volumePath, "rb");
    if (file == NULL)
        return EXIT_FAILURE;
    fread(chBuffer, m_volumeWidth * m_volumeHeight * m_volumeDepth, 1, file);

    glGenTextures(1, &mu3DTex );
    glBindTexture( GL_TEXTURE_3D, mu3DTex );
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    int size = m_volumeDepth * m_volumeHeight * m_volumeWidth;
    for( int nIndx = 0; nIndx < size; ++nIndx )
    {
        chRGBABuffer[nIndx*4] = chBuffer[nIndx];
        chRGBABuffer[nIndx*4+1] = chBuffer[nIndx];
        chRGBABuffer[nIndx*4+2] = chBuffer[nIndx];
        chRGBABuffer[nIndx*4+3] = chBuffer[nIndx];
    }
    glTexImage3D( GL_TEXTURE_3D, 0, GL_RGBA,
                  m_volumeWidth, m_volumeHeight, m_volumeDepth, 0,
                  GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid *) chRGBABuffer );
    glBindTexture( GL_TEXTURE_3D, 0 );

    fclose(file);
    delete[] chBuffer;
    delete[] chRGBABuffer;
    return true;
}

void Timer(int value)
{
  glutPostRedisplay();
  glutTimerFunc(15, Timer, 0);
}

void SetDisplayList(void)
{
    printf("Creating Display List ... \n"); 
    
    /* Central slice */ 
    float mCenter       = 0;
    
    /* Left & right sides */  
    float mSide         = 0.5;
    
    /* Central slice */ 
    int nSlices         = 1;  
    
    /* Number of verticies */ 
    int nElements       = 4 * nSlices; 
    
    /* Coordinates */ 
    GLfloat *vPoints    = new GLfloat [3 * nElements]; 
    GLfloat *ptr        = vPoints;

    /* Fill the Display List with Vertecies */
    *(ptr++) = -mSide;
    *(ptr++) = -mSide;
    *(ptr++) =  mCenter;

    *(ptr++) =  mSide;
    *(ptr++) = -mSide;
    *(ptr++) =  mCenter;

    *(ptr++) =  mSide;
    *(ptr++) =  mSide;
    *(ptr++) =  mCenter;

    *(ptr++) = -mSide;
    *(ptr++) =  mSide;
    *(ptr++) =  mCenter;

    /* Fill the Display List (VERTEX_ARRAY) with Vertecies */ 
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vPoints);
    mDiaplayList = glGenLists(1);
    glNewList(mDiaplayList, GL_COMPILE);
        glDrawArrays(GL_QUADS, 0, nElements); 
    glEndList();
    delete [] vPoints;
    
    printf("    Display List Created Successfully \n\n"); 
}

void extractSlice()
{
    /* Clear buffer */ 
    glClear(GL_COLOR_BUFFER_BIT);

    /* Enable 3D texturing */ 
    glEnable(GL_TEXTURE_3D);

    /* Replacing the old projection-slice */ 
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    
    /* Bind the 3D spectral texture */
    glBindTexture(GL_TEXTURE_3D, mVolTexureID);
    
    /* Adjust OpenGL view-port - Marawanism :) */
    glViewport(-(mUniDim / 2), -(mUniDim / 2), (mUniDim * 2), (mUniDim * 2));
    
    /* Texture corrdinate generation */ 
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glEnable(GL_TEXTURE_GEN_R);
    
    /* Loading identity to projection matrix */ 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    /* Loading identity to model-view matrix */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /* Define the main six clip planes */
    static GLdouble eqx0[4] = { 1.0, 0.0, 0.0, 0.0};
    static GLdouble eqx1[4] = {-1.0, 0.0, 0.0, 1.0};
    static GLdouble eqy0[4] = {0.0,  1.0, 0.0, 0.0};
    static GLdouble eqy1[4] = {0.0, -1.0, 0.0, 1.0};
    static GLdouble eqz0[4] = {0.0, 0.0,  1.0, 0.0};
    static GLdouble eqz1[4] = {0.0, 0.0, -1.0, 1.0};

    /* Automatic texture coordinate generation */
    static GLfloat x[] = {1.0, 0.0, 0.0, 0.0};
    static GLfloat y[] = {0.0, 1.0, 0.0, 0.0};
    static GLfloat z[] = {0.0, 0.0, 1.0, 0.0};

    /* Saving scene state */ 
    glPushMatrix ();
    
     
    /* Transform (Rotation Only) the Viewing Direction
     * We don't need except the - 0.5 translation in each dimension to adjust 
     * the Texture in the center of the scene i.e. (3D k-space)
     */ 
    glRotatef(-mXrot, 0.0, 0.0, 1.0);
    glRotatef(-mYrot, 0.0, 1.0, 0.0);
    glRotatef(-mZrot, 1.0, 0.0, 0.0);
    glTranslatef(-0.5, -0.5, -0.5);
    
    /* Automatic texture coordinates generation */
    glTexGenfv(GL_S, GL_EYE_PLANE, x);
    glTexGenfv(GL_T, GL_EYE_PLANE, y);
    glTexGenfv(GL_R, GL_EYE_PLANE, z);

    /* Define the main six clip planes */
    glClipPlane(GL_CLIP_PLANE0, eqx0);
    glClipPlane(GL_CLIP_PLANE1, eqx1);
    glClipPlane(GL_CLIP_PLANE2, eqy0);
    glClipPlane(GL_CLIP_PLANE3, eqy1);
    glClipPlane(GL_CLIP_PLANE4, eqz0);
    glClipPlane(GL_CLIP_PLANE5, eqz1);

    /* Restore the State */ 
    glPopMatrix ();

    /* Enable Clip Planes */
    glEnable(GL_CLIP_PLANE0);
    glEnable(GL_CLIP_PLANE1);
    glEnable(GL_CLIP_PLANE2);
    glEnable(GL_CLIP_PLANE3);
    glEnable(GL_CLIP_PLANE4);
    glEnable(GL_CLIP_PLANE5);

    /* Render enclosing rectangle at (0,0) that represents the 
     * extracted projection-slice */
    glCallList(mDiaplayList);
    glPopMatrix();  

    /* Disable texturing */  
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
    
    /* Unbind 3D spectral texture texture */  
    glBindTexture(GL_TEXTURE_3D, 0);
}

void keyboard(int key, int x, int y)
{
  switch (key)
  {
    case GLUT_KEY_LEFT:
      angle -= 0.1;
      break;
    case GLUT_KEY_RIGHT:
      angle += 0.1;
      break;
    case GLUT_KEY_UP:
      mXrot += 0.1;
      break;
    case GLUT_KEY_DOWN:
      mXrot -= 0.1;
      break;
  }
}

void display()
{
    float fFrameCount = m_volumeDepth;
    glClear( GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT );

    GLfloat xPlane[] = { 1.f, 0.0f, 0.0f, 0.0f };
    GLfloat yPlane[] = { 0.0f, 1.0f, 0.0f, 0.0f };
    GLfloat zPlane[] = { 0.0f, 0.0f, 1.0f, 0.0f };

    glEnable( GL_TEXTURE_3D );
    glBindTexture( GL_TEXTURE_3D,  mu3DTex );

    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glEnable(GL_TEXTURE_GEN_R);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();
        glRotatef(90, 1.0, 0.0, 0.0);
        glTranslatef(-0.5, -0.5, -0.5);
        glTexGenfv(GL_S, GL_EYE_PLANE, xPlane);
        glTexGenfv(GL_T, GL_EYE_PLANE, yPlane);
        glTexGenfv(GL_R, GL_EYE_PLANE, zPlane);
    glPopMatrix();

    glBegin( GL_QUADS );
        glVertex3f(-dViewPortSize,-dViewPortSize, 0);
        glVertex3f(dViewPortSize,-dViewPortSize, 0);
        glVertex3f(dViewPortSize,dViewPortSize, 0);
        glVertex3f(-dViewPortSize,dViewPortSize, 0);
    glEnd();
    angle+= 0.1;
    glutSwapBuffers();
}

int main( int argc, char** argv )
{
    glutInit(&argc, argv);
    glutInitWindowSize( WIDTH, HEIGHT );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE );
    glutCreateWindow( "Yalla Render" );
    glutReshapeFunc( reshape );
    glutTimerFunc(0, Timer, 0);
    initGL();
    initTextures3D( "/home/prof/volumeRendering/skull.raw" );
    glutDisplayFunc( display );
    glutSpecialFunc( keyboard );
    glutMainLoop();

    return EXIT_SUCCESS;
}
