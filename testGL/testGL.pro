TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    utilitygl.cpp

LIBS += -lGL -lGLU -lglut

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    utilitygl.h



