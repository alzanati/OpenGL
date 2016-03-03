TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp
LIBS += -lglfw3 -lGLEW -lX11 -lXrandr -lXinerama
LIBS += -lXi -lXxf86vm -lXcursor -ldl -lGL -lm -lpthread

include(deployment.pri)
qtcAddDeployment()

