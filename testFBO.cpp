#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <stdio.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#endif

#define WIDTH 512
#define HIEGHT 512

#define m_WIDTH 256
#define m_HIEGHT 256

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

GLuint fbo, color_tex, depth_rb, pbo[2];

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	float ratio =  w * 1.0 / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45,ratio,1,100);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void initFBO()
{
	glGenTextures(1, &color_tex);
  	glBindTexture(GL_TEXTURE_2D, color_tex);
  	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_WIDTH, m_HIEGHT, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
  	glBindTexture(GL_TEXTURE_2D, 0);

  	glGenFramebuffersEXT(1, &fbo);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, color_tex, 0);
   
    glGenRenderbuffersEXT(1, &depth_rb);
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depth_rb);
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, 256, 256);

  	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, color_tex, 0);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depth_rb);

  	GLenum status;
    status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    if( status )
    	std::cout << "good" << std::endl;
    else
    	std::cout << "bad" << std::endl;
}

void initPBO()
{
	glGenBuffers(2, pbo);
}

void renderScene(void) 
{
	int index = (index + 1) % 2;
	int nextIndex = (index + 1) % 2;

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1);
	glViewport(0, 0, m_WIDTH, m_HIEGHT);

	glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, pbo[0]);
    glBufferData(GL_PIXEL_PACK_BUFFER_ARB, m_WIDTH * m_HIEGHT * 4, NULL, GL_STREAM_READ);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBegin(GL_TRIANGLES);
		glColor3f(1.f, 0.f, 0.f);
		glVertex3f(0, 0.5, 0.0);
		glColor3f(0.f, 1.f, 0.f);
		glVertex3f(0.5, 0, 0.0);
		glColor3f(0.f, 0.f, 1.f);
		glVertex3f(-0.5, 0, 0.0);
	glEnd();
	
	/* so to read pixels correctly we need to draw view port with specific width and height */
	/* and spacify the formate so color will be mapped to bytes */
	/* for example if width = 256, height = 256, type is unsigned char, and formate RGBA */
	/* will read at every time a pixel stored in 4 channels, so 4 unsigned char will mapped represent it */
	/* so complete from read pixels 256 * 256 * 4 * sizeof(unsigned char) */

	// unsigned char* ptr = (unsigned char*) malloc(sizeof(unsigned char) * m_WIDTH * m_HIEGHT * 4);
	// glReadPixels(0, 0, m_WIDTH, m_WIDTH, GL_BGRA, GL_UNSIGNED_BYTE, ptr);
	// for(int i = 0; i < 256 * 256 * 4; i++)
	// 	std::cout << (int)ptr[i] << std::endl;

	glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, pbo[0]);
	glReadPixels(0, 0, m_WIDTH, m_HIEGHT, GL_BGRA, GL_UNSIGNED_BYTE, BUFFER_OFFSET(0));

	glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, pbo[0]);
	unsigned char* ptr = (unsigned char*)glMapBufferARB(GL_PIXEL_PACK_BUFFER_ARB, GL_READ_ONLY);
	if(ptr)
	{
		for(int i = 0; i < 256 * 256 * 4; i++)
			std::cout << (int)ptr[i] << std::endl;
	}

}


int main(int argc, char **argv) 
{

	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(512, 512);
	glutCreateWindow("Lighthouse3D - GLUT Tutorial");
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	
	glewInit();
  	printf("ARB FBO: %i\n",glewIsSupported("GL_ARB_framebuffer_object"));
  	printf("EXT FBO: %i\n",glewIsSupported("GL_EXT_framebuffer_object"));

	initFBO();
	initPBO();

	// register callbacks
	
	//glutReshapeFunc(changeSize);
	renderScene();
	// enter GLUT event processing loop
	//glutMainLoop();

	return 1;
}