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

/*
	GROUP: include class header files here
*/
#include "./world/World.h"
#include "./rabbit/Rabbit.h"
#include "./carrot/Carrot.h"
#include "./hat/TopHat.h"

/*
	GROUP: define class variable here
*/
World *world;
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
float xPos = 0.0;
float yPos = 0.0;
float zPos = 0.0;
int xRot = 0.0;
int yRot = 0.0;

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
	/*
		GROUP: Place class constructor here
	*/
	world = new World(0.0, -3.5, 0.0);
	rabbit = new Rabbit(0.0, 0.0, -10.0);
	carrot = new Carrot();
	tophat = new TopHat(100.0, 5.0, 0.0);

	glutMainLoop();
	delete rabbit;
	return 0;
}

void initialize()
{
	glClearColor(0.25, 0.5, 1.0, 1.0); // Background color
	glClearDepth(1.0f); // Background Depth maximum
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);
	glCullFace(GL_BACK);
	// Light
	GLfloat lightamb[]={1.0,1.0,1.0,1.0};
	GLfloat lightdif[]={0.5,0.5,0.5,1.0};
	GLfloat lightpos[]={100,100,100,1};
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightamb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightdif);
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
}

void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat matAmbient[]   = {0.19125, 0.0735,  0.025,    1.0};

	#if RAYGL == 1
	rayglFrameBegin((char*)"./frames/frame");
	setFadeDistance(1000.0);
	setFadePower(2.0);
	// Material for povray
	glColor3fv(matAmbient);
	#endif

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
		glRotatef(yRot, 1.0, 0.0, 0.0); // pitch
		glRotatef(-xRot, 0.0, 1.0, 0.0); // Heading
		glTranslatef(-xPos, -yPos, -zPos);
		/*
			GROUP: place class draw function here
		*/
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
			carrot->carrotPos(5,5,5);
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

void reshape(GLsizei width, GLsizei height)
{
	if (height == 0) height = 1;
	GLfloat aspect = (GLfloat)width / (GLfloat)height;
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(45.0f, aspect, 1.0f, 2000.0f);
}

void specialKeyPressed(int key, int xMouse, int yMouse)
{
	if(key == GLUT_KEY_RIGHT){
		xRot = (xRot - 1) % 360;
	} else if (key == GLUT_KEY_LEFT){
		xRot = (xRot + 1) % 360;
	} else if (key == GLUT_KEY_UP){
		yRot = (yRot - 1) % 360;
	} else if (key == GLUT_KEY_DOWN){
		yRot = (yRot + 1) % 360;
	} else {
		return;
	}
	glutPostRedisplay();
}

void moveCamForward()
{
	xPos -= sin(xRot * DEGREES_TO_RADIAN); // 90 = 1, 270 = -1
	yPos -= sin(yRot * DEGREES_TO_RADIAN); // 90 = 1, 270 = -1
	zPos -= cos(xRot * DEGREES_TO_RADIAN); // 0 = 1, 180 = -1
}

void moveCamBackward()
{
	xPos += sin(xRot * DEGREES_TO_RADIAN); // 90 = 1, 270 = -1
	yPos += sin(yRot * DEGREES_TO_RADIAN); // 90 = 1, 270 = -1
	zPos += cos(xRot * DEGREES_TO_RADIAN); // 0 = 1, 180 = -1
}

void keyboardKeyPressed(unsigned char key, int xMouse, int yMouse)
{
	if (key == 'r')rabbit->cycleAnimation();
	else if(key == 'q')rabbit->cycleFancy();
	else if(key == 'w')moveCamForward();
	else if(key == 's')moveCamBackward();
	else return;
	glutPostRedisplay();
}

void idleFunction()
{
	/*
		GROUP: place class idle function here
	*/
	rabbit->idle();
	tophat->idle();
	glutPostRedisplay();
}
