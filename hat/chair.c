/****************************************
 * Name: Megan Giesler                  *
 * This draws a textured hat            *
 * CSci 446 / fall 2019 	  		  	*
 ****************************************/
 
/************************************************************
 * The controls to move the object:							*	
 * Page Up/Page Down keys will flip the object around		*
 * Up/Down arrow keys will rotate the object in a circle	*
 * Left and Right keys will spin the object 	  		  	*
 ************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#include "cone.h"
#include "libIO.h"


/******************************************************
 * Global Variables							  		  *
 ******************************************************/
GLUquadricObj *bodyPart;
double yy = 0; 
double xx = 85;
double zz = 270;
int thighR = 0, thighL = 0, shinR = 0, shinL = 0, moveLegs = 180, moveArms = 0, larmL = 0, larmR = 0;
int uarmL = -20;
int uarmR = 20;
int keepMoving = 0;
int stopMoving = 100;

GLfloat MatAmbient[] = {1.0, 0.5, 0.5, 1.0};

GLfloat MatDiffuse[]     = {1.0, 0.9, 0.3, 1.0};
GLfloat Light0Diffuse[]  = {1.0, 1.0, 1.0, 1.0};
GLfloat Light0Position[] = {100.0, 100.0,  100.0, 1.0};

GLfloat lightColor[] = {0, 0 , 0, 1}; 
GLfloat lightDirection[] = {0.5, 0.5, 0.5, 0.5}; 
GLfloat white[] = {1.0, 1.0, 1.0, 1.0};
GLfloat black[] = {0.05, 0.05, 0.05, 1.0};

unsigned *read_texture(char *name, int *width, int *height, int *components);

/******************************************************
 * myInIt									  		  *
 ******************************************************/
void SET_LIGHTS(void) 
{

       glLoadIdentity();
       glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
       glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_TRUE);
       // Ambient "light source".
       glMaterialfv(GL_FRONT, GL_AMBIENT, MatAmbient);
       // Diffuse light source.
       glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MatDiffuse);
       glLightfv(GL_LIGHT0, GL_DIFFUSE,  Light0Diffuse);
       glLightfv(GL_LIGHT0, GL_POSITION, Light0Position);

}


/******************************************************
 * myInIt									  		  *
 ******************************************************/
void myInIt() 
{
   glClearColor(1, 1, 1, 1);
   glPointSize(1);
   glOrtho(0.0, 1000.0, 0.0, 1000.0, -5000, 5000);                  
   glLoadIdentity();                             
   glClear(GL_COLOR_BUFFER_BIT);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LEQUAL);
}

void DrawBody()
{
	//Draw Body
		gluCylinder(bodyPart, 5.0, 5.0, 10.0, 30, 20);

	//Draw Rim
		gluDisk(bodyPart, 5.0, 7.5, 30, 10);
		glTranslatef(0.0, 0.0, 1.0);
		gluDisk(bodyPart, 0.0, 5.0, 30, 10);

	//Draw Base
		glTranslatef(0.0, 0.0, 9.0);
		gluDisk(bodyPart, 0.0, 5.0, 30, 10);
}

void DrawEyes()
{
	//Draw Left Eye
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTranslatef(4.5, -2.0, 3.5);
		gluSphere(bodyPart, 1.0, 15, 15);
		
	//Draw Right Eye
		glTranslatef(0.4, 3.5, 0.0);
		gluSphere(bodyPart, 1.0, 15, 15);

	//Draw Pupils
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
		glTranslatef(0.4, 0.0, 0.0);
		gluSphere(bodyPart, .75, 15, 15);
		glTranslatef(-0.4, -3.5, 0.0);
		gluSphere(bodyPart, .75, 15, 15);
	
}

void DrawLeftArm()
{
	//Shoulder
		glTranslatef(0.0, -5.0, 3.0);
		gluSphere(bodyPart, 1.0, 25, 25);
	//Bicep 
		glRotatef(uarmL, -180.0, 10.0, 0.0);
		gluCylinder(bodyPart, 1.0, 1.0, 5.0, 30, 10);
	//Elbow
        glTranslatef(0.0, 0.0, 5.0);
		gluSphere(bodyPart, 1.0, 15, 15); 
	//Forearm
		glRotatef(larmL, -50.0, 10.0, 0.0);
		gluCylinder(bodyPart, 1.0, 1.0, 6.0, 30, 30);
		gluDisk(bodyPart, 0.0, 1.0, 30, 10);
}

void DrawRightArm()
{
	//Shoulder
		glTranslatef(0.0, 5.0, 3.0);
		gluSphere(bodyPart, 1.0, 15, 15);
	//Bicep 
		glRotatef(uarmR, -180.0, 10.0, 0.0);
		gluCylinder(bodyPart, 1.0, 1.0, 5.0, 30, 10);
	//Elbow
		glTranslatef(0.0, 0.0, 5.0);
		gluSphere(bodyPart, 1.0, 15, 15); 
	//Forearm
		glRotatef(larmR, -50.0, 10.0, 0.0);
		gluCylinder(bodyPart, 1.0, 1.0, 6.0, 30, 30);
		gluDisk(bodyPart, 0.0, 1.0, 30, 10);
}

void DrawLeftLeg()
{
	//Hip
		glTranslatef(0.0, -2.5, 10);
		gluSphere(bodyPart, 1.0, 15, 15);
	//Thigh
        glRotatef(thighL,0.0, 1.0, 0.0);
		gluCylinder(bodyPart, 1.0, 1.0, 6.5, 30, 10);
	//Shin
		glTranslatef(shinR, 0.0, 2.5);
		gluCylinder(bodyPart, 1.0, 1.0, 7.0, 30, 30);
		glTranslatef(shinR, 0.0, 7.0);
		gluDisk(bodyPart, 0.0, 1.0, 30, 10);
}
void DrawRightLeg()
{
	//Hip
		glTranslatef(0.0, 5.0, -9.5);
		gluSphere(bodyPart, 1.0, 15, 15);
	//Thigh
		glRotatef(thighR,0.0, 1.0, 0.0);
		gluCylinder(bodyPart, 1.0, 1.0, 4.5, 30, 10);
	//Shin
		glTranslatef(shinL, 0.0, 2.5);
		gluCylinder(bodyPart, 1.0, 1.0, 7.0, 30, 30);
		glTranslatef(shinL, 0.0, 7.0);
		gluDisk(bodyPart, 0.0, 1.0, 30, 10);
}

void DrawChair()
{
	glBegin(GL_QUADS);
//Front
     glNormal3f(0.0f, 0.0f, 1.0f);
     glVertex3f(-2.0f, -0.2f, 2.0f);
     glVertex3f(2.0f, -0.2f, 2.0f);
     glVertex3f(2.0f, 0.2f, 2.0f);
     glVertex3f(-2.0f, 0.2f, 2.0f);
     
     //Right
     glNormal3f(1.0f, 0.0f, 0.0f);
     glVertex3f(2.0f, -0.2f, -2.0f);
     glVertex3f(2.0f, 0.2f, -2.0f);
     glVertex3f(2.0f, 0.2f, 2.0f);
     glVertex3f(2.0f, -0.2f, 2.0f);
     
     //Back
     glNormal3f(0.0f, 0.0f, -1.0f);
     glVertex3f(-2.0f, -0.2f, -2.0f);
     glVertex3f(-2.0f, 0.2f, -2.0f);
     glVertex3f(2.0f, 0.2f, -2.0f);
     glVertex3f(2.0f, -0.2f, -2.0f);
     
     //Left
     glNormal3f(-1.0f, 0.0f, 0.0f);
     glVertex3f(-2.0f, -0.2f, -2.0f);
     glVertex3f(-2.0f, -0.2f, 2.0f);
     glVertex3f(-2.0f, 0.2f, 2.0f);
     glVertex3f(-2.0f, 0.2f, -2.0f);
     
     //top
     glNormal3f(0.0f,1.0f,0.0f);
     
     glVertex3f(2.0f, 0.2f, 2.0f);
     glVertex3f(-2.0f, 0.2f, 2.0f);
     glVertex3f(-2.0f, 0.2f, -2.0f);
     glVertex3f(2.0f, 0.2f, -2.0f);
     
        //bottom
        glNormal3f(0.0f,-1.0f,0.0f);
     
     glVertex3f(2.0f, -0.2f, 2.0f);
     glVertex3f(-2.0f, -0.2f, 2.0f);
     glVertex3f(-2.0f, -0.2f, -2.0f);
     glVertex3f(2.0f, -0.2f, -2.0f);
     
        //table front leg
        //front
        glNormal3f(0.0f, 0.0f, 1.0f);
     
     glVertex3f(1.8f,-0.2f,1.6f);
     glVertex3f(1.4f, -0.2f, 1.6f);
     glVertex3f(1.4f, -3.0f, 1.6f);
     glVertex3f(1.8f, -3.0f, 1.6f);
     
        //back
        glNormal3f(0.0f, 0.0f, -1.0f);
     
     glVertex3f(1.8f,-0.2f,1.2f);
     glVertex3f(1.4f, -0.2f, 1.2f);
     glVertex3f(1.4f, -3.0f, 1.2f);
     glVertex3f(1.8f, -3.0f, 1.2f);
     
     //right
     glNormal3f(1.0f, 0.0f, 0.0f);
     
     glVertex3f(1.8f,-0.2f,1.6f);
     glVertex3f(1.8f, -0.2f, 1.2f);
     glVertex3f(1.8f, -3.0f, 1.2f);
     glVertex3f(1.8f, -3.0f, 1.6f);
     
        //left
        glNormal3f(-1.0f, 0.0f, 0.0f);
     
     glVertex3f(1.4f,-0.2f,1.6f);
     glVertex3f(1.4f, -0.2f, 1.2f);
     glVertex3f(1.4f, -3.0f, 1.2f);
     glVertex3f(1.4f, -3.0f, 1.6f);
     
     //back leg back
     //front
     glNormal3f(0.0f, 0.0f, -1.0f);
     
     glVertex3f(1.8f,-0.2f,-1.2f);
     glVertex3f(1.4f, -0.2f, -1.2f);
     glVertex3f(1.4f, -3.0f, -1.2f);
     glVertex3f(1.8f, -3.0f, -1.2f);
     
     //back
     glNormal3f(0.0f, 0.0f, -1.0f);
     
     glVertex3f(1.8f,-0.2f,-1.6f);
     glVertex3f(1.4f, -0.2f, -1.6f);
     glVertex3f(1.4f, -3.0f, -1.6f);
     glVertex3f(1.8f, -3.0f, -1.6f);
     
        //right
        glNormal3f(1.0f, 0.0f, 0.0f);
     
     glVertex3f(1.8f,-0.2f,-1.6f);
     glVertex3f(1.8f, -0.2f, -1.2f);
     glVertex3f(1.8f, -3.0f, -1.2f);
     glVertex3f(1.8f, -3.0f, -1.6f);
     
     //left
        glNormal3f(1.0f, 0.0f, 0.0f);
     
     glVertex3f(1.4f,-0.2f,-1.6f);
     glVertex3f(1.4f, -0.2f, -1.2f);
     glVertex3f(1.4f, -3.0f, -1.2f);
     glVertex3f(1.4f, -3.0f, -1.6f);
     
        //leg left front
        glNormal3f(0.0f, 0.0f, 1.0f);
     
     glVertex3f(-1.8f,-0.2f,1.6f);
     glVertex3f(-1.4f, -0.2f, 1.6f);
     glVertex3f(-1.4f, -3.0f, 1.6f);
     glVertex3f(-1.8f, -3.0f, 1.6f);
     
        //back
        glNormal3f(0.0f, 0.0f, -1.0f);
     
     glVertex3f(-1.8f,-0.2f,1.2f);
     glVertex3f(-1.4f, -0.2f, 1.2f);
     glVertex3f(-1.4f, -3.0f, 1.2f);
     glVertex3f(-1.8f, -3.0f, 1.2f);
     
     //right
     glNormal3f(1.0f, 0.0f, 0.0f);
     
     glVertex3f(-1.8f,-0.2f,1.6f);
     glVertex3f(-1.8f, -0.2f, 1.2f);
     glVertex3f(-1.8f, -3.0f, 1.2f);
     glVertex3f(-1.8f, -3.0f, 1.6f);
     
        //left
        glNormal3f(-1.0f, 0.0f, 0.0f);
     
     glVertex3f(-1.4f,-0.2f,1.6f);
     glVertex3f(-1.4f, -0.2f, 1.2f);
     glVertex3f(-1.4f, -3.0f, 1.2f);
     glVertex3f(-1.4f, -3.0f, 1.6f);
     
     //left leg back front
     
     //front
     glNormal3f(0.0f, 0.0f, -1.0f);
     
     glVertex3f(-1.8f,-0.2f,-1.2f);
     glVertex3f(-1.4f, -0.2f, -1.2f);
     glVertex3f(-1.4f, -3.0f, -1.2f);
     glVertex3f(-1.8f, -3.0f, -1.2f);
     
     //back
     glNormal3f(0.0f, 0.0f, -1.0f);
     
     glVertex3f(-1.8f,-0.2f,-1.6f);
     glVertex3f(-1.4f, -0.2f, -1.6f);
     glVertex3f(-1.4f, -3.0f, -1.6f);
     glVertex3f(-1.8f, -3.0f, -1.6f);
     
        //right
        glNormal3f(1.0f, 0.0f, 0.0f);
     
     glVertex3f(-1.8f,-0.2f,-1.6f);
     glVertex3f(-1.8f, -0.2f, -1.2f);
     glVertex3f(-1.8f, -3.0f, -1.2f);
     glVertex3f(-1.8f, -3.0f, -1.6f);
     
     //left
        glNormal3f(-1.0f, 0.0f, 0.0f);
     
     glVertex3f(-1.4f,-0.2f,-1.6f);
     glVertex3f(-1.4f, -0.2f, -1.2f);
     glVertex3f(-1.4f, -3.0f, -1.2f);
     glVertex3f(-1.4f, -3.0f, -1.6f);
     
     //chair back
     //front
     glColor3f(1,0,0);
     //glNormal3f(-1.0f, 0.0f, 0.0f);
     glVertex3f(-1.8f, 0.2f, -1.8f);
     glVertex3f(1.8f, 0.2f, -1.8f);
     glVertex3f(1.8f, 3.5f, -1.8f);
     glVertex3f(-1.8f, 3.5f, -1.8f);
     
        //back
     //glNormal3f(-1.0f, 0.0f, 0.0f);
     glVertex3f(-1.8f, 0.2f, -2.0f);
     glVertex3f(1.8f, 0.2f, -2.0f);
     glVertex3f(1.8f, 3.5f, -2.0f);
     glVertex3f(-1.8f, 3.5f, -2.0f);
     
     
      //  glNormal3f(-1.0f, 0.0f, 0.0f);
     glVertex3f(-1.8f, 0.2f, -2.0f);
     glVertex3f(-1.8f, 3.5f, -2.0f);
     glVertex3f(-1.8f, 3.5f, -1.8f);
     glVertex3f(-1.8f, 0.2f, -1.8f);
     
     
        glVertex3f(1.8f, 0.2f, -2.0f);
     glVertex3f(1.8f, 3.5f, -2.0f);
     glVertex3f(1.8f, 3.5f, -1.8f);
     glVertex3f(1.8f, 0.2f, -1.8f);
     
     glVertex3f(-1.8f, 3.5f, -2.0f);
     glVertex3f(-1.8f, 3.5f, -1.8f);
     glVertex3f(1.8f, 3.5f, -1.8f);
        glVertex3f(1.8f, 3.5f, -2.0f);
     glEnd();
}
void DrawHat()
{
	glPushMatrix();
    DrawBody();
    glPopMatrix();
    glPushMatrix();
    glDisable(GL_TEXTURE_2D);
    DrawEyes();
    glPopMatrix();
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    DrawLeftArm();
    glPopMatrix();
    glPushMatrix();
    DrawRightArm();
    glPopMatrix();
	glPushMatrix();
	DrawLeftLeg();
	DrawRightLeg();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	
}

void DrawPopcorn()
{

}
/******************************************************
 * myDisplay								  		  *
 ******************************************************/
void myDisplay(void)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	bodyPart = gluNewQuadric();
	gluQuadricDrawStyle(bodyPart, GLU_FILL);
	//gluQuadricDrawStyle(bodyPart, GLU_LINE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glLoadIdentity();
    glEnable(GL_DEPTH_TEST);

    /********************************LIGHTING***/
    SET_LIGHTS();
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_LIGHT0);

	glRotatef(xx, 1.0, 0.0, 0.0);
	glRotatef(yy, 0.0, 1.0, 0.0);
	glRotatef(zz, 0.0, 0.0, 1.0);

	glScalef(0.05, 0.05, 0.05);

	glTranslatef(0.0, 1.0, -5.0);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
    
  	gluQuadricTexture(bodyPart, GL_TRUE);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, ConeMap);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	
	
	gluCylinder(bodyPart, 1.8, 1.0, 5.0, 30, 10);
	glTranslatef(0.0, 0.0, 5.0);
	gluDisk(bodyPart, 0.0, 1.0, 30, 10);
	glBindTexture(GL_TEXTURE_2D, PopMap);
	glTranslatef(0.0, 0.0, -5.0);
	gluSphere(bodyPart, 1.75, 15, 15);
	

	glFlush();
	glutSwapBuffers();
    glDisable(GL_LIGHTING);
    glutPostRedisplay();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

}

void my_idle(void)
{
	if(keepMoving > -1) //< stopMoving)
	{
	
		if(moveArms >= 0)
		{
			moveArms = moveArms + 1;
			uarmL = uarmL - 1;
			uarmR = uarmR + 1;
			larmL = larmL - 1;
			larmR = larmR + 1;
			if(moveArms == 130) 
			{
				moveArms = -130;
			}	
		}
		else
		{
			moveArms = moveArms + 1;
			uarmL = uarmL + 1;
			uarmR = uarmR - 1;
			larmL = larmL + 1;
			larmR = larmR - 1;
		}
	
	
	
		if(moveLegs > 0)
		{
			thighR = (thighR + 2) % 360;
			thighL = (thighL - 1) % 360;
			if(thighL == -45)
			{
				moveLegs = -100;
			}
			glutPostRedisplay();
		}  
		else if( moveLegs < 0)
		{
			thighR = (thighR - 2) % 360;
			thighL = (thighL + 1) % 360;
			if(thighL == 45)
			{
				moveLegs = 100;
			}
			glutPostRedisplay();
		}	
		keepMoving++;
	}

}

/******************************************************
 * setMatrix								  		  *
 ******************************************************/
void setMatrix(void)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10.0, 10.0, -10.0, 10.0, -10.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/******************************************************
 * resize									  		  *
 ******************************************************/
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	setMatrix();
}
void specialKeys(int key, int x, int y)
{
	if (key == GLUT_KEY_RIGHT)
		zz += 5;

	else if (key == GLUT_KEY_LEFT)
		zz -= 5;

	else if (key == GLUT_KEY_UP)
		yy += 5;

	else if (key == GLUT_KEY_DOWN)
		yy-= 5;

	else if (key == GLUT_KEY_PAGE_UP)
		xx += 5;

	else if (key == GLUT_KEY_PAGE_DOWN)
		xx -= 5;

	//  Request display update
	glutPostRedisplay();
}

void my_textures(void)
{	
 	int w, h, c;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	data = PGM_FILE_READ_1D("bin.ppm", &w, &h, &c);
	glGenTextures(1, &ConeMap);
	glBindTexture(GL_TEXTURE_2D, ConeMap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data);  
	
	data = PGM_FILE_READ_1D("popcorn.ppm", &w, &h, &c);
	glGenTextures(1, &PopMap);
	glBindTexture(GL_TEXTURE_2D, PopMap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data);
    

}

/******************************************************
 * Main										  		  *
 ******************************************************/
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
 
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(1000,1000);
	glutCreateWindow("hw5-MeganGiesler");

	myInIt();
    my_textures();
	glutDisplayFunc(myDisplay);
	glutSpecialFunc(specialKeys);
	glutIdleFunc(my_idle);
	glutMainLoop();
	return 0;     
}






















