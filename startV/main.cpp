#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include "shaders.hpp"

float timed = 0.f;

int main(int argc, char** argv)
{
    // setup glfw
    if(!glfwInit())
    {
        fprintf(stderr, "Glfw not init\n");
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window =  glfwCreateWindow(680, 480, "startV", NULL, NULL);
    if(!window)
    {
        fprintf( stderr, "Failed to open GLFW window. If you "
                         "have an Intel GPU, they are not 3.3 compatible. Try"
                         "the 2.1 version of the tutorials.\n" );
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // setup glew
    glewExperimental = true;
    if(glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Faild to init glew\n");
        glfwTerminate();
        return EXIT_FAILURE;
    }

    GLuint program = LoadShaders("/home/mohamed/workspace/OpenGl/startV/simple.vert",
                                 "/home/mohamed/workspace/OpenGl/startV/simple.frag");

    // transform feedback
    const char* varying[] = {"v_position"};
    glTransformFeedbackVaryings(program, 1, varying, GL_INTERLEAVED_ATTRIBS);
    glLinkProgram(program);
    glUseProgram(program);

    GLuint vertex_buffer;
    GLuint color_buffer;
    GLuint feedBack_buffer;
    glGenBuffers(1, &vertex_buffer);
    glGenBuffers(1, &color_buffer);
    glGenBuffers(1, &feedBack_buffer);

    GLuint vertex_array[2];
    glGenVertexArrays(1, vertex_array);

    const GLfloat vertices[] =
    {
        -0.5f, -0.5f, 0.f,
        0.5f, -0.5f, 0.f,
        0.5f, 0.5f, 0.f,
    };
    const GLfloat colors[]=
    {
        0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
    };

    // bind buffers
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    /* bind to VAO */
    /* so you can bind all vertexs to one vao then use it in any location */
    glBindVertexArray(vertex_array[0]);

    GLint position_attrib = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(position_attrib);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glVertexAttribPointer(position_attrib, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    //should bind to shader attribute

    GLint color_attrib = glGetAttribLocation(program, "color_in");
    glEnableVertexAttribArray(color_attrib);
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
    glVertexAttribPointer(color_attrib, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    //should bind to shader attribute

    //tbo /* glEnable(GL_RASTERIZER_DISCARD); disable rendering */
    glBindBuffer(GL_ARRAY_BUFFER, feedBack_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), NULL, GL_STATIC_READ);



    glm::mat4 trans;
    trans = glm::translate(trans, glm::vec3(0.5f, -0.3f, 0.0f));

    glm::mat4 scale;
    scale = glm::scale(scale, glm::vec3(0.5f, 1.f, 0.5));
   // glm::mat4 model = scale * trans * rotate;


    // Perform feedback transform
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, position_attrib, feedBack_buffer);

    /* unbind to this vertex array */
    glBindVertexArray(0);

    /* start rendering */
    while(!glfwWindowShouldClose(window))
    {
        // Clear the screen to black
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // uniform model matrix
        // transformations
        glm::mat4 rotate;
        rotate = glm::rotate(rotate, timed * glm::degrees(20.f), glm::vec3(0.0f, 0.0f, 1.0f));
        GLint uniTrans = glGetUniformLocation(program, "trans");
        glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(rotate));

        timed += 0.0001;
        glBindVertexArray(vertex_array[0]);
        glBeginTransformFeedback(GL_TRIANGLES);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        glEndTransformFeedback();

        glfwSwapBuffers(window);
        glfwPollEvents();
        glFlush();
    }

    // dispaly data
    float* data = (float*) malloc(sizeof(vertices));
    glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(vertices), data);

    for(int i = 0; i < sizeof(vertices) / sizeof(float); i++)
        std::cout << data[i] << std::endl;

    //clean up the memories
    glDisableVertexAttribArray(position_attrib);
    glDisableVertexAttribArray(color_attrib);
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &color_buffer);
    glDeleteVertexArrays(1, vertex_array);
    glDeleteProgram(program);

    // Close OpenGL window and terminate GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
