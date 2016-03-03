#include <stdio.h>
#include <GL/freeglut.h>
#include <stdlib.h>

void display()
{
	glClear( GL_COLOR_BUFFER_BIT );
	printf("Displaying the window\n");
	glutSwapBuffers();
}

void reshape( int w, int h )
{
	printf("New dimensions: %d %d\n", w, h);
}

int main( int argc, char** argv )
{
	/* setUp Window */
	glutInit( &argc, argv );
	glutInitWindowSize( 500, 150);
	glutInitWindowPosition( 150, 200 );
	glutInitDisplayMode( GLUT_RGB|GLUT_DOUBLE );
	glutCreateWindow( " EL7L " );
	
	/* event handling */
	glutDisplayFunc( display );
	glutReshapeFunc( reshape );
	glClearColor( 1.0, 0.0, 1.0, 1.0 );
		
	/* run forever */
	glutMainLoop();
	
	return EXIT_SUCCESS;
}
