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
    float xrot=0;
    float yrot=0;
    float zrot=0;
    float angle=0;
    //Other global variables
    double rate=2.5;
	float d[3];
	float times;//what time in the animation the object is in, incriments by one for each movement
	float ambientStrength;
	GLuint ConeMap;
	bool animate;
	bool upwards;
	GLUquadricObj *sphere;

	void TEXTURE_MODEL(void){
		int w, h, c;
		char *arr=(char*)CARROT_FILE;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		unsigned char *data= stbi_load(arr, &w,&h,&c,0);
		glGenTextures(1, &ConeMap);
		glBindTexture(GL_TEXTURE_2D, ConeMap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}

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
		TEXTURE_MODEL();
	}

/*void animate(bool stage){
animate=stage;
}*/

	void drawCarrot (void){
		//TEXTURE_MODEL(); Don't reload the images every time the carrot is drawn
        glScalef(.5,.5,.5);
		glTranslatef(xpos,ypos,zpos);
        glRotatef(angle,xrot,yrot,zrot);
		glTranslatef(.01*times,0,0);
		glTranslatef(-.8,0,0);
		glDisable(GL_TEXTURE_2D);
		gluQuadricTexture(sphere, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D, ConeMap);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glEnable(GL_COLOR_MATERIAL);
		glColor3ub(0,50,0);//green
		//GLfloat green[] = { .3215,.5196,.1765, 1 };
		GLfloat green[]={0,100,0,1};
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
    
    void carrotRotation(float a,float x, float y, float z){
        angle=a;xrot=x;yrot=y;zrot=z;
    }

    void carrotRotation(float x, float y, float z){
        xrot=x;yrot=y;zrot=z;
    }

    void carrotAnimation(double input){
    animate=input;
    drawCarrot();
    }

	void idle (int frame){
		//frame by frame animation goes here
        carrotAnimation(times+=rate);
        if(times>100||times<0)
            rate*=-1;
	}
};
