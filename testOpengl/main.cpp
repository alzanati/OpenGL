#define VERTEX_SOURCE "/home/prof/workspace/OpenGl/testOpengl/simple_vertext_shader.vert"
#define FRAGMENT_SOURCE "/home/prof/workspace/OpenGl/testOpengl/simple_fragment_shader.frag"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>
#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


GLfloat first_coords[] = { -0.15f, -0.15f, 1.0f,
                           -0.15f,  0.15f, 1.0f,
                            0.15f,  0.15f, 1.0f,
                            0.15f, -0.15f, 1.0f };

GLfloat first_colors[] = { 0.0f,  0.0f, 0.0f,
                           0.25f, 0.0f, 0.0f,
                           0.50f, 0.0f, 0.0f,
                           0.75f, 0.0f, 0.0f };

GLfloat second_coords[] = { -0.30f, -0.30f, 0.0f,
                            -0.30f,  0.30f, 0.0f,
                             0.30f,  0.30f, 0.0f,
                             0.30f, -0.30f, 0.0f };

GLfloat second_colors[] = { 0.0f, 0.0f,  0.0f,
                            0.0f, 0.25f, 0.0f,
                            0.0f, 0.50f,  0.0f,
                            0.0f, 0.75f, 0.0f };

GLfloat third_coords[] = { -0.45f, -0.45f, -1.0f,
                           -0.45f,  0.45f, -1.0f,
                            0.45f,  0.45f, -1.0f,
                            0.45f, -0.45f, -1.0f };

GLfloat third_colors[] = { 0.0f, 0.0f, 0.0f,
                           0.0f, 0.0f, 0.25f,
                           0.0f, 0.0f, 0.50f,
                           0.0f, 0.0f, 0.75f };

GLuint vbo[ 6 ], vao[ 3 ];


char* read_file(const char* filename, GLint* size)
{
   FILE *fp;
   char *buffer;

   /* Read program file and place content into buffer */
   fp = fopen(filename, "r");
   if( fp == NULL )
   {
      perror("Couldn't find the file");
      exit(1);
   }

   fseek( fp, 0, SEEK_END );
   *size = ftell( fp );
   rewind( fp );
   buffer = (char*)malloc(*size+1);
   buffer[*size] = '\0';
   fread( buffer, sizeof(char), *size, fp );
   fclose( fp );

   return buffer;
}

void compile_shader( GLuint shader )
{
    GLint success;
    GLsizei log_size;
    GLchar* log;

    glCompileShader( shader );
    glGetShaderiv( shader, GL_COMPILE_STATUS, &success );

    if( !success )
    {
        glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &log_size );
        log = (GLchar*) malloc( log_size + 1 );
        log[ log_size ] = '\0';
        glGetShaderInfoLog( shader, log_size + 1, NULL, log );

        printf("%s\n", log);
        free(log);
        exit(1);
    }
}

void init_buffers()
{
    /* Create buffers and arrays */
    glGenVertexArrays( 3, vao );
    glGenBuffers( 6, vbo );

    /* Bind Vertex Array 0 */
    glBindVertexArray( vao[ 0 ] );

    /* Bind coords and colors at vertex array 0 */
    glBindBuffer( GL_ARRAY_BUFFER, vbo[ 0 ] );
    glBufferData( GL_ARRAY_BUFFER, 12 * sizeof( GLfloat ), first_coords, GL_STATIC_DRAW );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray( 0 );

    glBindBuffer( GL_ARRAY_BUFFER, vbo[ 1 ] );
    glBufferData( GL_ARRAY_BUFFER, 12 * sizeof( GLfloat ), first_colors, GL_STATIC_DRAW );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray( 1 );

    /* Bind Vertex Array 1 */
    glBindVertexArray( vao[ 1 ] );

    /* Bind coords and colors at vertex array 1 */
    glBindBuffer( GL_ARRAY_BUFFER, vbo[ 2 ] );
    glBufferData( GL_ARRAY_BUFFER, 12 * sizeof( GLfloat ), second_coords, GL_STATIC_DRAW );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray( 0 );

    glBindBuffer( GL_ARRAY_BUFFER, vbo[ 3 ] );
    glBufferData( GL_ARRAY_BUFFER, 12 * sizeof( GLfloat ), second_colors, GL_STATIC_DRAW );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray( 1 );

    /* Bind Vertex Array 2 */
    glBindVertexArray( vao[ 2 ] );

    /* Bind coords and colors at vertex array 2 */
    glBindBuffer( GL_ARRAY_BUFFER, vbo[ 4 ] );
    glBufferData( GL_ARRAY_BUFFER, 12 * sizeof( GLfloat ), third_coords, GL_STATIC_DRAW );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray( 0 );

    glBindBuffer( GL_ARRAY_BUFFER, vbo[ 5 ] );
    glBufferData( GL_ARRAY_BUFFER, 12 * sizeof( GLfloat ), third_colors, GL_STATIC_DRAW );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray( 1 );

    glBindVertexArray( 0 );
}

void init_shaders()
{
    GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );
    GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );

    GLuint program;
    GLint vertextShaderLength;
    GLint fragmentShaderLength;


    char* vertexShaderSource = read_file( VERTEX_SOURCE, &vertextShaderLength );

    char* fragmentShaderSource = read_file( FRAGMENT_SOURCE, &fragmentShaderLength );

    glShaderSource( vertexShader, 1, (const char**) &vertexShaderSource, &vertextShaderLength );
    glShaderSource( fragmentShader, 1, (const char**) &fragmentShaderSource, &fragmentShaderLength );

    compile_shader( vertexShader );
    compile_shader( fragmentShader );

    program = glCreateProgram();
    glBindAttribLocation( program, 0, "in_coords");
    glBindAttribLocation( program, 1, "in_colors");

    glAttachShader( program, vertexShader );
    glAttachShader( program, fragmentShader );

    glLinkProgram( program );
    glUseProgram( program );

}

void reshape( int w, int h )
{
    glViewport( 0, 0, (GLsizei)w, (GLsizei)h );
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glBindVertexArray( vao[ 0 ] );
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glBindVertexArray( vao[ 1 ] );
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glBindVertexArray( vao[ 2 ] );
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glBindVertexArray( 0 );
    glutSwapBuffers();

}

void keyBoard( unsigned char key, int x, int y )
{
    printf( "key :%c, x :%d, y :%d\n", key, x, y );
}

void Mouse( int x, int y, int x1, int y1 )
{
    printf( "x : %d, y : %d, x1: %d, x2: %d\n", x, y, x1, y1 );
}

int main(int argc, char **argv) {

    glutInit( &argc, argv );
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(300, 300);
    glutCreateWindow("Three Squares");
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
          perror("Couldn't initialize GLEW");
          exit(1);
    }

    init_buffers();
    init_shaders();

    glutDisplayFunc( display );
    glutReshapeFunc( reshape );
    glutKeyboardFunc( keyBoard );
    glutMouseFunc( Mouse );
    glutMainLoop();

    return EXIT_SUCCESS;
}
