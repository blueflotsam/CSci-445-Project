/**************************************************/
/* Name: Megan Giesler		                  */
/*                                                */
/* Draws the tophat and has animation for the hat */
/* Also has movement for the tophat for the movie */
/*                                                */
/* CSci 446 / fall 2019                           */
/**************************************************/

#include "../Harezini.h"

#define CHAIR_FILE "./chair/chair.ppm"
#define HAT_TEXTURE_NUM 1

class Chair
{
	private:
    float scaleX,scaleY,scaleZ;
    float degree;
	// Position values
	float xOrig;
	float yOrig;
	float zOrig;
    float x,y,z;

	// Textures:
	int numTextures; // gets set in constructor via HAT_TEXTURE_NUM
	GLuint texture[HAT_TEXTURE_NUM];
	static const int FELT = 0;

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
		texture_file = (char *)CHAIR_FILE;
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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, image[texture_index]->sizeX, image[texture_index]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image[texture_index]->data);
		#else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
		#endif
	}



	public:

	// Constructors
	Chair(float xOrig, float yOrig, float zOrig)
	{
		this->xOrig = xOrig;
		this->yOrig = yOrig;
		this->zOrig = zOrig;
		this->numTextures = HAT_TEXTURE_NUM;
        this->scaleX=.3;
        this->scaleY=.3;
        this->scaleZ=.3;
        this->degree=-90;
        this->x=0;
        this->y=0;
        this->z=0;
		loadTextures();
	}

	void draw(){
        glTranslatef(xOrig,yOrig,zOrig);
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
		
		glRotatef(x, 1, 0, 0);
		glRotatef(y, 0, 1, 0);
        glRotatef(z, 0, 0, 1);        
		glScalef(scaleX, scaleY, scaleZ);
		
        glPushMatrix();
            drawChair();
        glPopMatrix();
	    glDisable(GL_TEXTURE_2D);
	}

	void drawChair()
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
			//front
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
				glVertex3f(-1.8f, 0.2f, -1.8f);
				glVertex3f(1.8f, 0.2f, -1.8f);
				glVertex3f(1.8f, 3.5f, -1.8f);
				glVertex3f(-1.8f, 3.5f, -1.8f);

			//back
				glVertex3f(-1.8f, 0.2f, -2.0f);
				glVertex3f(1.8f, 0.2f, -2.0f);
				glVertex3f(1.8f, 3.5f, -2.0f);
				glVertex3f(-1.8f, 3.5f, -2.0f);

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

    
    void scale(float x, float y, float z){
        scaleX=x;
        scaleY=y;
        scaleZ=z;
    }

    void idle(int frame)
    {
       if(frame < 1470)
        {
            //do nothing
        }
        else if(frame == 1470){
            yOrig = 4.0;
        }
        else if(frame < 1597){
            // wait for hat to grab 
        }
        else if(frame < 1630){
            yOrig += .07;
            zOrig -= .015;
        }
        else if (frame < 1680){
            zOrig -= 0.1;
            yOrig += .05;
        }
        else if (frame < 1785){
            yOrig -= .1;
            //zOrig -=.1;
            xOrig -= 0.05;
        }
        
        
		    

    }
};

































