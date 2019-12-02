/**************************************************/
/* Name: David Andrew Wilson IV                   */
/*                                                */
/* Draws the environment                          */
/*                                                */
/* CSci 446 / fall 2019                           */
/**************************************************/

#include "../Harezini.h"

class World
{
	private:

	// Position values
	float xOrig;
	float yOrig;
	float zOrig;

	static void drawRoad(){
		glBegin(GL_QUADS);
			glMaterialfv(GL_FRONT, LIGHTING_TYPE, BLACK);
			glNormal3f(0.0,1.0,0.0);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(0.0, 0.0, 0.0);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(1.0, 0.0, 0.0);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(1.0, 0.0,-1.0);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(0.0, 0.0,-1.0);
		glEnd();
	}
	
	static void drawGrass(float size){
		size /= 2;
		glBegin(GL_QUADS);
			glMaterialfv(GL_FRONT, LIGHTING_TYPE, GREEN);
			glNormal3f(0.0,1.0,0.0);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-size, 0.0, size);
			glTexCoord2f(1.0, 0.0);
			glVertex3f( size, 0.0, size);
			glTexCoord2f(1.0, 1.0);
			glVertex3f( size, 0.0,-size);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(-size, 0.0,-size);
		glEnd();
	}

	static void drawSky(){
		glBegin(GL_QUADS);
			glMaterialfv(GL_FRONT, LIGHTING_TYPE, BLUE);
			glNormal3f(0.0,0.0,1.0);
			glVertex3f(-1000.0,-1000.0,-1000.0);
			glVertex3f( 1000.0,-1000.0,-1000.0);
			glVertex3f( 1000.0, 1000.0,-1000.0);
			glVertex3f(-1000.0, 1000.0,-1000.0);
		glEnd();
	}

	public:

	// Constructor
	World(float xOrig, float yOrig, float zOrig){
		this->xOrig = xOrig;
		this->yOrig = yOrig;
		this->zOrig = zOrig;
	}

	void draw(){
		glTranslatef(xOrig, yOrig, zOrig);
		drawGrass(100.0);
	}
};
