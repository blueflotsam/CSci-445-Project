/**************************************************/
/* Name: David Andrew Wilson IV                   */
/*                                                */
/* Global defines across program                  */
/*                                                */
/* CSci 446 / fall 2019                           */
/**************************************************/

#define LIGHTING_TYPE GL_AMBIENT_AND_DIFFUSE

// Libraries
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
	const float RED[]		= {1.0, 0.5, 0.5, 1.0};
	const float GREEN[]	= {0.0, 1.0, 0.0, 1.0};
	const float BLUE[]	= {0.25, 0.5, 1.0, 1.0};
#endif

/* image loader
#ifndef STB_IMAGE_IMPLEMENTATION
	#define STB_IMAGE_IMPLEMENTATION
	#include "stb_image.h"
#endif*/

// RAYGL
#define RAYGL 0
#if RAYGL == 1
	#include "./raygl/raygl.h"
	#include "./raygl/raygldefs.h"
#endif

// functions
void initialize();
void myDisplay();
void reshape(GLsizei width, GLsizei height);
void specialKeyPressed(int key, int xMouse, int yMouse);
void moveCamForward();
void moveCamBackward();
void keyboardKeyPressed(unsigned char key, int xMouse, int yMouse);
void idleFunction();
