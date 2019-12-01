/*
File: raygl.h
Author: Kris Zarns
Contact: kzarns@gmail.com
Revision: 36

*/

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glx.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <unistd.h>     // Header file for sleeping.
#include <stdio.h>      // Header file for printf debugging.
#include <stdarg.h>
#include <stdlib.h>
#include <errno.h>
#include <iomanip>
#include <vector>

#ifndef RAYGL_H
#define RAYGL_H

//using namespace std;
using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::stringstream;
using std::setfill;
using std::setw;

typedef unsigned char uchar;

/*************************************************************************
function rayglFrameBegin
	user
	this function takes a fileName parameter
	it creates a file to write ray code to, then writes out the basics
	setup for camera and light
	the file is closed by rayglEnd()
	THIS VERSIUON IS FOR ORTHOGRAPHIC SCENES
*************************************************************************/
void rayglFrameBegin(char* fileName, GLint eyeX, GLint eyeY, GLint eyeZ, GLint centerX,GLint centerY, GLint centerZ, GLint wX, GLint wY, GLint wZ);

/*************************************************************************
function rayglFrameBegin
	user
	this function takes a fileName parameter
	it creates a file to write ray code to, then writes out the basics
	setup for camera and light
	the file is closed by rayglEnd()
	STANDARD VERSION
*************************************************************************/
void rayglFrameBegin(char* fileName);

/*************************************************************************
function raygluPerspective 
	passive
	captures gluPerspective
*************************************************************************/
void raygluPerspective(GLdouble fovy,GLdouble aspect,GLdouble zNear,GLdouble zFar);

/*************************************************************************
function raygluLookAt
	passive
	captures gluLookAt 
*************************************************************************/
void raygluLookAt(GLdouble eyeX,GLdouble eyeY,GLdouble eyeZ,GLdouble centerX, GLdouble centerY,GLdouble centerZ,GLdouble upX,GLdouble upY,GLdouble upZ);

/*************************************************************************
function rayglFrameEnd
	user 
	this function is used to signal the end of a file
*************************************************************************/
void rayglFrameEnd();

/*************************************************************************
function rayglBegin
	passive
	this function is used to implement the standard glBegin and
	set the state of the begin variable to 1, also call getatribs
*************************************************************************/
void rayglBegin(GLenum mode);

/*************************************************************************
function rayglEnd 
	passive
	this function is used ot implement the standard glEnd and
	set the state of the begin variable to 0, writes everything out
*************************************************************************/
void rayglEnd();

/*************************************************************************
function raygluDisk
	passive
	this function implements gluDisk 
*************************************************************************/
void raygluDisk(GLUquadric *quad, GLdouble inner, GLdouble outer, GLint slices, GLint loops);

/*************************************************************************
function raygluSphere
	passive
	this function implements gluSphere
*************************************************************************/
void raygluSphere(GLUquadric *quad, GLdouble radius, GLint slices, GLint stacks);

/*************************************************************************
function raygluCylinder
	passive
	this function implements gluCylinder
*************************************************************************/
void raygluCylinder(GLUquadric *quad, GLdouble base, GLdouble top, GLdouble height, GLint slices, GLint stacks);

/*************************************************************************
function rayglutSolidSphere
	passive
	this function implements gluSphere
*************************************************************************/
void rayglutSolidSphere(GLdouble radius, GLint slices, GLint stacks);

/*************************************************************************
function rayglutSolidCube
	passive
	this function implements glutSolidCube
*************************************************************************/
void rayglutSolidCube(GLdouble size);

/*************************************************************************
function rayglutSolidCone
	passive
	this function implements glutSolidCone
*************************************************************************/
void rayglutSolidCone(GLdouble base, GLdouble height, GLint slices, GLint stacks);

/*************************************************************************
function rayglutSolidTorus
	passive
	this function implements glutSolidTorus
*************************************************************************/
void rayglutSolidTorus(GLdouble innerRadius, GLdouble outerRadius, GLint nsides,GLint rings);

/*************************************************************************
function getAtribs
	internal
	this function writes colors, coord transformations, etc to a string
	getatribs is not begin/end safe
*************************************************************************/
void getAtribs();

/*************************************************************************
function writeAtribs
	internal
	this function writes colors, textures, and materials to a file 
	note a getatribs must be called prior to this
	writeAtribs is begin/end safe
**************************************************************************/
void writeAtribs();

/*************************************************************************
function writeMatrix
	internal
	this function writes coord transformations to a file
*************************************************************************/
void writeMatrix();

/*************************************************************************
function setRenderPov
	user
	allows user to turn file writing on and off.
*************************************************************************/
void setRenderPov(GLboolean on);

/*************************************************************************
function getRenderPov
	user
	allows user to check if file writing is on or off
*************************************************************************/
GLboolean getRenderPov();

/*************************************************************************
function rayglFadeDistance
	user
	adjusts the distance at which light begins to fade
	should be relative to scene size
	global across all lights
	default is 100
*************************************************************************/
void setFadeDistance(GLfloat distance);

/*************************************************************************
function rayglFadePower
	user
	adjusts the power with which light fades
	good 1.4 - 2.0 real
	global across all lights
	default is 1.4
*************************************************************************/
void setFadePower(GLfloat power);

/*************************************************************************
function rayglInject
	user
	writes the parameter char array to the povray SDL file.
	strictly for use by people who have studied povray.
	I do not support the use of this function and will not assist anyone
  	in using it.  Not appropriate if the file hasn't been opened.
	Also useful for commenting pov files.  standard // / * style comments
	This function will segfault if called while the file is closed
*************************************************************************/
void rayglInject(char* inject);

/*************************************************************************
function stringAtribs
 	internal
	for debugging
 ************************************************************************/
void stringAtribs(string sret);

/*************************************************************************
function rayglVertex3i
	passive 
	this function is used to implement the standard glvertex3i and 
	to add vertices to objects in SDL

*************************************************************************/
void rayglVertex3i(GLint localX,GLint localY,GLint localZ);

/*************************************************************************
function rayglVertex3f
	passive
	this function is used to implement the standard glvertex3i and 
	to add vertices to objects in SDL
*************************************************************************/
void rayglVertex3f(GLfloat localX,GLfloat localY,GLfloat localZ);

/*************************************************************************
function rayglVertex3d 
	passive
	this function is used to implement the standard glvertex3i and 
	to add vertices to objects in SDL
*************************************************************************/
void rayglVertex3d(GLdouble localX,GLdouble localY,GLdouble localZ);

/*************************************************************************
function rayglTexImage2D 
	passive
	captures texImage2D, associates image objects with texture names
*************************************************************************/
void rayglTexImage2D(GLenum target, GLint level, GLint internalFormat, GLint width, GLint height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);

/*************************************************************************
function raygluBuild2DMipmaps 
	passive
	captures build2DMipmaps, associates image objects with texture names
*************************************************************************/
void raygluBuild2DMipmaps(GLenum target, GLint internalFormat, GLint width, GLint height, GLenum format, GLenum type, const GLvoid *pixels);

/*************************************************************************
function rayglTexture
	user
	this function adjusts the texture flag so that the user may enter pov
	textures by string.  Texture char * here is the string to be added to
	file.  Contributed by Rick Fillion
 * **********************************************************************/
void rayglTexture(char* texture);

/*************************************************************************
function rayglDisableTexture
	user
	this function adjusts the texture flag so that the opengl texture is
	used.  Contributed by Rick Fillion
 * **********************************************************************/
void rayglDisableTexture();

/*************************************************************************
function rayglScaleTexture
	user
	this function adjusts the scale of a texture for non-unit size polygons
	default value is 1,1,1.  This is global, so reset it for each object as
	needed.

	note the normal of the last vertex in an object adjusts the orientation
	of it's texture.  improperly oriented texture are usualy black
*************************************************************************/
void rayglScaleTexture(GLdouble texX, GLdouble texY, GLdouble texZ);

/*************************************************************************
function rayglTranslateTexture
	user
	this function adjusts the tracking of a texture.
	default value is 0, 0, 0.  This is global, so reset it for each
	object as needed.

	note the normal of the last vertex in an object adjusts the orientation
	of it's texture.  improperly oriented texture are usualy black
*************************************************************************/
void rayglTranslateTexture(GLdouble texX, GLdouble texY, GLdouble texZ);

/*************************************************************************
function rayglRotateTexture
	this function adjusts the rotation of a texture
	Hopefully this will be able to be automated later.
	default value is 0,0,0.  This is global, so reset it for each object as
	needed.
*************************************************************************/
void rayglRotateTexture(GLdouble texX, GLdouble texY, GLdouble texZ);

/**************************************************************************
function rayglTextureType
	user
	adjusts the mapping type of the texture.  Default is 0, planar.  
	1 = spherical
	2 = cylindrical
	3,4 undefined
	5 = torus
	again, this is global, change on a per texture basis as needed.

	note the normal of the last vertex in an object adjusts the orientation
	of it's texture.  improperly oriented texture are usualy black
**************************************************************************/
void rayglTextureType(GLint texTypeIn);

//the image object to store texture information
//consider moving shape, translate, scale into this
struct Image {
	unsigned long sizeX;
	unsigned long sizeY;
	GLuint texName;
	char* texFileType;
	char* texFileName;
	uchar* data;
	void toString(){cout<<"name: "<<texFileName<<" texName: "<<texName<<" Size x,y "<<sizeX<<","<<sizeY<<endl;}
};

/**************************************************************************
function imageLoad
	user
	required to pass textures between opengl and povray
	this function loads ppm images into a image object and then adds the
	object to an internal array fo images for later lookup.
	this grabs the dimensions, filename, and data.  The texture number
	is associated with the image later by comparing the pointer to the image
	data with the one passed to glTexImage2d

	new CustomLoadedTexture alternative!!!
**************************************************************************/
int imageLoad(char *filename, Image *image);

/*************************************************************************
function rayglCustomLoadedTexture
	user
	support for custom loaders for non-ppm type images and such
	pass in an image struct with all the fields set and it will be added to
	the image vector
*************************************************************************/
void rayglCustomLoadedTexture(Image* image);

//some code used in image load
uchar *PGM_FILE_READ(char *filename, int *targetW, int *targetH, int *color);
void PGM_FILE_WRITE(char *filename, uchar data[], int targetW, int targetH, int color);


#endif
