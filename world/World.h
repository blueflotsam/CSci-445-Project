/**************************************************/
/* Name: David Andrew Wilson IV                   */
/*                                                */
/* Draws the environment                          */
/*                                                */
/* CSci 446 / fall 2019                           */
/**************************************************/

#include "../Harezini.h"


#define ROAD_FILE 	"./world/road_texture.ppm"
#define GRASS_FILE 	"./world/grass_texture.ppm"
#define SKY_FILE		"./world/sky_texture.ppm"
#define WORLD_TEXTURE_NUM 3

class World
{
	private:

	// Position values
	float xOrig;
	float yOrig;
	float zOrig;

	// Textures:
	int numTextures; // set in constructor via WORLD_TEXTURE_NUM
	GLuint texture[WORLD_TEXTURE_NUM];
	static const int ROAD = 0;
	static const int GRASS = 1;
	static const int SKY = 2;

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

		// Load road texture
		texture_file = (char *)ROAD_FILE;
		texture_index = ROAD;
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

		// Load grass texture
		texture_file = (char *)GRASS_FILE;
		texture_index = GRASS;
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

		// Load sky texture
		texture_file = (char *)SKY_FILE;
		texture_index = SKY;
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

	static void drawRoad(float xSize, float zSize){
		// Texture Change
		#if RAYGL == 1
			rayglScaleTexture(xSize, xSize, 1);
			rayglTranslateTexture(0, 0, 0);
			rayglRotateTexture(0, 0, 0);
			rayglTextureType(0);
		#endif

		xSize /= 2;
		zSize /= 2;

		// Rotates as if looking down, then draws it looking head on
		glPushMatrix();
			glRotatef(90, 1.0, 0.0, 0.0);
			glBegin(GL_QUADS);
				glMaterialfv(GL_FRONT, LIGHTING_TYPE, GREEN);
				glNormal3f( 0.0, 0.0, 1.0);
				glTexCoord2f(0.0, 0.0);
				glVertex3f(-xSize, zSize, 0.0);
				glTexCoord2f(1.0, 0.0);
				glVertex3f( xSize, zSize, 0.0);
				glTexCoord2f(1.0, zSize/xSize);
				glVertex3f( xSize,-zSize, 0.0);
				glTexCoord2f(0.0, zSize/xSize);
				glVertex3f(-xSize,-zSize, 0.0);
			glEnd();
		glPopMatrix();
	}
	
	static void drawGrass(float xSize, float zSize){
		// Texture Change
		#if RAYGL == 1
			rayglScaleTexture(10, 10, 1);
			rayglTranslateTexture(0, 0, 0);
			rayglRotateTexture(0, 0, 0);
			rayglTextureType(0);
		#endif

		xSize /= 2;
		zSize /= 2;

		// Rotates as if looking down, then draws it looking head on
		glPushMatrix();
			glRotatef(-90, 1.0, 0.0, 0.0);
			glBegin(GL_QUADS);
				glMaterialfv(GL_FRONT, LIGHTING_TYPE, GREEN);
				glNormal3f( 0.0, 0.0, 1.0);
				glTexCoord2f(0.0, zSize);
				glVertex3f(-xSize, -zSize, 0.0);
				glTexCoord2f(xSize, zSize);
				glVertex3f( xSize, -zSize, 0.0);
				glTexCoord2f(xSize, 0.0);
				glVertex3f( xSize, zSize, 0.0);
				glTexCoord2f(0.0, 0.0);
				glVertex3f(-xSize, zSize, 0.0);
			glEnd();
		glPopMatrix();
	}

	static void drawSky(float xSize, float zSize){
		// Texture Change
		#if RAYGL == 1
			rayglScaleTexture(xSize, zSize, 1);
			rayglTranslateTexture(0, 0, 0);
			rayglRotateTexture(0, 0, 0);
			rayglTextureType(0);
		#endif

		xSize /= 2;
		zSize /= 2;

		glPushMatrix();
			for (int i = 0; i < 4; i++) {
				glBegin(GL_QUADS);
					glMaterialfv(GL_FRONT, LIGHTING_TYPE, BLUE);
					glNormal3f( 0.0, 0.0, 1.0);
					glTexCoord2f(0.0, 1.0);
					glVertex3f(-xSize,   0.0,-zSize);
					glTexCoord2f(1.0, 1.0);
					glVertex3f( xSize,   0.0,-zSize);
					glTexCoord2f(1.0, 0.0);
					glVertex3f( xSize, xSize + zSize,-zSize);
					glTexCoord2f(0.0, 0.0);
					glVertex3f(-xSize, xSize + zSize,-zSize);
				glEnd();
				glRotatef(90.0, 0.0, 1.0, 0.0);
			}
		glPopMatrix();
	}
    

    /*
            ***
           *****
           *****
            ***
             *
             *
             *
             @
             
        @ = (xpos, ypos, zpos) coordinate in relation to the tree
    */
    static void drawTree(float xpos, float ypos, float zpos,float tHeight){
        glTranslatef(xpos, ypos, zpos);
        GLUquadricObj *leaves;
        GLUquadricObj *trunk;
        leaves=gluNewQuadric();
        trunk=gluNewQuadric();
        float leavesRadius=5;
        float leavesSharpness=9;
        float trunkRadius=1;
        float trunkHeight=tHeight;
        float trunkSharpness=14;
        glMaterialfv(GL_FRONT, LIGHTING_TYPE, BROWN);
        glRotatef(90,-90,0,0);
        gluCylinder(trunk,trunkRadius,trunkRadius,trunkHeight,trunkSharpness,trunkSharpness);
        glMaterialfv(GL_FRONT, LIGHTING_TYPE, LEAFGREEN);
        glTranslatef(0,0,trunkHeight);
        gluSphere(leaves,leavesRadius,leavesSharpness,leavesSharpness);        
    }

	public:

	// Constructors
	World(){
		this->xOrig = 0;
		this->yOrig = 0;
		this->zOrig = 0;
		this->numTextures = WORLD_TEXTURE_NUM;
		loadTextures();
	}
	World(float xOrig, float yOrig, float zOrig){
		this->xOrig = xOrig;
		this->yOrig = yOrig;
		this->zOrig = zOrig;
		this->numTextures = WORLD_TEXTURE_NUM;
		loadTextures();
	}

	void draw(){
		// turn on textures
		glEnable(GL_TEXTURE_2D);
		// move to center location of object
		glTranslatef(xOrig, yOrig, zOrig);
		// Sky
		glBindTexture(GL_TEXTURE_2D, texture[SKY]);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		drawSky(1000.0, 1000.0);
		// Road
		glBindTexture(GL_TEXTURE_2D, texture[ROAD]);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		drawRoad(10.0, 1000.0);
		// Grass
		glBindTexture(GL_TEXTURE_2D, texture[GRASS]);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glTranslatef(-252.5, 0.0, 0.0);
		drawGrass(495.0, 1000.0);
		glTranslatef( 505.0, 0.0, 0.0);
		drawGrass(495.0, 1000.0);
        //reading treeSpacing file to see where to draw trees
        std::ifstream file;
        file.open("treeSpacing.txt");
        float x,y,z,h;
	glDisable(GL_TEXTURE_2D);
        while(file>>x>>y>>z>>h){
            glPushMatrix();
            glTranslatef(-252,0,0);
            drawTree(x,y,z,h);
            glPopMatrix();
        }
	glEnable(GL_TEXTURE_2D);

	}
};
