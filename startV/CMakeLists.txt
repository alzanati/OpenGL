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

set(MY_HEADERS "headers")

### src directory
set( MY_SOURCES "src" )

include_directories(${CURRENT_DIR}/headers)
include_directories(${CURRENT_DIR}/src)

### library name
set( MY_LIBRARY "libV" )

### set source files
set( SOURCE_FILES
${MY_SOURCES}/main.cpp
${MY_SOURCES}/shaders.cpp
${MY_SOURCES}/Texture.cpp
)

set( HEADER_FILES
${MY_HEADERS}/shaders.hpp
${MY_HEADERS}/Texture.hpp
${MY_HEADERS}/transform.vert
${MY_HEADERS}/simple.frag
${MY_HEADERS}/simple.vert
${MY_HEADERS}/texture.frag
)

# add a shared library
add_library(${MY_LIBRARY} SHARED ${SOURCE_FILES})

# generate executable file with program name
add_executable(${PROGRAM_NAME} ${SOURCE_FILES} ${HEADER_FILES})

# link our project to generated library
target_link_libraries(${PROGRAM_NAME} ${MY_LIBRARY} -lSOIL )
