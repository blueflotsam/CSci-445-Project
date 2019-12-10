/**************************************************/
/* Name: Cory Haas                                */
/*                                                */
/* Defines the Carrot class                       */
/*                                                */
/* CSci 446 / fall 2019                           */
/**************************************************/
#include "../Harezini.h"

#define CARROT_FILE "./carrot/carrot2.ppm"

class Carrot
{
	private:

	// Location
	float xpos;
	float ypos;
	float zpos;
    //Rotation
    float xrot;
    float yrot;
    float zrot;
    float angle;
    //Other global variables
    double rate;
	float d[3];
    float size;
	float times;//what time in the animation the object is in, incriments by one for each movement
	float ambientStrength;
	bool animate;
	bool upwards;
	GLUquadricObj *sphere;

	GLuint texture[1];
	int numTextures;

	/*
			Loads the textures to be used later
	*/
	void loadTextures() {
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

		// Load Carrot texture
		texture_file = (char *)CARROT_FILE;
		texture_index = 0;
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

	/*void TEXTURE_MODEL(void){
		int w, h, c;
		char *arr=(char*)CARROT_FILE;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		unsigned char *data= stbi_load(arr, &w,&h,&c,0);
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}*/

	void drawTriangle(float x, float y, float z, float y1, float z1){
		float midz=(z+z1)/2;
		float midy=(y+y1)/2;
		glBegin (GL_TRIANGLES);
		glVertex3f( 0, y, z );
		glVertex3f( 0,y1 , z);
		glVertex3f( x, midy, midz);

		glVertex3f( 0, y, z );
		glVertex3f( 0,y , z1);
		glVertex3f( x, midy, midz);

		glVertex3f( 0, y1, z1 );
		glVertex3f( 0,y , z1);
		glVertex3f( x, midy, midz);

		glVertex3f( .4, y1, z1 );
		glVertex3f( .4,y1 , z);
		glVertex3f( x, midy, midz);
		glEnd();
	}

	public:

	// Constructor
	Carrot(float xpos, float ypos, float zpos){
		this->xpos = xpos;
		this->ypos = ypos;
		this->zpos = zpos;
		d[0] = d[1] = d[2] = 0.1; // d = [0.1, 0.1, 0.1]
		times=0;
		ambientStrength=.1;
		animate=false;
		upwards=true;
		sphere =gluNewQuadric();
		xrot=0;
    	yrot=0;
    	zrot=0;
    	angle=0;
    	rate=2.5;
        size=.2;
    	numTextures = 1;
		loadTextures();
	}

/*void animate(bool stage){
animate=stage;
}*/

	void drawCarrot (void){
		//TEXTURE_MODEL(); Don't reload the images every time the carrot is drawn
		glTranslatef(xpos,ypos,zpos);
        glScalef(size,size,size);
        glRotatef(xrot,1,0,0);
        glRotatef(yrot,0,1,0);    
        glRotatef(zrot,0,0,1);
		glTranslatef(.01*times,0,0);
		glTranslatef(-.8,0,0);
		glDisable(GL_TEXTURE_2D);
		gluQuadricTexture(sphere, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glEnable(GL_COLOR_MATERIAL);
		glColor3ub(0,50,0);//green
		//GLfloat green[] = { .3215,.5196,.1765, 1 };
		//GLfloat green[]={0,100,0,1};
		//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, green);
		//glMaterialfv(GL_FRONT, GL_EMISSION, green);
		drawTriangle(-7,.5,.5,-.5,-.5);
		drawTriangle(-5,-.5,.5,-1.5,-.5);
		drawTriangle(-5,.5,.5,1.5,-.5);
		drawTriangle(-5,.5,-.5,-.5,-1.5);
		drawTriangle(-5,.5,.5,-.5,1.5);
		glEnd();
		float radius =2;
		float percent=.1;
		float sharpness=16;
		float decrease=percent*radius;
		glColor3f(0.929411765,0.568627451,0.011764706);//orange
		GLfloat orange[] = { 0.929411765,0.568627451,0.011764706, 1 };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, orange);
		//glMaterialfv(GL_FRONT, GL_SPECULAR, orange);
		glEnable(GL_TEXTURE_2D);
		for(float i=0; i<9;i++){
			if(i==0);
			else if(i<4){
				glTranslatef(0.003*times,.01*times,0);
			}
			else if(i>4){
				glTranslatef(-.005*times,-.01*times,0);
			}

			if(i!=0) glTranslatef(radius,0,0);
			radius-=decrease;
			gluSphere(sphere,radius, sharpness, sharpness);
			decrease=radius*percent;
		}

		glEnd();

		//animation part
		if(animate){
			if(upwards) times+=.01;
			else times-=.01;
			if(times>=100) upwards=false;
			else if(times<=0) upwards=true;
			glutPostRedisplay();
		}
		glDisable(GL_COLOR_MATERIAL);
	}

    void carrotPosition(float x,float y,float z){
        xpos=x;ypos=y;zpos=z;
    }

    void carrotAnimation(double input){
    animate=input;
    drawCarrot();
    }

	void idle (int frame){
        if(frame<2880){
            //do nothing
        }
        else if(frame==2880){
            zrot-=90;
            carrotPosition(23,35,-65);
            
        }
        else if(frame<2940){
            ypos-=.68;
        }
        else if(frame<2950){
            zrot-=9.01;
        }
	}
};
