# Set the minimum version of CMake to be able to run the application
cmake_minimum_required( VERSION 2.8 )

### set program name
set( PROGRAM_NAME "startV" )

### Create project with program name
project( ${PROGRAM_NAME} )

# get current directory
set( CURRENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} )

### Add CMake modules
set( CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake )

### Dependencies
include( OpenGL )
include( GLEW )
include( GLFW )
include( GLM )

### shaders
set( HEADER_FILES shaders.hpp simple.vert simple.frag)

### src directory
set( MY_SOURCES "src" )

### library name
set( MY_LIBRARY "libV" )

### set source files
set( SOURCE_FILES main.cpp shaders.cpp)

# add a shared library
add_library( ${MY_LIBRARY} SHARED ${SOURCE_FILES} )

# generate executable file with program name
add_executable( ${PROGRAM_NAME} ${SOURCE_FILES} ${HEADER_FILES})

# link our project to generated library
target_link_libraries( ${PROGRAM_NAME} ${MY_LIBRARY} )
