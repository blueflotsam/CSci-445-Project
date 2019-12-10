/**************************************************/
/* Name: David Andrew Wilson IV                   */
/* Name: Megan Giesler                            */
/* Name: Cory Haas                                */
/*                                                */
/* HW9                                            */
/*                                                */
/* Global defines across program                  */
/*                                                */
/* CSci 446 / fall 2019                           */
/**************************************************/

#define LIGHTING_TYPE GL_AMBIENT_AND_DIFFUSE

// Libraries
#include <iostream>
#include <fstream> 
#include <string>
#include <stdlib.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Constant colors
#ifndef COLOR
	#define COLOR
	const float BLACK[]	= {0.0, 0.0, 0.0, 1.0};
	const float WHITE[]	= {1.0, 1.0, 1.0, 1.0};
	const float GRAY[]	= {0.5, 0.5, 0.5, 1.0};
	const float RED[]	= {1.0, 0.5, 0.5, 1.0};
	const float GREEN[]	= {0.0, 1.0, 0.0, 1.0};
	const float BLUE[]	= {0.25, 0.5, 1.0, 1.0};
#endif

// RAYGL
#define RAYGL 0
#if RAYGL == 1
	#include "./raygl/raygl.h"
	#include "./raygl/raygldefs.h"
#else
	#ifndef STB_IMAGE_IMPLEMENTATION
		#define STB_IMAGE_IMPLEMENTATION
		#include "stb_image.h"
	#endif
#endif

// functions
void initialize();
void reshape(GLsizei width, GLsizei height);
void myDisplay();
void moveCam3f (float xChange, float yChange, float zChange);
void moveCamForward();
void moveCamBackward();
void rotateCam(int direction, float degrees);
void idleFunction();

/*
		REMOVE LATER
*/

void keyboardKeyPressed(unsigned char key, int xMouse, int yMouse);
void specialKeyPressed(int key, int xMouse, int yMouse);
