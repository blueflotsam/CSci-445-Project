/**************************************************/
/* Name: David Andrew Wilson IV                   */
/*                                                */
/* Draws the environment                          */
/*                                                */
/* CSci 446 / fall 2019                           */
/**************************************************/

#include "../Harezini.h"

class World{
	public:
	static void draw(){
		glLoadIdentity();
		// Draw sky
		drawSky();
		// Draw Floor 100 x 100
		for(int x = 0; x < 100; x++){
			glPushMatrix();
			for(int y = 0; y < 100; y++){
				drawGrass();
				glTranslatef(0.0,1.0,0.0);
			}
			glPopMatrix();
			glTranslatef(1.0, 0.0, 0.0);
		}
	}

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
	
	static void drawGrass(){
		glBegin(GL_QUADS);
			glMaterialfv(GL_FRONT, LIGHTING_TYPE, GREEN);
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
};
