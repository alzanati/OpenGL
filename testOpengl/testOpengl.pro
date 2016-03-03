TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lGL
LIBS += -lglut
LIBS += -lGLEW

SOURCES += main.cpp

include(deployment.pri)

qtcAddDeployment()

