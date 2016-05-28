all : rotate.exe textures2d.exe texture3d.exe

rotate.exe: rotate.o
	g++ cubeRotation.cpp -lGL -lGLU -lglut -lSOIL -o rotate.o

textures2d.exe : textures2d.o
	g++ textures2d.cpp -lGL -lGLU -lglut -lSOIL -o textures2d.o

texture3d.exe : texture3d.o
	g++ texture3d.cpp -lGLEW -lGLU -lGL -lglut -o texture3d.o
