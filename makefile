all : rotate.exe textures2d.exe

rotate.exe: rotate.o
	g++ cubeRotation.cpp -lGL -lGLU -lglut -o rotate.o

textures2d.exe : textures2d.o
	g++ textures2d.cpp -lGL -lGLU -lglut -o textures2d.o
