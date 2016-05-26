all : rotate.exe

rotate.exe : rotate.o
	g++ cubeRotation.cpp -lGL -lGLU -lglut -o rotate.o
