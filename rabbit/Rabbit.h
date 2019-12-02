/**************************************************/
/* Name: David Andrew Wilson IV                   */
/*                                                */
/* Defines the Rabbit class                       */
/*                                                */
/* CSci 446 / fall 2019                           */
/**************************************************/

#include "../Harezini.h"

class Rabbit {
	private:

	// Animation Defaults
	static const float RIGHT_POINT_DEF = 0.0;
	static const float LEFT_POINT_DEF = 0.0;
	static const float LEG_BEND_DEF = 30.0;
	static const float SQUISH_DEF = 1.0;
	static const float RIGHT_SWING_DEF = 25.0;
	static const float LEFT_SWING_DEF = 25.0;
	static const float RIGHT_FLEX_DEF = 15.0;
	static const float LEFT_FLEX_DEF = 15.0;

	// Animation Values
	int animation;
	int fancy;
	float rightPoint;
	float leftPoint;
	float legBend;
	float squish;
	float rightSwing;
	float leftSwing;
	float rightFlex;
	float leftFlex;

	// Position Values
	float xOrig;
	float yOrig;
	float zOrig;

	public:

	// Animation Definitions
	static const int IDLE = 0;
	static const int RIGHT_POINT = 1;
	static const int LEFT_POINT = 2;
	static const int JUMPING = 3;
	static const int TOTAL_ANIMATIONS = 4;

	// Constructor
	Rabbit(float xOrig, float yOrig, float zOrig){
		this->xOrig = xOrig;
		this->yOrig = yOrig;
		this->zOrig = zOrig;
		animation = 0;
		fancy = 1;
		rightPoint = RIGHT_POINT_DEF;
		leftPoint = LEFT_POINT_DEF;
		legBend = LEG_BEND_DEF;
		squish = SQUISH_DEF;
		rightSwing = RIGHT_SWING_DEF;
		leftSwing = LEFT_SWING_DEF;
		rightFlex = RIGHT_FLEX_DEF;
		leftFlex = LEFT_FLEX_DEF;
	}

	void cycleFancy(){
		fancy = !fancy;
	}

	void cycleAnimation(){
		animation += 1;
		if (animation == TOTAL_ANIMATIONS)animation = IDLE;
	}

	void setAnimation(int set){
		// Set animation
		animation = set;
		// Set animation variables to default value
		rightPoint = RIGHT_POINT_DEF;
		leftPoint = LEFT_POINT_DEF;
		legBend = LEG_BEND_DEF;
		squish = SQUISH_DEF;
	}

	void idle(){
		if(animation == RIGHT_POINT){
			rightPoint += 1.0;
			if(rightPoint > 70)rightPoint = RIGHT_POINT_DEF;
		} else if (animation == LEFT_POINT){
			leftPoint += 1.0;
			if(leftPoint > 70)leftPoint = LEFT_POINT_DEF;
		} else if (animation == JUMPING){
			legBend += 1.0;
			squish *= 0.999;
			if(legBend > 45){
				legBend = LEG_BEND_DEF;
				squish = SQUISH_DEF;
			}
		}
	}

	void draw(){
		glDisable(GL_TEXTURE_2D);
		// Global Matrix
		glTranslatef(xOrig, yOrig, zOrig);
		// Draw Head
		glPushMatrix();
			glTranslatef(0.0,1.0,0.2);
			drawHead();
			// Eyes
			for(float side = 1.0; side >= -1.0; side -= 2.0){ 
				glPushMatrix();
					glTranslatef(side*0.4,0.9,0.2);
					glMaterialfv(GL_FRONT, LIGHTING_TYPE, BLUE);
					glutSolidSphere(0.1,8,8);
					glPushMatrix();
						glTranslatef(0.0,-0.02,0.1);
						glMaterialfv(GL_FRONT, LIGHTING_TYPE, BLACK);
						glutSolidSphere(0.05,8,8);
					glPopMatrix();
				glPopMatrix();
			}
			// Ears
			for(float side = 1.0; side >= -1.0; side -= 2.0){
				glPushMatrix();
					glTranslatef(side*0.5, 1.0, -0.7);
					glRotatef(180.0-(side*20), 0.0, 0.0, 1.0);
					glScalef(0.6, 0.7, 0.2);
					drawBoxDown(WHITE);
					glMaterialfv(GL_FRONT, LIGHTING_TYPE, RED);
					glBegin(GL_QUADS);
                        glNormal3f( 0.0,0.0,-1.0);
						glVertex3f(-0.2, 0.0, 0.51);
						glVertex3f(-0.2,-1.0, 0.51);
						glVertex3f( 0.2,-1.0, 0.51);
						glVertex3f( 0.2, 0.0, 0.51);
					glEnd();
					glTranslatef(0.0,-1.0, 0.0);
					drawBoxDown(WHITE);
					glMaterialfv(GL_FRONT, LIGHTING_TYPE, RED);
					glBegin(GL_QUADS);
                        glNormal3f( 0.0,0.0,-1.0);
						glVertex3f(-0.2, 0.0, 0.51);
						glVertex3f(-0.2,-0.8, 0.51);
						glVertex3f( 0.2,-0.8, 0.51);
						glVertex3f( 0.2, 0.0, 0.51);
					glEnd();
				glPopMatrix();
			}
		glPopMatrix();
		// Draw Torso
		glPushMatrix();
			glTranslatef(0.0, 1.0, 0.0);
			glScalef(2.0, 2.0, 1.0);
			drawBoxDown(WHITE);
			// Bow Tie
			if(fancy){
				glTranslatef(0.0,-0.1,0.51);
				glMaterialfv(GL_FRONT, LIGHTING_TYPE, BLACK);
				glBegin(GL_TRIANGLES);
                    glNormal3f( 0.0,0.0, 1.0);
					glVertex3f( 0.0, 0.0, 0.0);
					glVertex3f( 0.3,-0.15, 0.0);
					glVertex3f( 0.3, 0.15, 0.0);
					glVertex3f( 0.0, 0.0, 0.0);
					glVertex3f(-0.3, 0.15, 0.0);
					glVertex3f(-0.3,-0.15, 0.0);
				glEnd();
				glutSolidSphere(0.1, 8, 8);
			}
		glPopMatrix();
		// Draw Arms
		for(float facing = 1.0; facing >= -1.0; facing -= 2.0){
			glPushMatrix();
				glTranslatef(facing,0.75,0.0);
				glRotatef((facing == 1.0) ? leftSwing : rightSwing, 0.0f, 0.0f, facing);
				glRotatef((facing == 1.0) ? leftPoint : rightPoint, -1.0, 0.0, 0.0);
				glPushMatrix();
					glScalef(0.5,1.0,0.5);
					drawBoxDown(WHITE);
				glPopMatrix();
				glutSolidSphere(0.3,16,16);
				glTranslatef(0.0,-1.0,0.0);
				glutSolidSphere(0.3,16,16);
				glRotatef((facing == 1.0) ? leftFlex : -rightFlex, 0.0f, 0.0f, 1.0f);
				glPushMatrix();
					glScalef(0.5,1.0,0.5);
					drawBoxDown(WHITE);
				glPopMatrix();
				// Draw magic wand
				if(fancy && facing == -1.0) { // Right arm
					glTranslatef(0.0, -0.8, 0.25);
					glRotatef(90.0, -1.0, 0.0, 0.0);
					glScalef(0.2,1.0,0.2);
					drawBoxDown(BLACK);
					glTranslatef(0.0,-1.0,0.0);
					glScalef(1.0, 0.2, 1.0);
					drawBoxDown(WHITE);
				}
			glPopMatrix();
		}
		// Draw Legs
		for(float facing = 1.0; facing >= -1.0; facing -= 2.0){
			glPushMatrix();
				glTranslatef(facing*0.5,-1.0,0.0);
				glRotatef(-legBend, 1.0f, 0.0f, 0.0f);
				// Top Section
				glPushMatrix();
					glScalef(0.8f, 1.0f, 0.8f);
					drawBoxDown(WHITE);
				glPopMatrix();
				// Joint
				glPushMatrix();
					glScalef(0.9f, 1.0f, 1.0f);
					glutSolidSphere(0.5,16,16);
				glPopMatrix();
				// Bottom Section
				glTranslatef(0.0,-1.0,0.0);
				glRotatef(legBend*2.0, 1.0f, 0.0f, 0.0f);
				glPushMatrix();
					glScalef(0.8f, 1.0f, 0.8f);
					drawBoxDown(WHITE);
				glPopMatrix();
				// Joint
				glPushMatrix();
					glScalef(0.9f, 1.0f, 1.0f);
					glutSolidSphere(0.5,16,16);
				glPopMatrix();
				// Joint
				glTranslatef(0.0,-1.0, 0.0);
				glPushMatrix();
					glScalef(0.9f, 1.0f, 1.0f);
					glutSolidSphere(0.4,16,16);
				glPopMatrix();
				// Foot
				glTranslatef(0.0,-0.5,-0.3);
				glRotatef(-legBend, 1.0f, 0.0f, 0.0f);
				glRotatef(90.0f,-1.0f, 0.0f, 0.0f);
				glPushMatrix();
					glScalef(0.81f, 1.5f, 0.6f);
					drawBoxDown(GRAY);
				glPopMatrix();
			glPopMatrix();
		}
	}

	/*
	 *	draws a unit box with the top face centered on the origin
	 *
	 *	Parameter color is the color the box will be drawn
	**/
	void drawBoxDown(const float color[]){
		glBegin(GL_QUADS);
			// Color
		      glMaterialfv(GL_FRONT, LIGHTING_TYPE, color);
			// Top
		      glNormal3f( 0.0,1.0,0.0);
			glVertex3f( 0.5, 0.0, 0.5); // FR
			glVertex3f( 0.5, 0.0,-0.5); // BR
			glVertex3f(-0.5, 0.0,-0.5); // BL
			glVertex3f(-0.5, 0.0, 0.5); // FL
			// Bottom
		      glNormal3f( 0.0,-1.0,0.0);
			glVertex3f( 0.5,-1.0, 0.5); // FR
			glVertex3f(-0.5,-1.0, 0.5); // FL
			glVertex3f(-0.5,-1.0,-0.5); // BL
			glVertex3f( 0.5,-1.0,-0.5); // BR
			// Front
		      glNormal3f( 0.0,0.0,1.0);
			glVertex3f( 0.5, 0.0, 0.5); // TR
			glVertex3f(-0.5, 0.0, 0.5); // TL
			glVertex3f(-0.5,-1.0, 0.5); // BL
			glVertex3f( 0.5,-1.0, 0.5); // BR
			// Back
		      glNormal3f( 0.0,0.0,-1.0);
			glVertex3f( 0.5, 0.0,-0.5); // TR
			glVertex3f( 0.5,-1.0,-0.5); // BR
			glVertex3f(-0.5,-1.0,-0.5); // BL
			glVertex3f(-0.5, 0.0,-0.5); // TL
			// Left
		      glNormal3f(-1.0,0.0,0.0);
			glVertex3f(-0.5, 0.0, 0.5); // FT
			glVertex3f(-0.5, 0.0,-0.5); // BT
			glVertex3f(-0.5,-1.0,-0.5); // BB
			glVertex3f(-0.5,-1.0, 0.5); // FB
			// Right
		      glNormal3f( 1.0,0.0,0.0);
			glVertex3f( 0.5, 0.0, 0.5); // FT
			glVertex3f( 0.5,-1.0, 0.5); // FB
			glVertex3f( 0.5,-1.0,-0.5); // BB
			glVertex3f( 0.5, 0.0,-0.5); // BT
		glEnd();
	}

	void drawHead(){
		glBegin(GL_QUADS);
			// Color
			glMaterialfv(GL_FRONT, LIGHTING_TYPE, WHITE);
			// Head Back
		      glNormal3f( 0.0,0.0,-1.0);
			glVertex3f( 0.8, 0.0,-1.0); // BR
			glVertex3f(-0.8, 0.0,-1.0); // BL
			glVertex3f(-0.8, 1.4,-1.0); // TL
			glVertex3f( 0.8, 1.4,-1.0); // TR
			// Head Top
		      glNormal3f( 0.0,1.0,0.0);
			glVertex3f(-0.8, 1.4,-1.0); // BL
			glVertex3f(-0.8, 1.4,-0.4); // FL
			glVertex3f( 0.8, 1.4,-0.4); // FR
			glVertex3f( 0.8, 1.4,-1.0); // BR
			// Head Front Diagonal
		      glNormal3f( 0.0,1.0, 1.0);
			glVertex3f( 0.8, 0.6, 0.5); // FR
			glVertex3f( 0.8, 1.4,-0.4); // BR
			glVertex3f(-0.8, 1.4,-0.4); // BL
			glVertex3f(-0.8, 0.6, 0.5); // FL
			// Head Front
		      glNormal3f( 0.0,0.0, 1.0);
			glVertex3f( 0.8, 0.0, 0.5); // FR
			glVertex3f( 0.8, 0.6, 0.5); // BR
			glVertex3f(-0.8, 0.6, 0.5); // BL
			glVertex3f(-0.8, 0.0, 0.5); // FL
			// Head Side Left Back
		      glNormal3f(-1.0,0.0,0.0);
			glVertex3f(-0.8, 1.4,-0.4); // TR
			glVertex3f(-0.8, 1.4,-1.0); // TL
			glVertex3f(-0.8, 0.0,-1.0); // BL
			glVertex3f(-0.8, 0.0,-0.4); // BR
			// Head Side Left Front
		      glNormal3f(-1.0,0.0,0.0);
			glVertex3f(-0.8, 1.4,-0.4); // TL
			glVertex3f(-0.8, 0.0,-0.4); // BL
			glVertex3f(-0.8, 0.0, 0.5); // BR
			glVertex3f(-0.8, 0.6, 0.5); // TR
			// Head Side Right Back
		      glNormal3f( 1.0,0.0,0.0);
			glVertex3f( 0.8, 1.4,-0.4); // TR
			glVertex3f( 0.8, 0.0,-0.4); // BR
			glVertex3f( 0.8, 0.0,-1.0); // BL
			glVertex3f( 0.8, 1.4,-1.0); // TL
			// Head Side Right Front
		      glNormal3f( 1.0,0.0,0.0);
			glVertex3f( 0.8, 1.4,-0.4); // TL
			glVertex3f( 0.8, 0.6, 0.5); // TR
			glVertex3f( 0.8, 0.0, 0.5); // BR
			glVertex3f( 0.8, 0.0,-0.4); // BL
			// Head Base
		      glNormal3f(0.0,-1.0,0.0);
			glVertex3f( 0.8, 0.0, 0.5); // FR
			glVertex3f(-0.8, 0.0, 0.5); // FL
			glVertex3f(-0.8, 0.0,-1.0); // BL
			glVertex3f( 0.8, 0.0,-1.0); // BR
		glEnd();
		// Nose
		glBegin(GL_LINES);
			glMaterialfv(GL_FRONT, LIGHTING_TYPE, RED);
			// Vertical
			glVertex3f( 0.0, 0.0, 0.51);
			glVertex3f( 0.0, 0.2, 0.51);
			// Right
			glVertex3f( 0.0, 0.2, 0.51);
			glVertex3f(-0.3, 0.4, 0.51);
			// Left
			glVertex3f( 0.0, 0.2, 0.51);
			glVertex3f( 0.3, 0.4, 0.51);
		glEnd();
	}
};
