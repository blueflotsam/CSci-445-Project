/**************************************************/
/* Name: Megan Giesler	                          */
/*                                                */
/* Draws a teapot and has the code for the	  */
/* movie					  */
/*                                                */
/* CSci 446 / fall 2019                           */
/**************************************************/

#include "../Harezini.h"

#define DOT_FILE "./teapot/texture5.ppm"
#define TEA_TEXTURE_NUM 1

class Teapot
{
	private:

    GLUquadricObj *bodyPart;
    int uarmL;
    int uarmR;
    int moveLegs;
    int thighR, thighL, shinR, shinL, moveArms, larmL, larmR;
    bool isAlive;
    float degree;
	// Position values
	float xOrig;
	float yOrig;
	float zOrig;
    float x,y,z;
    
	// Textures:
	int numTextures; // gets set in constructor via TEA_TEXTURE_NUM
	GLuint texture[TEA_TEXTURE_NUM];
	static const int DOT = 0;

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

		// Load road texture  ! change to yours
		texture_file = (char *)DOT_FILE;
		texture_index = DOT;
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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, image[texture_index]->sizeX, image[texture_index]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image[texture_index]->data);
		#else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
		#endif
	}

    

	public:

	// Constructors
	Teapot(float xOrig, float yOrig, float zOrig){
		this->xOrig = xOrig;
		this->yOrig = yOrig;
		this->zOrig = zOrig;
		this->numTextures = TEA_TEXTURE_NUM;
        this->degree=-90;
		loadTextures();
	}

	void draw()
	{
        glTranslatef(xOrig,yOrig,zOrig);
        glEnable(GL_TEXTURE_2D);
		// Teapot texture binding
		glBindTexture(GL_TEXTURE_2D, texture[DOT]);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		#if RAYGL == 1
			rayglScaleTexture(1, 1, 1);
			rayglTranslateTexture(0, 0, 0);
			rayglRotateTexture(0, 0, 0);
			rayglTextureType(0);
		#endif
		glRotatef(x, 1, 0, 0);
		glRotatef(y, 0, 1, 0);
        glRotatef(z, 0, 0, 1);    
        glScalef(.5, .5, .5);  
        glPushMatrix();
            int size = 1;
		    glutSolidTeapot(size);
        glPopMatrix();
	    glDisable(GL_TEXTURE_2D);
	}
	
	
    void idle(int frame)
    {
        if(frame < 1810)
        {
            //do nothing
        }
        else if(frame == 1810){  //draw teapot in hat
            yOrig = 4.0;
        }
        else if(frame < 1920){
            //do nothing
        }       
        else if(frame < 1990){ //toss teapot
            yOrig += .04;
            zOrig += .05;
        }
        else if (frame < 2030){//toss teapot
            zOrig += 0.2;
            yOrig += .03;
        }
        else if (frame < 2090){ //toss teapot
            yOrig -= .03;
            zOrig+=.15;
        }
        else if (frame == 2090){ //draw teapot in hat
            yOrig = 4.0;
            xOrig = 22.0;
            zOrig = -60.0;
        }
        else if(frame < 2290){
            //do nothing
        }       
        else if(frame < 2360){ //toss teapot
            yOrig += .04;
            zOrig += .07;
        }
        else if (frame < 2400){//toss teapot
            zOrig += 0.2;
            yOrig += .03;
        }
        else if (frame < 2460){ //toss teapot
            yOrig -= .03;
            zOrig+=.15;
        }
        else if (frame == 2460){ //draw teapot in hat
            yOrig = 4.0;
            xOrig = 22.0;
            zOrig = -60.0;
        }
        else if(frame < 2540){
            //do nothing
        }       
        else if(frame < 2600){ //toss teapot
            yOrig += .055;
            zOrig -= .05;
        }
        else if (frame < 2630){//toss teapot
            zOrig -= 0.2;
            yOrig += .03;
        }
        else if (frame < 2710){ //toss teapot
            yOrig -= .03;
            zOrig -=.15;
        }
        else if (frame == 2710){ //draw teapot in the under world
            yOrig = -14.0;
            xOrig = 22.0;
            zOrig = -60.0;
        }

// 2170 -- 2700
    }
};
