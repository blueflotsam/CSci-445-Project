/**************************************************/
/* Name: David Andrew Wilson IV                   */
/* Name: Megan Giesler                            */
/* Name: Cory Haas                                */
/*                                                */
/* HW9                                            */
/*                                                */
/* Main source file for rendering scene           */
/*                                                */
/* CSci 446 / fall 2019                           */
/**************************************************/

#include "Harezini.h"
#include "./world/World.h"
#include "./rabbit/Rabbit.h"
#include "./carrot/Carrot.h"
#include "./hat/TopHat.h"

World  *world;
Rabbit *rabbit;
Carrot *carrot;
TopHat *tophat;

#define LIGHTING_TYPE GL_AMBIENT_AND_DIFFUSE
#define YSCALE_DEF 1.0

// Scale globals
GLfloat xScale = 1.0f;
GLfloat yScale = YSCALE_DEF;
GLfloat zScale = 1.0f;

// Camera
double DEGREES_TO_RADIAN = M_PI / 180;
float xPos = -20.0;
float yPos = 0.0;
float zPos = 0.0;
float xRot = 90.0;
float yRot = 0.0;

// Swap buffer global
int swapBuffer = 1;

int main(int argc, char **argv)
{
	// Window creation
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Harezini");
	// Callback functions
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboardKeyPressed);
	glutSpecialFunc(specialKeyPressed);
	glutIdleFunc(idleFunction);
	// Start program
	initialize();
	// initialize class objects
	world = new World(0.0, -5.5, 0.0);
	rabbit = new Rabbit(3.0, -2.0, 100.0, 180);
	carrot = new Carrot(-5.0,-9.0,-93.0);
	tophat = new TopHat(22, -4, -60);
	// Main loop
	glutMainLoop();
	// Cleanup
	//delete world;
	//delete rabbit;
	//delete carrot;
	//delete tophat; 
	return 0;
}

void initialize()
{
	glClearColor(0.1, 0.3, 0.87, 1.0); // Background color
	glClearDepth(1.0f); // Background Depth maximum
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);
	glCullFace(GL_BACK);
	// Light
	GLfloat lightpos[]={999,999,999,1};
	#if RAYGL == 1
		GLfloat lightamb[]={0.6,0.6,0.6,1.0};
		GLfloat lightdif[]={1.0,1.0,1.0,1.0};
	#else
		GLfloat lightamb[]={1.0,1.0,1.0,1.0};
		GLfloat lightdif[]={0.7,0.7,0.7,1.0};
	#endif
	
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);

	// Ambient
	glColorMaterial(GL_FRONT, GL_AMBIENT);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightamb);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightamb);

	// Diffuse
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightdif);

	// position
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
}

void reshape(GLsizei width, GLsizei height)
{
	if (height == 0) height = 1;
	GLfloat aspect = (GLfloat)width / (GLfloat)height;
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(45.0f, aspect, 1.0f, 2000.0f);
}

void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat matAmbient[]   = {0.19125, 0.0735,  0.025,    1.0};
	// Material for povray
	glColor3fv(matAmbient);

	#if RAYGL == 1
	rayglFrameBegin((char*)"frames.txt");
	setFadeDistance(1000.0);
	setFadePower(2.0);
	#endif

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
		glRotatef(yRot, 1.0, 0.0, 0.0); // pitch
		glRotatef(-xRot, 0.0, 1.0, 0.0); // Heading
		glTranslatef(-xPos, -yPos, -zPos);
		// World
		glPushMatrix();
			world->draw();
		glPopMatrix();
		// Rabbit		
		glPushMatrix();        
			rabbit->draw();
		glPopMatrix();
		// TopHat
		glPushMatrix();
			tophat->draw();
		glPopMatrix();
		// Carrot
		glPushMatrix();
			carrot->drawCarrot();
		glPopMatrix();
	glPopMatrix();

	#if RAYGL == 1
	rayglFrameEnd();
	#endif

	if (swapBuffer) {
		glutSwapBuffers();
	}
}

void moveCam3f (float xChange, float yChange, float zChange)
{
	xPos += xChange;
	yPos += yChange;
	zPos += zChange;
}

void moveCamForward(float distance)
{
	xPos -= distance * sin(xRot * DEGREES_TO_RADIAN); // 90 = 1, 270 = -1
	yPos -= distance * sin(yRot * DEGREES_TO_RADIAN); // 90 = 1, 270 = -1
	zPos -= distance * cos(xRot * DEGREES_TO_RADIAN); // 0 = 1, 180 = -1
}

void moveCamBackward(float distance)
{
	xPos += distance * sin(xRot * DEGREES_TO_RADIAN); // 90 = 1, 270 = -1
	yPos += distance * sin(yRot * DEGREES_TO_RADIAN); // 90 = 1, 270 = -1
	zPos += distance * cos(xRot * DEGREES_TO_RADIAN); // 0 = 1, 180 = -1
}

//0=x, 1=y, 2=z degrees is degrees of rotation
void rotateCam(int direction, float degrees)
{
	if(direction==0){
		xRot= (xRot-degrees);
	}
	if(direction==1){
		yRot= (yRot-degrees);
	}
	if(direction==2){
		//zRot= (zRot-degrees);
	}

} 


void idleFunction()
{	
	static int frame = 1;
	/*
		GROUP: place class idle function here
	*/
	rabbit->idle(frame);
	tophat->idle(frame);
	carrot->idle(frame);

	// Pan around landscape, 16.67 seconds
	if (frame < 500){
		moveCamForward(0.2);
		rotateCam(0, -0.54);
	}
	// Follow behind rabbit, 10 seconds
	else if (frame < 800) {
		moveCamForward(0.2);
	} 
	// Rabbit turns to see hat, camera does nothing
	else if (frame < 845) {
		// Do nothing
	}
	// Turn to see hat
	else if (frame < 875) {
		rotateCam(0, 1);
	}
	// Wait to view hat for longer
	else if (frame < 900) {
		// Do nothing
	}
	// Jump to in front of Rabbit
	else if (frame == 900) {
		float *rabbitPos = rabbit->getLocation();
		xPos = rabbitPos[0] + 15;
		yPos = rabbitPos[1];
		zPos = rabbitPos[2];
		xRot = 90;
		yRot = -10.0;
	}
	// Rabbit spins
	else if (frame < 1050) {
		// Do nothing
	}
	// Move camera back to see rabbit and hat
	else if (frame < 1080) {
		moveCamBackward((frame-1050)/20);
		moveCam3f(0.0, 0.0, 0.4);
		rotateCam(0, 2.2);
	}
	// Wait
	else if (frame < 1245) {
		// Do nothing
	}
	// turn to look at hat as it enlarges
	else if (frame < 1290) {
		rotateCam(0, 0.3);
	}
	//repositions camera
	else if(frame<1320){
		rotateCam(0,.23);
		rotateCam(1,-.05);
		yPos+=.11;
		xPos-=.20;
	}
	
	glutPostRedisplay();
	frame++;
}

/*
		REMOVE LATER
*/

void keyboardKeyPressed(unsigned char key, int xMouse, int yMouse)
{
	if(key == 'w')moveCamForward(1.0);
	else if(key == 's')moveCamBackward(1.0);
	else return;
	glutPostRedisplay();
}

void specialKeyPressed(int key, int xMouse, int yMouse)
{
	if(key == GLUT_KEY_RIGHT){
		xRot = (xRot - 1);
	} else if (key == GLUT_KEY_LEFT){
		xRot = (xRot + 1);
	} else if (key == GLUT_KEY_UP){
		yRot = (yRot - 1);
	} else if (key == GLUT_KEY_DOWN){
		yRot = (yRot + 1);
	} else {
		return;
	}
	glutPostRedisplay();
}
