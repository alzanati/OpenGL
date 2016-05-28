################################################################################
### Copyrights (c) EPFL/BBP 2015
### Marwan Abdellah <marwanm.abdellah@epfl.ch>
################################################################################

find_package( GLEW REQUIRED )
if( GLEW_FOUND )
    message( STATUS "GLEW Found" )
    include_directories( ${GLEW_INCLUDE_DIR} )
    include_directories( ${GLEW_INCLUDE_DIR}/GL )
    link_libraries( ${GLEW_LIBRARY} )
else( GLEW_FOUND )
    message( FATAL_ERROR "GLEW NOT Found" )
endif( GLEW_FOUND )


include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( GLEW DEFAULT_MSG
    GLEW_INCLUDE_DIR
    GLEW_LIBRARY
)
