/**************************************************/
/* Name: David Andrew Wilson IV                   */
/*                                                */
/* Draws the environment                          */
/*                                                */
/* CSci 446 / fall 2019                           */
/**************************************************/

#include "../Harezini.h"

#define FELT_FILE "./hat/felt.ppm"
#define HAT_TEXTURE_NUM 1

class TopHat
{
	private:

    GLUquadricObj *bodyPart;
    int uarmL;
    int uarmR;
    int moveLegs;
    int thighR, thighL, shinR, shinL, moveArms, larmL, larmR;

	// Position values
	float xOrig;
	float yOrig;
	float zOrig;

	// Textures:
	int numTextures; // gets set in constructor via HAT_TEXTURE_NUM
	GLuint texture[HAT_TEXTURE_NUM];
	static const int FELT = 0;

	void loadTextures(){
		int width, height, nrChannels, texture_index;
		char *texture_file;
		unsigned char *data;

		// Allocate raygl texture space
		#if RAYGL == 1
		Image *image[numTextures];
		for (int i = 0; i < numTextures; i++){
			image[i] = (Image *) malloc(sizeof(Image));
			if (image[i] == NULL) exit(0);
		}
		#endif

		// Load road texture  ! change to yours
		texture_file = (char *)FELT_FILE;
		texture_index = FELT;
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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		#if RAYGL == 1
		glTexImage2DGL_TEXTURE_2D, 0, GL_RGB8, image[texture_index]->sizeX, image[texture_index]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image[texture_index]->data);
		#else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
		#endif
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
		    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, WHITE);
		    glTranslatef(4.5, -2.0, 3.5);
		    gluSphere(bodyPart, 1.0, 15, 15);
		
	    //Draw Right Eye
		    glTranslatef(0.4, 3.5, 0.0);
		    gluSphere(bodyPart, 1.0, 15, 15);

	    //Draw Pupils
		    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, BLACK);
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

   /* void DrawHat()
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
	
    }*/

	public:

	// Constructors
	TopHat(){
		this->xOrig = 0;
		this->yOrig = 0;
		this->zOrig = 0;
		this->numTextures = HAT_TEXTURE_NUM;
        this->uarmL = -20;
        this->uarmR = 20;
        this->thighR = 0;
		this->thighL = 0;
		this->shinR = 0;
		this->shinL = 0;
		this->moveArms = 0;
		this->larmL = 0;
		this->larmR = 0;
        this->moveLegs = 180;
		loadTextures();
	}
	TopHat(float xOrig, float yOrig, float zOrig){
		this->xOrig = xOrig;
		this->yOrig = yOrig;
		this->zOrig = zOrig;
		this->numTextures = HAT_TEXTURE_NUM;
        this->uarmL = -20;
        this->uarmR = 20;
		this->thighR = 0;
		this->thighL = 0;
		this->shinR = 0;
		this->shinL = 0;
		this->moveArms = 0;
		this->larmL = 0;
		this->larmR = 0;
        this->moveLegs = 180;
		loadTextures();
	}

	void draw(){
		bodyPart = gluNewQuadric();
		gluQuadricDrawStyle(bodyPart, GLU_FILL);
        glEnable(GL_TEXTURE_2D);
		// Hat texture binding
		glBindTexture(GL_TEXTURE_2D, texture[FELT]);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		#if RAYGL == 1
			rayglScaleTexture(1, 1, 1);
			rayglTranslateTexture(0, 0, 0);
			rayglRotateTexture(0, 0, 0);
			rayglTextureType(0);
		#endif
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

    void idle(int frame)
    {
	    if(0 > -1) // 
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
		   // keepMoving++;
	    }

    }
};