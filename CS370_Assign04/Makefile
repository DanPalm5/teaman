#
# Makefile for a CS 101 lab or programming assignment.
# Do not modify this file!!!
#

# FIgure out which OS
OS = $(shell uname -s)

# Mac OSX
ifeq ($(OS),Darwin)
OSDEF = OSX
LIBS = -lSOIL -framework GLUT -framework OpenGL -framework Cocoa -framework CoreFoundation
else
OSDEF = Linux
LIBS = -lGLEW -lglut -lSOIL -lGLU -lGL
endif

CXXFLAGS = -D$(OSDEF) -g -Wall

SRC = TeaMan.cpp
OBJ = $(SRC:.cpp=.o)
EXE = $(SRC:.cpp=.exe)

$(EXE) : clean $(OBJ)
	$(CXX) -o $@ $(OBJ) $(LIBS)

submit :
	cd .. && make submit

# This is just a dummy target to force other targets
# to always be out of date.
nonexistent :

# Remove generated files.
clean : 
	rm -f *.o *.exe collected-files.txt submit.properties solution.zip
