################################################################################
### Copyrights (c) EPFL/BBP 2015
### Marwan Abdellah <marwanm.abdellah@epfl.ch>
################################################################################

find_package( GLUT REQUIRED )
if( GLUT_FOUND )
    message( STATUS "GLUT Found" )
    include_directories( ${GLUT_INCLUDE_DIR} )
    include_directories( ${GLUT_INCLUDE_DIR}/GL )
    link_libraries( ${GLUT_LIBRARY} )
else( GLUT_FOUND)
    message( FATAL_ERROR "GLUT NOT Found" )
endif( GLUT_FOUND )

include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( GLUT DEFAULT_MSG
    GLUT_INCLUDE_DIR
    GLUT_LIBRARY
)
