#ifndef SHADER_HPP
#define SHADER_HPP

#include <glew.h>
#include <GL/gl.h>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <algorithm>
#include <vector>

GLuint LoadShaders( const char * vertex_file_path,
                    const char * fragment_file_path );

#endif
