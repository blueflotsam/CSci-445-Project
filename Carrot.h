
#include <GL/gl.h>
//#include <raygl.h>
//#include <raygldefs.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <GL/freeglut.h>
#include <GL/glut.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "libIO.h"
#include <iostream>
#include <fstream>

class Carrot{

float d[3] = {0.1, 0.1, 0.1};
float times=0;//what time in the animation the object is in, incriments by one for each movement
float ambientStrength=.1;
GLuint ConeMap;
bool animate=false;
bool upwards=true;
float xpos=0;
float ypos=0;
float zpos=0;
GLUquadricObj *sphere =gluNewQuadric();

void carrotPos(float x, float y, float z){
xpos=x;
ypos=y;
zpos=z;
}




void drawTriangle(float x, float y, float z, float y1, float z1){
	float midz=(z+z1)/2;
	float midy=(y+y1)/2;
	glBegin (GL_TRIANGLES);
	glVertex3f( 0, y, z );
	glVertex3f( 0,y1 , z);
	glVertex3f( x, midy, midz);

	glVertex3f( 0, y, z );
	glVertex3f( 0,y , z1);
	glVertex3f( x, midy, midz);

	glVertex3f( 0, y1, z1 );
	glVertex3f( 0,y , z1);
	glVertex3f( x, midy, midz);

	glVertex3f( .4, y1, z1 );
	glVertex3f( .4,y1 , z);
	glVertex3f( x, midy, midz);
	glEnd;
}

void drawCarrot (void)
{
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
glTranslatef(xpos,ypos,zpos);
glTranslatef(.01*times,0,0);
glTranslatef(-.8,0,0);
glDisable(GL_TEXTURE_2D);
gluQuadricTexture(sphere, GL_TRUE);
glBindTexture(GL_TEXTURE_2D, ConeMap);
glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

glColor3f(.3215,.4196,.1765);//green
GLfloat green[] = { .3215,.4196,.1765, 1 };
glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, green);
//glMaterialfv(GL_FRONT, GL_EMISSION, green);
drawTriangle(-7,.5,.5,-.5,-.5);
drawTriangle(-5,-.5,.5,-1.5,-.5);
drawTriangle(-5,.5,.5,1.5,-.5);
drawTriangle(-5,.5,-.5,-.5,-1.5);
drawTriangle(-5,.5,.5,-.5,1.5);
glEnd();
float radius =2;
float percent=.1;
float sharpness=16;
float decrease=percent*radius;
glColor3f(0.929411765,0.568627451,0.011764706);//orange
GLfloat orange[] = { 0.929411765,0.568627451,0.011764706, 1 };
glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, orange);
//glMaterialfv(GL_FRONT, GL_SPECULAR, orange);
glEnable(GL_TEXTURE_2D);
for(float i=0; i<9;i++){
if(i==0);
else if(i<4){
    glTranslatef(0.003*times,.01*times,0);
}
else if(i>4){
    glTranslatef(-.005*times,-.01*times,0);
}

if(i!=0)
glTranslatef(radius,0,0);
radius-=decrease;
gluSphere(sphere,radius, sharpness, sharpness);
decrease=radius*percent;
}

    glEnd();

    //animation part
    if(animate){
    if(upwards)
    times+=.01;
    else
    times-=.01;
    if(times>=100)
        upwards=false;
    else if(times<=0)
        upwards=true;
    glutPostRedisplay();
    
    }

}



};
