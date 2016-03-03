#define VERTEX_SOURCE "/home/prof/workspace/OpenGl/testOpenGl3/vertext_shader.vert"
#define FRAG_SOURCE "/home/prof/workspace/OpenGl/testOpenGl3/fragment_shader.frag"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include <stdlib.h>



float vertices[6] = { 0.0f,  0.5f,
                      0.5f, -0.5f,
                     -0.5f, -0.5f };

GLuint vbo, vao;
GLuint prog;

char* read_file(const char* filename, GLint* size)
{
   FILE *handle;
   char *buffer;

   /* Read program file and place content into buffer */
   handle = fopen(filename, "r");
   if( handle == NULL )
   {
      perror("Couldn't find the file");
      exit(1);
   }

   fseek(handle, 0, SEEK_END);
   *size = ftell(handle);
   rewind(handle);
   buffer = (char*)malloc(*size+1);
   buffer[*size] = '\0';
   fread(buffer, sizeof(char), *size, handle);
   fclose(handle);

   return buffer;
}

void compile_shader(GLint shader) {

   GLint success;
   GLsizei log_size;
   GLchar *log;

   glCompileShader(shader);
   glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
   if (!success)
   {
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_size);
      log = (char*) malloc(log_size+1);
      log[log_size] = '\0';
      glGetShaderInfoLog(shader, log_size+1, NULL, log);
      printf("%s\n", log);
      free(log);
      exit(1);
   }
}

void init_shaders(void)
{

   GLuint vs, fs;
   char *vs_source, *fs_source;
   GLint vs_length, fs_length;

   vs = glCreateShader(GL_VERTEX_SHADER);
   fs = glCreateShader(GL_FRAGMENT_SHADER);

   vs_source = read_file(VERTEX_SOURCE, &vs_length);
   fs_source = read_file(FRAG_SOURCE , &fs_length);

   glShaderSource(vs, 1, (const char**)&vs_source, &vs_length);
   glShaderSource(fs, 1, (const char**)&fs_source, &fs_length);

   compile_shader(vs);
   compile_shader(fs);

   prog = glCreateProgram();

   glAttachShader(prog, vs);
   glAttachShader(prog, fs);

   glLinkProgram(prog);
}

int main()
{
    printf("Compiled against GLFW %i.%i.%i\n",
           GLFW_VERSION_MAJOR,
           GLFW_VERSION_MINOR,
           GLFW_VERSION_REVISION);

    if( !glfwInit() )
    {
        fprintf( stderr, "Faild To Initialize glfw");
        return EXIT_FAILURE;
    }

    glfwWindowHint( GLFW_SAMPLES, 4 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE) ;
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_VISIBLE, GL_TRUE );

    /* Create Window */
    GLFWwindow* window;
    window = glfwCreateWindow( 500, 500, "First GLFW", NULL, NULL );
    if( window == NULL )
    {
        fprintf( stderr, "Faild Create Window");
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent( window );

    glewExperimental = true;
    if( glewInit() != GLEW_OK )
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return EXIT_FAILURE;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    /* start shading */

    init_shaders();
    GLint uniformMat = glGetUniformLocation( prog, "trans" );

    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    glGenBuffers( 1, &vbo );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW );

    GLuint vertexAttrib = glGetAttribLocation( prog, "in_coords");
    printf("in_coords : %d\n", vertexAttrib );

    glm::mat4 trans;
    trans = glm::translate( trans, glm::vec3( 0.5f, 0.5f, 0.5f ));
    //trans = glm::scale( trans, glm::vec3( 0.5f, 0.5f, 0.5f ));
    //trans = glm::rotate(trans, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    glm::vec4 result = trans * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    printf("%f, %f, %f\n", result.x, result.y, result.z);


    glfwSetInputMode( window, GLFW_STICKY_KEYS, GL_TRUE );
    while ( !glfwWindowShouldClose( window ))
    {
        glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT );

        glUseProgram(prog);
        glUniformMatrix4fv( uniformMat, 1, GL_FALSE, glm::value_ptr( trans ));

        glVertexAttribPointer( vertexAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0 );
        glEnableVertexAttribArray( vertexAttrib );

        glDrawArrays( GL_TRIANGLES, 0, 3 );

        glfwSwapBuffers( window );
        glfwPollEvents();
    }

    if( glfwGetKey( window, GLFW_KEY_ESCAPE ) != GLFW_PRESS )
            glfwSetWindowShouldClose( window, GL_TRUE );

    // Cleanup VBO
    glDeleteBuffers( 1, &vbo);
    glDeleteVertexArrays( 1, &vao );
    glDeleteProgram(prog);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

}

