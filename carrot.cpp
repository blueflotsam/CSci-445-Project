  /**************************************************/
  /* Name: Cory Haas                                */
  /*                                                */
  /* THis code draws a textured carrot              */
  /*                                                */
  /* CSci 446 / fall 2019                           */
  /**************************************************/

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

#ifndef _pgmIO_
#define _pgmIO_
unsigned char *PGM_FILE_READ_1D(char *, int *, int *, int *);
#endif

#define  RADDEG  57.29577951f
float XUP[3] = {1,0,0}, XUN[3] = {-1, 0, 0},
      YUP[3] = {0,1,0}, YUN[3] = { 0,-1, 0},
      ZUP[3] = {0,0,1}, ZUN[3] = { 0, 0,-1},
      ORG[3] = {0,0,0};
GLUquadricObj *sphere =gluNewQuadric();
GLfloat viewangle = 0, tippangle = 0, traj[120][3];

GLfloat d[3] = {0.1, 0.1, 0.1};
GLfloat times=0;//what time in the animation the object is in, incriments by one for each movement
GLfloat  xAngle = 0.0, yAngle = 0.0, zAngle = 0.0;
float ambientStrength=.1;
GLuint ConeMap;
void TEXTURE_MODEL(void){

   int w, h, c;
   //char arr[13]={'.','/','c','a','r','r','o','t','2','.','p','p','m'};
   char *arr;
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   std::ifstream images;
   images.open("./carrot2.ppm");
   //unsigned char *data= PGM_FILE_READ_1D(arr, &w, &h, &c); 
   glGenTextures(1, &ConeMap);
   glBindTexture(GL_TEXTURE_2D, ConeMap);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT); 
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, images.get());
   //free(data);
}

void Special_Keys (int key, int x, int y)
{
    switch (key) {

       case GLUT_KEY_LEFT :  viewangle -= 5;  break;
       case GLUT_KEY_RIGHT:  viewangle += 5;  break;
       case GLUT_KEY_UP   :  tippangle -= 5;  break;
       case GLUT_KEY_DOWN :  tippangle += 5;  break;

    }

    glutPostRedisplay();
}

bool animate=false;
bool upwards=true;
void Mouse(int button, int state, int x, int y){
    if(state==GLUT_DOWN){
        switch(button){
            case GLUT_LEFT_BUTTON : if(animate)animate=false; else animate=true;  break;
            case GLUT_RIGHT_BUTTON: animate=false; break;
            case GLUT_MIDDLE_BUTTON:  if(upwards)upwards=false; else upwards=true;animate=true; break;

        }
    }

    glutPostRedisplay();
}

void drawTriangles(float x, float y, float z, float y1, float z1){
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


void myDisplay (void)
{
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
drawTriangles(-7,.5,.5,-.5,-.5);
drawTriangles(-5,-.5,.5,-1.5,-.5);
drawTriangles(-5,.5,.5,1.5,-.5);
drawTriangles(-5,.5,-.5,-.5,-1.5);
drawTriangles(-5,.5,.5,-.5,1.5);
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



void redraw (void)
{
    int v;

    glClear  (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable (GL_DEPTH_TEST);


    glLoadIdentity ();
    glTranslatef (0, 0, -3);
    glRotatef (tippangle, 1,0,0);  
    glRotatef (viewangle, 0,1,0);

    glPushMatrix ();
       glTranslatef (d[0], d[1], d[2]);  
       glScalef (0.2, 0.2, 0.2);
       glRotatef (zAngle, 0,0,1);
       glRotatef (yAngle, 0,1,0);
       glRotatef (xAngle, 1,0,0);
       myDisplay ();//draws carrot
    glPopMatrix ();

    glutSwapBuffers();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize     (750, 750);
    glutInitWindowPosition (0, 0);
    glutInitDisplayMode    (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow ("hw6");
   GLfloat light_intesity[] = { .05, .05, .05, .5 };
   GLfloat light[]={1,1,1, 100};
   GLfloat pos[]={100,100,100,1};
   glShadeModel (GL_SMOOTH);
   glLightfv(GL_LIGHT1, GL_DIFFUSE, light_intesity);
   glLightfv(GL_LIGHT1, GL_POSITION, pos);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT1);
   glEnable(GL_DEPTH_TEST);
	
    glutDisplayFunc  (   redraw   );
    glutSpecialFunc  (Special_Keys);
    glutMouseFunc(Mouse);
    //glCullFace(GL_BACK);
    TEXTURE_MODEL();

    glClearColor (0.1, 0.0, 0.1, 1.0);

    glMatrixMode   (GL_PROJECTION);
    gluPerspective (60, 1.5, 1, 10);
    glMatrixMode   (GL_MODELVIEW);
    glutMainLoop   ();

    return 0;
}
