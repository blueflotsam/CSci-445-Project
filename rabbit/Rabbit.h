/**************************************************/
/* Name: David Andrew Wilson IV                   */
/*                                                */
/* Defines the Rabbit class                       */
/*                                                */
/* CSci 446 / fall 2019                           */
/**************************************************/

#include "../Harezini.h"

#define RABBIT_TEXTURE "./rabbit/rabbit_texture.ppm"
#define RABBIT_FACE "./rabbit/rabbit_face.ppm"
#define RABBIT_TEXTURE_NUM 2

class Rabbit
{
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
	float rotation;
	float degree;
	float dx;
	float dy;
	float dz;

	// Textures
	int numTextures; // set in constructor (RABBIT_TEXTURE_NUM)
	GLuint texture[RABBIT_TEXTURE_NUM];
	static const int FACE = 0;

	/*
			Loads the textures to be used later
	*/
	void loadTextures(){
		int width, height, nrChannels, texture_index;
		char *texture_file;
		unsigned char *data;

		// Helps with raygl texture problems?
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// Allocate raygl texture space
		#if RAYGL == 1
		Image *image[numTextures];
		for (int i = 0; i < numTextures; i++){
			image[i] = (Image *) malloc(sizeof(Image));
			if (image[i] == NULL) exit(0);
		}
		#endif

		// Load Rabbit face texture
		texture_file = (char *)RABBIT_FACE;
		texture_index = FACE;
		#if RAYGL == 1
		if (!imageLoad(texture_file, image[texture_index])) exit(0);
		#else
		data = stbi_load(texture_file, &width, &height, &nrChannels, 0);
		if (!data){
				printf("Unable to load \"%s\", exiting program\n", texture_file);
				stbi_image_free(data);
				exit(-1);
		}
		#endif
		glGenTextures(numTextures, &texture[texture_index]);
		glBindTexture(GL_TEXTURE_2D, texture[texture_index]);
		// Set texture wrapping options for bound texture
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		#if RAYGL == 1
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, image[texture_index]->sizeX, image[texture_index]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image[texture_index]->data);
		#else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
		#endif
	}

	void drawHead(){
		// Color
		glRotatef(degree,dx,0,0);
        	glRotatef(degree,0,dy,0);
        	glRotatef(degree,0,0,dz);
		glMaterialfv(GL_FRONT, LIGHTING_TYPE, WHITE);
		// Textured Part
		glEnable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);
			// Head Front
			glNormal3f( 0.0, 0.0, 1.0);
			glTexCoord2f(1.0, 1.0);
			glVertex3f( 0.8, 0.0, 0.5); // FR
			glTexCoord2f(1.0, 0.5);
			glVertex3f( 0.8, 0.6, 0.5); // BR
			glTexCoord2f(0.0, 0.5);
			glVertex3f(-0.8, 0.6, 0.5); // BL
			glTexCoord2f(0.0, 1.0);
			glVertex3f(-0.8, 0.0, 0.5); // FL
		glEnd();
		glDisable(GL_TEXTURE_2D);
		// Non-textured
		glBegin(GL_QUADS);
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
	}

	/*
	 *	draws a unit box with the top face centered on the origin
	 *
	 *	Parameter color is the color the box will be drawn
	**/
	void drawBoxDown(const float color[], float xScale, float yScale, float zScale){
		xScale /= 2;
		zScale /= 2;
		glBegin(GL_QUADS);
			// Color
			glMaterialfv(GL_FRONT, LIGHTING_TYPE, color);
			// Top
			glNormal3f( 0.0, 1.0, 0.0);
			glVertex3f( xScale,    0.0, zScale); // FR
			glVertex3f( xScale,    0.0,-zScale); // BR
			glVertex3f(-xScale,    0.0,-zScale); // BL
			glVertex3f(-xScale,    0.0, zScale); // FL
			// Bottom
			glNormal3f( 0.0,-1.0, 0.0);
			glVertex3f( xScale,-yScale, zScale); // FR
			glVertex3f(-xScale,-yScale, zScale); // FL
			glVertex3f(-xScale,-yScale,-zScale); // BL
			glVertex3f( xScale,-yScale,-zScale); // BR
			// Front
			glNormal3f( 0.0,0.0,1.0);
			glVertex3f( xScale,    0.0, zScale); // TR
			glVertex3f(-xScale,    0.0, zScale); // TL
			glVertex3f(-xScale,-yScale, zScale); // BL
			glVertex3f( xScale,-yScale, zScale); // BR
			// Back
			glNormal3f( 0.0, 0.0,-1.0);
			glVertex3f( xScale,    0.0,-zScale); // TR
			glVertex3f( xScale,-yScale,-zScale); // BR
			glVertex3f(-xScale,-yScale,-zScale); // BL
			glVertex3f(-xScale,    0.0,-zScale); // TL
			// Left
			glNormal3f(-1.0, 0.0, 0.0);
			glVertex3f(-xScale,    0.0, zScale); // FT
			glVertex3f(-xScale,    0.0,-zScale); // BT
			glVertex3f(-xScale,-yScale,-zScale); // BB
			glVertex3f(-xScale,-yScale, zScale); // FB
			// Right
			glNormal3f( 1.0, 0.0, 0.0);
			glVertex3f( xScale,    0.0, zScale); // FT
			glVertex3f( xScale,-yScale, zScale); // FB
			glVertex3f( xScale,-yScale,-zScale); // BB
			glVertex3f( xScale,    0.0,-zScale); // BT
		glEnd();
	}

	public:

	// Animation Definitions
	static const int IDLE = 0;
	static const int RIGHT_POINT = 1;
	static const int LEFT_POINT = 2;
	static const int JUMPING = 3;
	static const int TOTAL_ANIMATIONS = 4;

	// Constructor
	Rabbit(float xOrig, float yOrig, float zOrig, float rotation){
		this->xOrig = xOrig;
		this->yOrig = yOrig;
		this->zOrig = zOrig;
		this->degree=0;
		this->rotation = rotation;
		numTextures = RABBIT_TEXTURE_NUM;
		animation = 0;
		fancy = 0;
		rightPoint = RIGHT_POINT_DEF;
		leftPoint = LEFT_POINT_DEF;
		legBend = LEG_BEND_DEF;
		squish = SQUISH_DEF;
		rightSwing = RIGHT_SWING_DEF;
		leftSwing = LEFT_SWING_DEF;
		rightFlex = RIGHT_FLEX_DEF;
		leftFlex = LEFT_FLEX_DEF;
		loadTextures();
	}

	void cycleFancy(){
		fancy = !fancy;
	}

	void cycleAnimation(){
		animation += 1;
		if (animation == TOTAL_ANIMATIONS)animation = IDLE;
	}

	float *getLocation () {
		float *pos;
		float temp[] = {xOrig, yOrig, zOrig};
		pos = temp;
		return pos;
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

		void idle(int frame){
		if (frame == 1){
			setAnimation(JUMPING);
			zOrig -= 0.2;
		} else if (frame < 800){
			zOrig -= 0.2;
		} else if (frame == 800){
			setAnimation(IDLE);
		} 
        else if(frame<830){
            //wait
        }else if (frame < 875) {
			rotation += -2;
		} else if (frame < 930) {
			// Do nothing
		} else if (frame < 1050) {

            if (frame == 990) {
			    cycleFancy();
            }
            rotation+=3;
		} else if (frame < 1110) {
			// Do nothing
		} else if (frame == 1110) {
			setAnimation(JUMPING);
			xOrig += 0.1;
		} else if (frame < 1210) {
			xOrig += 0.1;
		} else if (frame == 1210) {
			setAnimation(IDLE);
			rightPoint += 2;
		} else if (frame < 1235) {
			rightPoint += 2;
		} else if (frame < 1260) {
			rightPoint -= 2;
		} else if (frame < 1275) {
			// Wait half second
		} else if (frame == 1245) {
			setAnimation(JUMPING);
			xOrig -= 0.05;
		} else if (frame < 1320) {
			xOrig -= 0.05; // back away from hat as it enlarges
		} else if (frame == 1320) {
			setAnimation(IDLE);
		}else if(frame<1380){
            		//nothing
       		}
       		 //wants carrot
        	else if(frame<1470){
            	//beckons hat to make carrot
            		leftPoint+=1;
        	}
        	else if(frame<1740){
            		//nothing may want to hop in anger before this
        	}
       		 //hops in anger
        	else if(frame==1740){
            	setAnimation(JUMPING); //doesnt work with rotating head
        	}
        	//shakes head for 80 frames multiple of four is good
        	else if(frame<1760){
            		degree-=.7;
            		dy=1;
        	}
        	else if(frame<1800){
            	degree+=.7;
        	}
        	else if(frame<1820){
            	degree-=.7;
        	}
        	else if(frame==1820){
            	degree=0;
            	dy=0;
            	setAnimation(IDLE);
        	}
		
		if (animation == RIGHT_POINT){
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
		// Setup texture
		glBindTexture(GL_TEXTURE_2D, texture[FACE]);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		#if RAYGL == 1
			rayglScaleTexture(1, 1, 1);
			rayglTranslateTexture(0, 0, 0);
			rayglRotateTexture(0, 0, 0);
			rayglTextureType(0);
		#endif
		glDisable(GL_TEXTURE_2D);
		// Global Matrix
		glTranslatef(xOrig, yOrig, zOrig);
		glRotatef(rotation, 0.0, 1.0, 0.0);
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
					drawBoxDown(WHITE, 0.6, 0.7, 0.2);
					glEnable(GL_TEXTURE_2D);
					glBegin(GL_QUADS);
						glNormal3f( 0.0,0.0,-1.0);
						glTexCoord2f(0.0, 0.5);
						glVertex3f(-0.25, 0.0, 0.11);
						glTexCoord2f(0.0, 0.0);
						glVertex3f(-0.25,-0.7, 0.11);
						glTexCoord2f(0.5, 0.0);
						glVertex3f( 0.25,-0.7, 0.11);
						glTexCoord2f(0.5, 0.5);
						glVertex3f( 0.25, 0.0, 0.11);
					glEnd();
					glTranslatef(0.0,-0.7, 0.0);
					drawBoxDown(WHITE, 0.6, 0.7, 0.2);
					glMaterialfv(GL_FRONT, LIGHTING_TYPE, RED);
					glBegin(GL_QUADS);
						glNormal3f( 0.0,0.0,-1.0);
						glTexCoord2f(0.5, 0.5);
						glVertex3f(-0.25, 0.0, 0.11);
						glTexCoord2f(0.5, 0.0);
						glVertex3f(-0.25,-0.7, 0.11);
						glTexCoord2f(1.0, 0.0);
						glVertex3f( 0.25,-0.7, 0.11);
						glTexCoord2f(1.0, 0.5);
						glVertex3f( 0.25, 0.0, 0.11);
					glEnd();
					glDisable(GL_TEXTURE_2D);
				glPopMatrix();
			}
		glPopMatrix();
		// Draw Torso
		glPushMatrix();
			glTranslatef(0.0, 1.0, 0.0);
			drawBoxDown(WHITE, 2.0, 2.0, 1.0);
			// Bow Tie
			if(fancy){
				glTranslatef(0.0,-0.2,0.51);
				glMaterialfv(GL_FRONT, LIGHTING_TYPE, BLACK);
				glBegin(GL_TRIANGLES);
					glNormal3f( 0.0,0.0, 1.0);
					glVertex3f( 0.0, 0.0, 0.0);
					glVertex3f( 0.6,-0.3, 0.0);
					glVertex3f( 0.6, 0.3, 0.0);
					glVertex3f( 0.0, 0.0, 0.0);
					glVertex3f(-0.6, 0.3, 0.0);
					glVertex3f(-0.6,-0.3, 0.0);
				glEnd();
				glutSolidSphere(0.2, 8, 8);
			}
		glPopMatrix();
		// Draw Arms
		for(float facing = 1.0; facing >= -1.0; facing -= 2.0){
			glPushMatrix();
				glTranslatef(facing,0.75,0.0);
				glRotatef((facing == 1.0) ? leftSwing : rightSwing, 0.0f, 0.0f, facing);
				glRotatef((facing == 1.0) ? leftPoint : rightPoint, -1.0, 0.0, 0.0);
				drawBoxDown(WHITE, 0.5, 1.0, 0.5);
				glutSolidSphere(0.3,16,16);
				glTranslatef(0.0,-1.0,0.0);
				glutSolidSphere(0.3,16,16);
				glRotatef((facing == 1.0) ? leftFlex : -rightFlex, 0.0f, 0.0f, 1.0f);
				drawBoxDown(WHITE, 0.5, 1.0, 0.5);
				// Draw magic wand
				if(fancy && facing == -1.0) { // Right arm
					glTranslatef(0.0, -0.8, 0.25);
					glRotatef(90.0, -1.0, 0.0, 0.0);
					drawBoxDown(BLACK, 0.2, 1.0, 0.2);
					glTranslatef(0.0,-1.0,0.0);
					drawBoxDown(WHITE, 0.2, 0.2, 0.2);
				}
			glPopMatrix();
		}
		// Draw Legs
		for(float facing = 1.0; facing >= -1.0; facing -= 2.0){
			glPushMatrix();
				glTranslatef(facing*0.5,-1.0,0.0);
				glRotatef(-legBend, 1.0f, 0.0f, 0.0f);
				// Top Section
				drawBoxDown(WHITE, 0.8, 1.0, 0.8);
				// Joint
				glPushMatrix();
					glScalef(0.9f, 1.0f, 1.0f);
					glutSolidSphere(0.5,16,16);
				glPopMatrix();
				// Bottom Section
				glTranslatef(0.0,-1.0,0.0);
				glRotatef(legBend*2.0, 1.0f, 0.0f, 0.0f);
				drawBoxDown(WHITE, 0.8, 1.0, 0.8);
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
				drawBoxDown(GRAY, 0.81, 1.5, 0.6);
			glPopMatrix();
		}
	}
};
