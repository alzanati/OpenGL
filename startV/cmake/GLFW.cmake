# Find PkgConfig
find_package( PkgConfig REQUIRED )

# Find glfw3 library
pkg_search_module( GLFW REQUIRED glfw3 )

# GLFW_FOUND set if library exsist
if( GLFW_FOUND )
  message("GLFW Found")
  include_directories(${GLFW_INCLUDE_DIRS})
  link_libraries(${GLFW_STATIC_LIBRARIES})

else( GLFW_FOUND )
    message("GLFW Not Found")
endif( GLFW_FOUND )

# handle the QUIETLY and REQUIRED arguments
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( GLFW DEFAULT_MSG
    GLFW_INCLUDE_DIRS
    GLFW_STATIC_LIBRARIES )
