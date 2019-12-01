#include "raygl.h"
/*************************************************************************

File: raygl.cpp
Author: Kris Zarns
Contact: kzarns@gmail.com
Revision: 39

 *	be careful what you call between glbegin/end sequences.
 *	this can have catastrophic effects on memory!!!!!!!!!!!

	basic form...

state:		keep track of the glstate machine...
	
glbegin:	grab type and initialize
glvertex:	grab vertices buildup object
glend:		end and write out object vertices and modifiers to file

basic clipping setup by Tyler Kaye

*/

/*************************************************************************
class variables
begin -- tracks whether we are inside a glBegin/glEnd state
type	-- tracks the type of object being drawn in a glBegin/glEnd
			-- 0, rest state no type
			-- 1, points approximated as spheres
			-- 2, lines implemented with splines
			-- 3, polygons as polygons
*************************************************************************/
//the file
FILE* rayF;

//glstate variables
//GLdouble 
GLdouble color[4] = {0.0,0.0,0.0,0.0};

//raygl state variables
GLint begin = 0;
GLint vertexCounter = 0;
GLenum rayMode = 0;
GLint fileCounter = 0;
GLdouble rayAspect = 1.33;
string rayFov = "";
string rayLookat = "camera{\n	location <0,0,0>\n	sky<0,1,0>\n	look_at <0,0,-100>\n	up	<0,1,0>\n	right	<-1.333,0,0>\n}\n";
GLfloat rayFadeDistance = 100;
GLfloat rayFadePower = 1.4;
GLboolean renderPov = true;
//raygl storage variables
GLboolean colorMat[1];
string output = "";
string output2 = "";
string firstPolyVertex = "";
string secondPolyVertex = "";
string thirdPolyVertex = "";
string fourthPolyVertex = "";
GLdouble locCoords[16];
GLfloat materialAmb[4];
GLfloat materialDif[4];
GLfloat materialSpc[4];
GLfloat materialEms[4];
GLfloat materialShi[1];
GLuint boundTexture = 0;
GLboolean texEnabled = false;
//tex coords stuff
GLdouble texScaleX = 1;
GLdouble texScaleY = 1;
GLdouble texScaleZ = 1;
GLdouble texTransX = 0;
GLdouble texTransY = 0;
GLdouble texTransZ = 0;
GLdouble texRotX = 1;
GLdouble texRotY = 1;
GLdouble texRotZ = 1;
GLfloat  texRot[3] = {0,0,0};
GLint texType = 0;

//Rick Fillion's texture vars
GLint rayglTextureEnabled = 0;
string rayglTextureString = "";
bool debug = true;

//Tyler Kaye's clipping Vars
GLdouble locCoords2[16];
GLboolean clipEnabled[1];
GLint maxClip[1];
GLdouble eqn[4];


vector<Image*> iVector;

/*************************************************************************
function rayglFrameBegin
this function takes a fileName parameter
it creates a file to write ray code to, then writes out the basics
setup for camera and light
the file is closed by rayglEnd()
*************************************************************************/
void rayglFrameBegin(char* fileName, GLint eyeX, GLint eyeY, GLint eyeZ, GLint centerX,GLint centerY, GLint centerZ, GLint wX, GLint wY, GLint wZ){

	GLint i = 0;
	//guaranteed 8
	GLint maxLights[] = {8};
	GLfloat lColAmbAccum[4] = {0,0,0,0};
	// note the reverse Z coordinate definintion
	string fileTemp = fileName;
	stringstream ssout("");
	ssout<<fileTemp<<setfill('0')<<setw(10)<<fileCounter<<".pov";

	fileTemp = ssout.str();
	printf("frame %d ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ\n",fileCounter);
	fileCounter++;

	if(renderPov){
		if(!(rayF = fopen(fileTemp.c_str(),"w"))){
			int errsv = errno;
			printf("file problem error line %d errno %d %s\n", __LINE__,errsv,strerror(errno));
		}
		GLfloat backCol[4];
		glGetFloatv(GL_COLOR_CLEAR_VALUE,backCol);
		fprintf(rayF, "#declare cam_locx = %d;\n", eyeX);
		fprintf(rayF, "#declare cam_locy = %d;\n", eyeY);
		fprintf(rayF, "#declare cam_locz = %d;\n", eyeZ);			 
		fprintf(rayF, "#declare cam_lookx = %d;\n", centerX);
		fprintf(rayF, "#declare cam_looky = %d;\n", centerY);
		fprintf(rayF, "#declare cam_lookz = %d;\n", centerZ);
		fprintf(rayF, "#declare cam_rx = 0;\n");
		fprintf(rayF, "#declare cam_ry = 0;\n");
		fprintf(rayF, "#declare cam_rz = 0;\n");

		fprintf(rayF, "camera {\n");
		fprintf(rayF, "	orthographic\n");
		fprintf(rayF, "	location <cam_locx, cam_locy, cam_locz>\n");
		fprintf(rayF, "	up	<0,%d,0>\n",wY);
		fprintf(rayF, "	right	<-%d,0,0>\n",wX);	
		fprintf(rayF, "	rotate <cam_rx, cam_ry, cam_rz>\n");
		fprintf(rayF, "	look_at <cam_lookx, cam_looky, cam_lookz>\n");
		fprintf(rayF, "}\n");

//initialize light sources here

		//search for enabled lights and create as per specs.
		printf("background {color rgb<%f,%f,%f>}\n",backCol[0],backCol[1],backCol[2]);
		fprintf(rayF, "background {color rgb<%f,%f,%f>}\n",backCol[0],backCol[1],backCol[2]);
		glGetIntegerv(GL_MAX_LIGHTS,maxLights);
//	printf("lights %d\n\n",maxLights[0]);
		for(i = 0;i < maxLights[0] && glIsEnabled(GL_LIGHTING); i++){
			if(glIsEnabled(GL_LIGHT0 + i)){
				GLfloat lColAmb[4];
				GLfloat lColDif[4];
				GLfloat lColSpe[4];	
				GLfloat lPos[4];
				GLfloat lSpotDir[3];
				GLint lSpotExp[1];
				GLint lSpotCut[1];
				GLint lConstAtten[1];
				GLint lLineAtten[1];
				GLint lQuadAtten[1];

				//ambient details
				glGetLightfv(GL_LIGHT0+i,GL_AMBIENT,lColAmb);
				//diffuse details
				glGetLightfv(GL_LIGHT0+i,GL_DIFFUSE,lColDif);
				//specular details
				glGetLightfv(GL_LIGHT0+i,GL_SPECULAR,lColSpe);
				glGetLightfv(GL_LIGHT0+i,GL_POSITION,lPos);
				glGetLightfv(GL_LIGHT0+i,GL_SPOT_DIRECTION,lSpotDir);
				glGetLightiv(GL_LIGHT0+i,GL_SPOT_EXPONENT,lSpotExp);
				glGetLightiv(GL_LIGHT0+i,GL_SPOT_CUTOFF,lSpotCut);
				//attenuation for all local lights
				glGetLightiv(GL_LIGHT0+i,GL_CONSTANT_ATTENUATION,lConstAtten);
				glGetLightiv(GL_LIGHT0+i,GL_LINEAR_ATTENUATION,lLineAtten);
				glGetLightiv(GL_LIGHT0+i,GL_QUADRATIC_ATTENUATION,lQuadAtten);
				if(lColAmbAccum[0] < lColAmb[0])
					lColAmbAccum[0] = lColAmb[0];
				if(lColAmbAccum[1] < lColAmb[1])
					lColAmbAccum[1] = lColAmb[1];
				if(lColAmbAccum[2] < lColAmb[2])
					lColAmbAccum[2] = lColAmb[2];
				if(lColAmbAccum[3] < lColAmb[3])
					lColAmbAccum[3] = lColAmb[3];
			/*	
			 *	ambient????
				fprintf(rayF, "light_source {\n");
				fprintf(rayF, "	<%d, %d, %d>\n", lPos[0],lPos[1],lPos[2]);
				fprintf(rayF, "	color rgb <%f, %f, %f>\n",lColDif[0],lColDif[1],lColDif[2]);
				fprintf(rayF, "	pointlight\n");
				//fprintf(rayF, "	point_at <0,0,0>\n");
				fprintf(rayF, "}\n\n");
				*/

				//Diffuse
				fprintf(rayF, "light_source {\n");
				fprintf(rayF, "	<%f, %f, %f>\n", lPos[0],lPos[1],lPos[2]);
				fprintf(rayF, "	color rgb <%f, %f, %f>\n",lColDif[0],lColDif[1],lColDif[2]);
//			fprintf(rayF, "	pointlight\n");
				//fprintf(rayF, "	point_at <0,0,0>\n");
				fprintf(rayF, "}\n\n");

				//Specular/Spotlight
				fprintf(rayF, "light_source {\n");
				fprintf(rayF, "	<%f, %f, %f>\n", lPos[0],lPos[1],lPos[2]);
				fprintf(rayF, "	color rgb <%f, %f, %f>\n",lColDif[0],lColDif[1],lColDif[2]);
				fprintf(rayF, "	spotlight\n");
				fprintf(rayF, "	radius		%f \n",lSpotCut[0]*2/3.0);
				//+lSpotExponent[0]));
				fprintf(rayF, "	tightness	%d \n",lSpotExp[0]);
				fprintf(rayF, "	falloff		%d \n",lSpotCut[0]);
			
				fprintf(rayF, "	point_at <%f,%f,%f>\n",
									 lPos[0]+lSpotDir[0],lPos[1]+lSpotDir[1],lPos[2]+lSpotDir[2]);
				
				fprintf(rayF, "}\n\n");
			}
		}
		if(lColAmbAccum[0] == 0 && lColAmbAccum[1] == 0 && lColAmbAccum[2] == 0){
			fprintf(rayF, "global_settings {ambient_light rgb <1,1,1>}\n\n");
		}else{
			fprintf(rayF, "global_settings {ambient_light rgb <%f,%f,%f>}\n\n",lColAmbAccum[0],lColAmbAccum[1],lColAmbAccum[2]);
		}
	}
}
/*************************************************************************
function rayglFrameBegin
this function takes a fileName parameter
it creates a file to write ray code to, then writes out the basics
setup for camera and light
the file is closed by rayglEnd()
*************************************************************************/
void rayglFrameBegin(char* fileName){

	GLint i = 0;
	//guaranteed 8
	GLint maxLights[] = {8};
	GLfloat lColAmbAccum[4] = {0,0,0,0};
	// note the reverse Z coordinate definintion
	string fileTemp = fileName;
	stringstream ssout("");
	//ssout<<fileTemp<<fileCounter<<".pov";
	ssout<<fileTemp<<setfill('0')<<setw(10)<<fileCounter<<".pov";

	fileTemp = ssout.str();
	printf("frame %d ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ\n",fileCounter);	
	fileCounter++;
	if(renderPov){
		if(!(rayF = fopen(fileTemp.c_str(),"w"))){
			int errsv = errno;
			printf("file problem error line %d errno %d %s\n", __LINE__,errsv,strerror(errno));
		}
		GLfloat backCol[4];
		glGetFloatv(GL_COLOR_CLEAR_VALUE,backCol);

		//sets camera lookat...
		fprintf(rayF,"%s",rayLookat.c_str());
//initialize light sources here

		//search for enabled lights and create as per specs.	
		fprintf(rayF, "background {color rgb<%f,%f,%f>}\n",backCol[0],backCol[1],backCol[2]);
		glGetIntegerv(GL_MAX_LIGHTS,maxLights);
//	printf("lights %d\n\n",maxLights[0]);
		for(i = 0;i < maxLights[0] && glIsEnabled(GL_LIGHTING); i++){
			if(glIsEnabled(GL_LIGHT0 + i)){
				GLfloat lColAmb[4];
				GLfloat lColDif[4];
				GLfloat lColSpe[4];	
				GLfloat lPos[4];
				GLfloat lSpotDir[3];
				GLint lSpotExp[1];
				GLint lSpotCut[1];
				GLint lConstAtten[1];
				GLint lLineAtten[1];
				GLint lQuadAtten[1];

				//ambient details
				glGetLightfv(GL_LIGHT0+i,GL_AMBIENT,lColAmb);
				if(lColAmbAccum[0] < lColAmb[0])
					lColAmbAccum[0] = lColAmb[0];
				if(lColAmbAccum[1] < lColAmb[1])
					lColAmbAccum[1] = lColAmb[1];
				if(lColAmbAccum[2] < lColAmb[2])
					lColAmbAccum[2] = lColAmb[2];
				if(lColAmbAccum[3] < lColAmb[3])
					lColAmbAccum[3] = lColAmb[3];

				//diffuse details
				glGetLightfv(GL_LIGHT0+i,GL_DIFFUSE,lColDif);
				//specular details
				glGetLightfv(GL_LIGHT0+i,GL_SPECULAR,lColSpe);
				glGetLightfv(GL_LIGHT0+i,GL_POSITION,lPos);
				glGetLightfv(GL_LIGHT0+i,GL_SPOT_DIRECTION,lSpotDir);
				glGetLightiv(GL_LIGHT0+i,GL_SPOT_EXPONENT,lSpotExp);
				glGetLightiv(GL_LIGHT0+i,GL_SPOT_CUTOFF,lSpotCut);
				//attenuation for all local lights
				glGetLightiv(GL_LIGHT0+i,GL_CONSTANT_ATTENUATION,lConstAtten);
				glGetLightiv(GL_LIGHT0+i,GL_LINEAR_ATTENUATION,lLineAtten);
				glGetLightiv(GL_LIGHT0+i,GL_LINEAR_ATTENUATION,lQuadAtten);
				//printf("const atten %d, linear%d, quad%d\n",lConstAtten[0],lLineAtten[0],lQuadAtten[0]);
				//Diffuse
				fprintf(rayF, "light_source {\n");
				fprintf(rayF, "	<%f, %f, %f>\n", lPos[0],lPos[1],lPos[2]);
				fprintf(rayF, "	color rgb <%f, %f, %f>\n",lColDif[0],lColDif[1],lColDif[2]);
				fprintf(rayF, "	fade_distance %f\n",rayFadeDistance);
				fprintf(rayF, "	fade_power %f\n",rayFadePower);
				//fprintf(rayF, "	point_at <0,0,0>\n");
				fprintf(rayF, "}\n\n");

				//Specular/Spotlight
				fprintf(rayF, "light_source {\n");
				fprintf(rayF, "	<%f, %f, %f>\n", lPos[0],lPos[1],lPos[2]);
				fprintf(rayF, "	color rgb <%f, %f, %f>\n",lColSpe[0],lColSpe[1],lColSpe[2]);
				fprintf(rayF, "	spotlight\n");
				fprintf(rayF, "	radius		%f \n",lSpotCut[0]*2/3.0);
				//+lSpotExponent[0]));
				fprintf(rayF, "	tightness	%d \n",lSpotExp[0]);
				fprintf(rayF, "	falloff		%d \n",lSpotCut[0]);
			
				fprintf(rayF, "	point_at <%f,%f,%f>\n",
									 lPos[0]+lSpotDir[0],lPos[1]+lSpotDir[1],lPos[2]+lSpotDir[2]);
				
				fprintf(rayF, "}\n\n");
			}
		}
		fprintf(rayF, "global_settings {ambient_light rgb <%f,%f,%f>}\n\n",lColAmbAccum[0],lColAmbAccum[1],lColAmbAccum[2]);
	}
}

/*************************************************************************
function raygluPerspective 
	captures gluPerspective
*************************************************************************/
void raygluPerspective(GLdouble fovy,GLdouble aspect,GLdouble zNear,GLdouble zFar){
   gluPerspective(fovy,aspect,zNear,zFar);
	stringstream ssout("");
	ssout<<"degrees(2*atan("<<rayAspect<<"*tan(radians("<<fovy<<"/2))))";// div 2
	rayFov = ssout.str();
	rayAspect = aspect;
}

/*************************************************************************
function raygluLookAt
	captures gluLookAt 
*************************************************************************/
void raygluLookAt(GLdouble eyeX, GLdouble eyeY,GLdouble eyeZ,GLdouble centerX,GLdouble centerY,GLdouble centerZ,GLdouble upX,GLdouble upY,GLdouble upZ ){
	
	gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
	getAtribs();

	stringstream ssout("");
	//note up in opengl used as sky in povray not up.
	//up and right here are to do with aspect rati & coord system.
	//order is important
	ssout<<"camera{\n\
		  	  perspective\n\
		  	  location <"<<eyeX<<","<<eyeY<<","<<eyeZ<<">\n\
		  	  sky<"<<upX<<","<<upY<<","<<upZ<<">\n\
		  	  up	<0,1,0>\n\
		  	  right	<"<<-rayAspect<<",0,0>\n\
		  	  angle "<<rayFov<<"\n\
		  	  look_at <"<<centerX<<","<<centerY<<","<<centerZ<<">\n\
		  	  matrix\n\
		  	  <"<<locCoords[0]<<","<<locCoords[1]<<","<<locCoords[2]<<",\n\
			  "<<locCoords[4]<<","<<locCoords[5]<<","<<locCoords[6]<<",\n\
			  "<<locCoords[8]<<","<<locCoords[9]<<","<<locCoords[10]<<",\n\
			  "<<locCoords[12]<<","<<locCoords[13]<<","<<locCoords[14]<<">\n\
				}\n";
	rayLookat = ssout.str();
}

/*************************************************************************
function rayglFrameEnd 
	this function is used to signal the end of a file
*************************************************************************/
void rayglFrameEnd(){
	if(renderPov){
		if(!rayF){
			int errsv = errno;
			printf("file problem error line %d errno %d %s\n", __LINE__,errsv,strerror(errno));
		}
		fflush(rayF);
		fclose(rayF);
	}
}

/*************************************************************************
function rayglBegin
	this function is used to implement the standard glBegin and
	set the state of the begin variable to 1
*************************************************************************/
void rayglBegin(GLenum mode){
	//initialize vertexCounterer
	vertexCounter = 1;
	output = "";
	output2 = "";
	firstPolyVertex = "";	
	secondPolyVertex = "";	
	thirdPolyVertex = "";	
	fourthPolyVertex = "";	
	rayMode = mode;
	getAtribs();
	begin = 1;
//	glGetIntegerv(GL_TEXTURE_BINDING_2D,&tempint);
//			cout<<"bound texture "<<(GLuint)tempint<<endl;
	glBegin(mode);
}

/*************************************************************************
function rayglEnd 
	this function is used ot implement the standard glEnd and
	set the state of the begin variable to 0
*************************************************************************/
void rayglEnd(){
	glEnd();

	//wakka wakka, how does this happen before writeatribs below?
//	glGetFloatv(GL_CURRENT_NORMAL,texRot);

//	cout<<"normal after end "<<texRot[0]<<","<<texRot[1]<<","<<texRot[2]<<endl;
//	cout<<"adjusted "<<texRot[1]<<","<<texRot[0]<<","<<texRot[2]<<endl;
	if(renderPov){
	
		if(!rayF){
			int errsv = errno;
			printf("file problem error line %d errno %d %s\n", __LINE__,errsv,strerror(errno));
		}
		if(rayMode == GL_POLYGON){
			output.append(firstPolyVertex);	
			char* localOutput = new char[output.length()+1];
			strcpy(localOutput,output.c_str());
	
			fprintf(rayF,"polygon{\n	%d,\n%s\n",vertexCounter,localOutput);
			writeAtribs();		
			writeMatrix();
			delete(localOutput);
		}
	}
	begin = 0;
}
/*************************************************************************
function raygluDisk
	this function implements gluDisk 
*************************************************************************/
void raygluDisk(GLUquadric *quad, GLdouble inner, GLdouble outer, GLint slices, GLint loops){
	
	getAtribs();
	gluDisk(quad, inner, outer, slices, loops);
	if(renderPov){
		if(!rayF){
			int errsv = errno;
			printf("file problem error line %d errno %d %s\n", __LINE__,errsv,strerror(errno));
		}
		fprintf(rayF,"disc{\n	<0,0,0>, <0,0,1>, %.15f, %.15f\n",outer, inner);
		writeAtribs();
		writeMatrix();
	}
}

/*************************************************************************
function raygluSphere
	this function implements gluSphere
*************************************************************************/
void raygluSphere(GLUquadric *quad, GLdouble radius, GLint slices, GLint stacks){
	
	getAtribs();	  
	gluSphere(quad, radius, slices, stacks);

	if(renderPov){
	//clip test
		if(!rayF){
			int errsv = errno;
			printf("file problem error line %d errno %d %s\n", __LINE__,errsv,strerror(errno));
		}
		if(clipEnabled[0])
		{
			fprintf(rayF,"sphere{\n<0,0,0>, %.15f\n", radius);
			fprintf(rayF,"clipped_by{plane { <0, 1, 0>, 0}}\n");
			fprintf(rayF,"rotate x*-180\n");
			writeAtribs();
			writeMatrix();
		}
		else
		{
			fprintf(rayF,"sphere{\n<0,0,0>, %.15f", radius);
			writeAtribs();
			writeMatrix();
		}
	}
}

/*************************************************************************
function raygluCylinder
	this function implements gluCylinder
*************************************************************************/
void raygluCylinder(GLUquadric *quad, GLdouble base, GLdouble top, GLdouble height, GLint slices, GLint stacks){
	
	getAtribs();
	gluCylinder(quad, base, top, height, slices, stacks);

	if(renderPov){
		if(!rayF){
			int errsv = errno;
			printf("file problem error line %d errno %d %s\n", __LINE__,errsv,strerror(errno));
		}
		fprintf(rayF,"cone{\n<0,0,0>,%.15f, <0,0,%.15f>, %.15f open\n", base, height, top);
		writeAtribs();
		writeMatrix();
	}
}

/*************************************************************************
function rayglutSolidSphere
	this function implements gluSphere
  	untested	
*************************************************************************/
void rayglutSolidSphere(GLdouble radius, GLint slices, GLint stacks){

	getAtribs();  
	glutSolidSphere(radius, slices, stacks);

	if(renderPov){
		if(!rayF){
			int errsv = errno;
			printf("file problem error line %d errno %d %s\n", __LINE__,errsv,strerror(errno));
		}
		//clip test
		if(clipEnabled[0])
		{
			fprintf(rayF,"sphere{\n<0,0,0>, %.15f\n", radius);
			fprintf(rayF,"clipped_by{plane { <0, 1, 0>, 0}}\n");
			fprintf(rayF,"rotate x*-180\n");
			writeAtribs();
			writeMatrix();
		}
		else
		{
			fprintf(rayF,"sphere{\n<0,0,0>, %.15f", radius);
			writeAtribs();
			writeMatrix();
		}
	}
}

/*************************************************************************
function rayglutSolidCube
	this function implements glutSolidCube
  	untested	
*************************************************************************/
void rayglutSolidCube(GLdouble size){

	getAtribs();
	glutSolidCube(size);
	size /= 2;
	if(renderPov){
		if(!rayF){
			int errsv = errno;
			printf("file problem error line %d errno %d %s\n", __LINE__,errsv,strerror(errno));
		}
		fprintf(rayF,"box{\n<%.15f,%.15f,%.15f>, <%.15f,%.15f,%.15f>\n", -size,-size,size,size,size,-size);
		writeAtribs();
		writeMatrix();
	}
}

/*************************************************************************
function rayglutSolidCone
	this function implements glutSolidCone
	untested
*************************************************************************/
void rayglutSolidCone(GLdouble base, GLdouble height, GLint slices, GLint stacks){

	getAtribs();  
	glutSolidCone(base, height, slices, stacks);
	if(renderPov){
		if(!rayF){
			int errsv = errno;
			printf("file problem error line %d errno %d %s\n", __LINE__,errsv,strerror(errno));
		}
		fprintf(rayF,"cone{\n<0,0,0>,%.15f, <0,0,%.15f>, 0 \n", base, height);
		writeAtribs();
		writeMatrix();
	}
}

/*************************************************************************
function rayglutSolidTorus
	this function implements glutSolidTorus
*************************************************************************/
void rayglutSolidTorus(GLdouble innerRadius, GLdouble outerRadius, GLint nsides, GLint rings){

	getAtribs();
	glutSolidTorus(innerRadius, outerRadius, nsides, rings);
//	GLdouble sub = outerRadius - innerRadius;
//	GLdouble minor = sub/2;
//	GLdouble major = outerRadius - minor;

	if(renderPov){
		if(!rayF){
			int errsv = errno;
			printf("file problem error line %d errno %d %s\n", __LINE__,errsv,strerror(errno));
		}
		//clip test
		if(clipEnabled[0])
		{

		//Tyler's code begins
		//
		fprintf(rayF,"torus{\n%f, %f\n",outerRadius,innerRadius);// major, minor);
		fprintf(rayF,"rotate x*-90\n");  //rotate torus from POV-RAY standard to openGL standard
		writeAtribs();

		fprintf(rayF,"clipped_by{plane { <0, 1, 0>, 0}}\n"); //Clipping about the origin

		fprintf(rayF,"rotate x*-180\n");  //clipping in openGL and POV-RAY are done opposite

		glGetDoublev(GL_MODELVIEW_MATRIX,locCoords2);
	/*
		glPushMatrix();
			glLoadIdentity();
			glMultMatrixd(locCoords2);
			glRotatef (180.0, 1.0, 0.0, 0.0);
			glGetDoublev(GL_MODELVIEW_MATRIX,locCoords2);
		glPopMatrix();
	*/

		fprintf(rayF,"\n\
\tmatrix\n\
\t\t%.15f,%.15f,%.15f,\n\
\t\t%.15f,%.15f,%.15f,\n\
\t\t%.15f,%.15f,%.15f,\n\
\t\t%.15f,%.15f,%.15f>}\n",
		locCoords2[0],locCoords2[1],locCoords2[2],
		locCoords2[4],locCoords2[5],locCoords2[6],
		locCoords2[8],locCoords2[9],	locCoords2[10],
		locCoords2[12],locCoords2[13],locCoords2[14]);
		}
		else
		{
		fprintf(rayF,"torus{\n%f, %f\n",outerRadius,innerRadius);// major, minor);
		fprintf(rayF,"rotate x*-90\n");  //rotate torus to look like openGL
		writeAtribs();
		glGetDoublev(GL_MODELVIEW_MATRIX,locCoords2);

	/*	glPushMatrix();
			glLoadIdentity();
			glMultMatrixd(locCoords2);
			glRotatef (90.0, 1.0, 0.0, 0.0);
			glGetDoublev(GL_MODELVIEW_MATRIX,locCoords2);
		glPopMatrix();
	*/
		fprintf(rayF,"\n\
\tmatrix\n\
\t\t<%.15f,%.15f,%.15f,\n\
\t\t%.15f,%.15f,%.15f,\n\
\t\t%.15f,%.15f,%.15f,\n\
\t\t%.15f,%.15f,%.15f> }\n",
			locCoords2[0],locCoords2[1],locCoords2[2],
			locCoords2[4],locCoords2[5],locCoords2[6],
			locCoords2[8],locCoords2[9],	locCoords2[10],
			locCoords2[12],locCoords2[13],locCoords2[14]);
		}
		//End Tylers code
		//	fprintf(rayF,"	translate <-%.15f,-%.15f,-%.15f>\n",locCoords[12],locCoords[13],locCoords[14]);
		//	fprintf(rayF,"	rotate <90,0,0>\n");
		//	fprintf(rayF,"	translate <%.15f,%.15f,%.15f>\n}\n",locCoords[12],locCoords[13],locCoords[14]);
	}
}

/*************************************************************************
function getAtribs
	this function writes colors and coord transformations to a string
*************************************************************************/
void getAtribs(){

	//printf("in the beginning %d\n",begin);	  
	glGetDoublev(GL_MODELVIEW_MATRIX,locCoords);

	glGetBooleanv(GL_COLOR_MATERIAL,colorMat);
	if(colorMat[0]){
		//printf("color materials %d\n",begin);	  
		//		rayglInject("//material currently disabled\n");
		glGetDoublev(GL_CURRENT_COLOR,color);
		//printf("color materials %d\n",begin);	  
	}else{
		//printf("non color materials %d\n",begin);	  
		glGetMaterialfv(GL_FRONT,GL_AMBIENT,materialAmb);
		glGetMaterialfv(GL_FRONT,GL_DIFFUSE,materialDif);
		glGetMaterialfv(GL_FRONT,GL_SPECULAR,materialSpc);
		glGetMaterialfv(GL_FRONT,GL_EMISSION,materialEms);
		glGetMaterialfv(GL_FRONT,GL_SHININESS,materialShi);
		//printf("non-color materials %d\n",begin);
	}
	//printf("color materials were good %d\n",begin);

	texEnabled = glIsEnabled(GL_TEXTURE_2D);
	GLint tempint = 0;
	glGetIntegerv(GL_TEXTURE_BINDING_2D,&tempint);
	boundTexture = (GLuint)tempint;

	glGetIntegerv(GL_MAX_CLIP_PLANES,maxClip);
	glGetBooleanv(GL_CLIP_PLANE0 ,clipEnabled);
	if(clipEnabled[0])
	{
		glGetClipPlane(GL_CLIP_PLANE0,eqn);
	}
}

/*************************************************************************
function	rayglTexture
	this function adjusts the texture flag such that the user may enter pov
	textures by string.  Texture char * here is the string to be added to
	file.  Contributed by Rick Fillion
 * **********************************************************************/
void rayglTexture(char * texture){
	rayglTextureEnabled = 1;
 	rayglTextureString = texture;
}


/*************************************************************************
function	rayglDisableTexture
	this function adjusts the texture flag such that the opengl texture is
	used.  Contributed by Rick Fillion
 * **********************************************************************/
 void rayglDisableTexture(){
 	rayglTextureEnabled = 0;
 }

/*************************************************************************
function rayglScaleTexture
	this function adjusts the scale of a texture for non-unit size polygons
	Hopefully this will be able to be automated later.
	default value is 1.  This is global, so reset it for each object as
	needed.
*************************************************************************/
void rayglScaleTexture(GLdouble texX, GLdouble texY, GLdouble texZ){
	texScaleX = texX;
	texScaleY = texY;
	texScaleZ = texZ;
}

/*************************************************************************
function rayglTranslateTexture
	this function adjusts the tracking of a texture.
	Hopefully this will be able to be automated later.
	default value is 1/2, 1/2, 0.  This is global, so reset it for each
	bject as needed.
*************************************************************************/
void rayglTranslateTexture(GLdouble texX, GLdouble texY, GLdouble texZ){
	texTransX = texX;
	texTransY = texY;
	texTransZ = texZ;
}

/*************************************************************************
function rayglRotateTexture
	this function adjusts the rotation of a texture
	Hopefully this will be able to be automated later.
	default value is 0,0,0.  This is global, so reset it for each object as
	needed.
*************************************************************************/
void rayglRotateTexture(GLdouble texX, GLdouble texY, GLdouble texZ){
	texRotX = texX;
	texRotY = texY;
	texRotZ = texZ;
}

/**************************************************************************
function rayglTextureType
	adjusts the mapping type of the texture.  Default is 0, planar.  
	1 = spherical
	2 = cylindrical
	3,4 undefined
	5 = torus
	again, this is global, change on a per texture basis as needed.
**************************************************************************/
void rayglTextureType(GLint texTypeIn){
	texType = texTypeIn;
}


/*************************************************************************
function writeAtribs
	this function writes colors and materials to a file 
	note a getatribs must be called prior to this
	writeAtribs is begin/end safe
**************************************************************************/
void writeAtribs(){
	
	if(renderPov){
		if(!rayF){
			int errsv = errno;
			printf("file problem error line %d errno %d %s\n", __LINE__,errsv,strerror(errno));
		}
		//setup color
			//printf("start\n");
 		if (rayglTextureEnabled){
			fprintf(rayF,"\n\ttexture {\n");
			fprintf(rayF, "//set to use povray style texture\n");
 			fprintf(rayF, rayglTextureString.c_str());

			//needed to close texture
			fprintf(rayF,"\t}\n");
			//fprintf(rayF,"		}\n");
		}else{
		
			//for now lets check for texztures or materials
			//bound texture and texturing enabled?
			//wakka wakka is this a safe call???
//			GLint tempint = 0;
//			glGetIntegerv(GL_TEXTURE_BINDING_2D,&tempint);
			if(debug){cout<<"bound texture "<<boundTexture<<endl;}
			if(debug){cout<<"tex enabled "<<texEnabled<<endl;}
			if(texEnabled && boundTexture != 0){
				fprintf(rayF,"\n\ttexture {\n");
				//got a texture, look it up
				bool searching = true;
				for(vector<Image*>::iterator it = iVector.begin();it!=iVector.end()&&searching;++it){
					if((*it)->texName == boundTexture){
						searching = false;
						//found it
						//get rotation
						glGetFloatv(GL_CURRENT_NORMAL,texRot);

//	cout<<"normal after end in write "<<texRot[0]<<","<<texRot[1]<<","<<texRot[2]<<endl;
//	cout<<"adjusted in write "<<texRot[1]<<","<<texRot[0]<<","<<texRot[2]<<endl;
						fprintf(rayF,"\t\tpigment{\n\t\t\timage_map{\n\t\t\t\t%s \"%s\"\n\t\t\t\tmap_type %d\n\t\t\t}\n\t\t}\n\t\ttranslate<%.15f,%.15f,%.15f>\n\t\tscale<%.15f,%.15f,%.15f>\n\t\trotate<-90*%.15f*%.15f,90*%.15f*%.15f,90*%.15f*%.15f>\n",(*it)->texFileType,(*it)->texFileName,texType,texTransX,texTransY,texTransZ,texScaleX,texScaleY,texScaleZ,texRot[1],texRotX,texRot[0],texRotY,texRot[2],texRotZ);
					}
				}
				if(searching){
					cout<<"error applying texture, texture not found number "<<boundTexture<<endl;
				}
				
				//needed to close texture
				fprintf(rayF,"\t}\n");

			}else	if(colorMat[0]){
				//printf("if \n");
				fprintf(rayF,"\n\ttexture {\n");

			   rayglInject("//material disabled reverting to direct coloring\n");
				if(glIsEnabled(GL_BLEND)){
					fprintf(rayF,"			pigment{color	rgbf <%.15f,%.15f,%.15f,%.15f>}\n",color[0],color[1],color[2],(1-color[3]));
				}else{
					fprintf(rayF,"			pigment{color	rgb <%.15f,%.15f,%.15f>}\n",color[0],color[1],color[2]);
				}

				//needed to close texture
				fprintf(rayF,"\t}\n");
			}else{
				//printf("else\n");
				fprintf(rayF,"\n\ttexture {\n");
	
				//printf("pigment{color	rgb <%f,%f,%f>}\n",materialDif[0],materialDif[1],materialDif[2]);
				fprintf(rayF,"			pigment{color	rgb <%f,%f,%f>}\n",materialDif[0],materialDif[1],materialDif[2]);

				//begin finish block		
				fprintf(rayF,"			finish{\n");

				//ambient
				fprintf(rayF,"				ambient	rgb <%f,%f,%f>\n",
				materialAmb[0],materialAmb[1],materialAmb[2]);

				//diffuse
				fprintf(rayF,"				diffuse %f\n",(materialDif[0]+materialDif[1]+materialDif[2])/3);
	
				//specular
				fprintf(rayF,"				specular %f roughness %f\n",
					(materialSpc[0]+materialSpc[1]+materialSpc[2])/3,
					(128-materialShi[0])/1280);
	
				//end finish
				fprintf(rayF,"			}\n");
	
				//needed to close texture
				fprintf(rayF,"\t}\n");

				//end finish
				fprintf(rayF,"\
				interior{\n\
					media{\n\
						emission rgb <%f,%f,%f>\n\
					}\n\
				}\n",materialEms[0],materialEms[1],materialEms[2]);
			}
		}
	}	
}

/*
 *	support function not currently used
 */
void stringAtribs(string sret){
	stringstream ssout("");
	ssout<<"texture {\npigment{color	rgb <"<<color[0]<<","<<color[1]<<","<<
					color[2]<<">}\n	}\n	matrix\n"
				<<"<"<<color[0]<<","<<color[1]<<","<<color[2]<<","<<
				locCoords[0]<<","<<locCoords[1]<<","<<locCoords[2]<<","<<locCoords[4]<<","<<
				locCoords[5]<<","<<locCoords[6]<<","<<locCoords[8]<<","<<locCoords[9]<<","<<
				locCoords[10]<<","<<locCoords[12]<<","<<locCoords[13]<<","<<locCoords[14];
	sret = ssout.str();	
}

/*************************************************************************
function writeMatrix
	this function writes coord transformations to a file
*************************************************************************/
void writeMatrix(){

	if(renderPov){
		if(!rayF){
			int errsv = errno;
			printf("file problem error line %d errno %d %s\n", __LINE__,errsv,strerror(errno));
		}
		//setup matrix
		fprintf(rayF,"\
\tmatrix\n\
\t\t<%.15f,%.15f,%.15f,\n\
\t\t%.15f,%.15f,%.15f,\n\
\t\t%.15f,%.15f,%.15f,\n\
\t\t%.15f,%.15f,%.15f>\n}\n",
		locCoords[0],locCoords[1],locCoords[2],
		locCoords[4],locCoords[5],locCoords[6],
		locCoords[8],locCoords[9],	locCoords[10],
		locCoords[12],locCoords[13],locCoords[14]);
	}		  
}

/*************************************************************************
function rayglInject
	writes the parameter char array to the povray SDL file.
	strictly for use by people who have studied povray.
	I do not support the use of this function and will not assist anyone
  	in using it.  Not appropriate if the file hasn't been opened.
	Also useful for commenting pov files.  standard // / * style comments
	This function will segfault if called while the file is closed
*************************************************************************/
void rayglInject(char* inject){
	if(renderPov){
		if(!rayF){
			int errsv = errno;
			printf("file problem error line %d errno %d %s\n", __LINE__,errsv,strerror(errno));
		}
		fprintf(rayF,"%s",inject);
	}
}


/*************************************************************************
function setRenderPov
	allows user to turn file writing on and off.
	this function has yet to be tested thuroughly
*************************************************************************/
void setRenderPov(GLboolean on){
	renderPov = on;
}

/*************************************************************************
function getRenderPov
	allows user to turn file writing on and off.
	this function has yet to be tested thuroughly
*************************************************************************/
GLboolean getRenderPov(){
	return renderPov;
}

/*************************************************************************
function rayglFadeDistancet
	adjusts the distance at which light begins to fade
	should be relative to scene size somehow
	global across all lights currently
	default is 100
*************************************************************************/
void setFadeDistance(GLfloat distance){
	rayFadeDistance = distance;
}

/*************************************************************************
function rayglFadePower
	adjusts the power with which light fades
	good 1.4 - 2.0 real
	global across all lights currently
	default is 1.4
*************************************************************************/
void setFadePower(GLfloat power){
	rayFadePower = power;
}

/*************************************************************************
function rayglVertex3i 
	this function is used to implement the standard glvertex3i and 
	to add vertices to objects in SDL
*************************************************************************/
void rayglVertex3i(GLint localX,GLint localY,GLint localZ){
	glVertex3i(localX,localY,localZ);
	if(renderPov){
		if(!rayF){
			int errsv = errno;
			printf("file problem error line %d errno %d %s\n", __LINE__,errsv,strerror(errno));
		}
		stringstream ssout("");
		if(rayMode == GL_POINTS){

			//handle points as tiny spheres
			fprintf(rayF,"sphere{\n	<%i,%i,%i>, 1\n",localX, localY, localZ);


			glEnd();
			writeAtribs();
			writeMatrix();
			glBegin(GL_POINTS);
			
		}else if(rayMode == GL_TRIANGLES){
			ssout<<"	<"<<localX<<","<<localY<<","<<localZ<<">\n";
			output.append(ssout.str());
							
			if(vertexCounter %3==0){
				output.append(firstPolyVertex);
				char* localOutput = new char[output.length()+1];
				strcpy(localOutput,output.c_str());
				fprintf(rayF,"polygon{\n	4,\n%s\n",localOutput);

				glEnd();				
				writeAtribs();
				writeMatrix();
				glBegin(GL_TRIANGLES);

				output = "";
				firstPolyVertex = "";
				delete(localOutput);
			}else if(vertexCounter %4 == 1){
				firstPolyVertex.append(ssout.str());			
			}
			vertexCounter++;
		}else if(rayMode == GL_TRIANGLE_STRIP){

			ssout<<"	<"<<localX<<","<<localY<<","<<localZ<<">\n";
			
			if(vertexCounter == 1){

				firstPolyVertex.append(ssout.str());
			}else if(vertexCounter == 2){
				
				secondPolyVertex.append(ssout.str());
			}else if(vertexCounter %2 == 1){

				thirdPolyVertex = ssout.str();
				output.append(firstPolyVertex);
				output.append(secondPolyVertex);
				output.append(thirdPolyVertex);
				output.append(firstPolyVertex);

				char* localOutput = new char[output.length()+1];
				strcpy(localOutput,output.c_str());
			
				fprintf(rayF,"polygon{\n	4,\n%s\n",localOutput);
				writeAtribs();		
				writeMatrix();

				output = "";
				delete(localOutput);

			}else	if(vertexCounter %2 == 0){
		
				firstPolyVertex = secondPolyVertex;
				secondPolyVertex = thirdPolyVertex;
				thirdPolyVertex = ssout.str();
				output.append(secondPolyVertex);
				output.append(firstPolyVertex);
				output.append(thirdPolyVertex);
				output.append(secondPolyVertex);

				char* localOutput = new char[output.length()+1];
				strcpy(localOutput,output.c_str());
			
				fprintf(rayF,"polygon{\n	4,\n%s\n",localOutput);
				writeAtribs();
				writeMatrix();

				output = "";
				firstPolyVertex = secondPolyVertex;
				secondPolyVertex = thirdPolyVertex;
				delete(localOutput);
				
			}			  

			vertexCounter++;
		}else if(rayMode == GL_TRIANGLE_FAN){

			//char tempBuf [100];
//		sprintf(tempBuf, "<%i,%i,%i>\n",localX, localY, localZ);
			//if we have all the coords writeout
			ssout<<"	<"<<localX<<","<<localY<<","<<localZ<<">\n";
			
			if(vertexCounter >2){

				thirdPolyVertex = ssout.str();
				output.append(firstPolyVertex);
				output.append(secondPolyVertex);
				output.append(thirdPolyVertex);
				output.append(firstPolyVertex);

				char* localOutput = new char[output.length()+1];
				strcpy(localOutput,output.c_str());
			
				fprintf(rayF,"polygon{\n	4,\n%s\n",localOutput);
				writeAtribs();		
				writeMatrix();

				output = "";
				secondPolyVertex = thirdPolyVertex;
				delete(localOutput);
			}else if(vertexCounter == 1){

				firstPolyVertex.append(ssout.str());
			}else{
				
				secondPolyVertex.append(ssout.str());
			}

			vertexCounter++;
		
		}else if(rayMode == GL_QUADS){

			//char tempBuf [100];
//		sprintf(tempBuf, "<%i,%i,%i>\n",localX, localY, localZ);
			ssout<<"	<"<<localX<<","<<localY<<","<<localZ<<">\n";
			output.append(ssout.str());
							
			if(vertexCounter %4==0){//&& vertexCounter != 0){
				output.append(firstPolyVertex);
				char* localOutput = new char[output.length()+1];
				strcpy(localOutput,output.c_str());
				fprintf(rayF,"polygon{\n	5,\n%s\n",localOutput);

				glEnd();
				writeAtribs();
				writeMatrix();
				glBegin(GL_QUADS);

				output = "";
				firstPolyVertex = "";
				delete(localOutput);
			}else if(vertexCounter %4== 1){
				firstPolyVertex.append(ssout.str());			
			}
			vertexCounter++;
		}else if(rayMode == GL_QUAD_STRIP){

			//char tempBuf [100];
			//sprintf(tempBuf, "<%i,%i,%i>\n",localX, localY, localZ);
			//if we have all the coords writeout
			ssout<<"	<"<<localX<<","<<localY<<","<<localZ<<">\n";
			
			if(vertexCounter == 1){

				firstPolyVertex.append(ssout.str());
			}else if(vertexCounter == 2){
				
				secondPolyVertex.append(ssout.str());
			}else if(vertexCounter %2 == 1){

				thirdPolyVertex.append(ssout.str());			
			}else	if(vertexCounter %2 == 0){
				fourthPolyVertex.append(ssout.str());

				output.append(firstPolyVertex);
				output.append(secondPolyVertex);
				output.append(fourthPolyVertex);
				output.append(thirdPolyVertex);

				output.append(firstPolyVertex);
				char* localOutput = new char[output.length()+1];
				strcpy(localOutput,output.c_str());
		

				//wakka wakka sure wish we could export this	to rayglend
				fprintf(rayF,"polygon{\n	5,\n%s\n",localOutput);
				writeAtribs();		
				writeMatrix();

				output = "";
				firstPolyVertex = thirdPolyVertex;
				secondPolyVertex = fourthPolyVertex;
				thirdPolyVertex = "";
				fourthPolyVertex = "";
				delete(localOutput);
				
			}			  

			vertexCounter++;
			
		}else if(rayMode == GL_POLYGON){

			//check if this is the first vertex, if so remember it
			//increment the vertex counter
			//write to the output string
			//warning
			//consider revising tempbuf to fit any size. consider size of doubles
			//char tempBuf [100];
			//sprintf(tempBuf,"	<%i,%i,%i>\n",localX, localY, localZ);
			ssout<<"	<"<<localX<<","<<localY<<","<<localZ<<">\n";
			if(vertexCounter == 1)
				firstPolyVertex.append(ssout.str());
			vertexCounter++;
			output.append(ssout.str());
		}
	}	
}
/*************************************************************************
function rayglVertex3f 
	this function is used to implement the standard glvertex3f and 
	to add vertices to objects in SDL
*************************************************************************/
void rayglVertex3f(GLfloat localX,GLfloat localY,GLfloat localZ){
	glVertex3f(localX,localY,localZ);
	if(renderPov){
		if(!rayF){
			int errsv = errno;
			printf("file problem error line %d errno %d %s\n", __LINE__,errsv,strerror(errno));
		}
		stringstream ssout("");
		if(rayMode == GL_POINTS){

			fprintf(rayF,"sphere{\n	<%f,%f,%f>, 1\n",localX, localY, localZ);

			glEnd();
			writeAtribs();
			writeMatrix();
			glBegin(GL_POINTS);			

		}else if(rayMode == GL_TRIANGLES){
//		printf("should be doint it 2\n ");
			//char tempBuf [100];
			//sprintf(tempBuf, "<%f,%f,%f>\n",localX, localY, localZ);
			ssout<<"	<"<<localX<<","<<localY<<","<<localZ<<">\n";
			output.append(ssout.str());
			
			if(vertexCounter %3==0){//&&vertexCounter != 1){
//		printf("should be doint it 3\n ");
				output.append(firstPolyVertex);
				char* localOutput = new char[output.length()+1];
				strcpy(localOutput,output.c_str());
			
				fprintf(rayF,"polygon{\n	4,\n%s\n",localOutput);

				glEnd();
				writeAtribs();
				writeMatrix();
				glBegin(GL_TRIANGLES);

				output = "";
				firstPolyVertex = "";
//			firstPolyVertex.append(ssout.str());
				delete(localOutput);
			}else if(vertexCounter %3== 1){
//		printf("should be doin it 4\n ");
				firstPolyVertex.append(ssout.str());			
			}
			vertexCounter++;

		}else if(rayMode == GL_TRIANGLE_STRIP){

			//char tempBuf [100];
			//sprintf(tempBuf, "<%f,%f,%f>\n",localX, localY, localZ);
			//if we have all the coords writeout
			ssout<<"	<"<<localX<<","<<localY<<","<<localZ<<">\n";
			
			if(vertexCounter == 1){

				firstPolyVertex.append(ssout.str());
			}else if(vertexCounter == 2){
				
				secondPolyVertex.append(ssout.str());
			}else if(vertexCounter %2 == 1){

				thirdPolyVertex = ssout.str();
//			thirdPolyVertex.append(ssout.str());			
				output.append(firstPolyVertex);
				output.append(secondPolyVertex);
				output.append(thirdPolyVertex);
				output.append(firstPolyVertex);

				char* localOutput = new char[output.length()+1];
				strcpy(localOutput,output.c_str());
			
				fprintf(rayF,"polygon{\n	4,\n%s\n",localOutput);
				writeAtribs();		
				writeMatrix();

				output = "";
				delete(localOutput);

			}else	if(vertexCounter %2 == 0){
		
				firstPolyVertex = secondPolyVertex;
				secondPolyVertex = thirdPolyVertex;
				thirdPolyVertex = ssout.str();
				output.append(secondPolyVertex);
				output.append(firstPolyVertex);
				output.append(thirdPolyVertex);
				output.append(secondPolyVertex);

				char* localOutput = new char[output.length()+1];
				strcpy(localOutput,output.c_str());
			
				fprintf(rayF,"polygon{\n	4,\n%s\n",localOutput);
				writeAtribs();		
				writeMatrix();

				output = "";
				firstPolyVertex = secondPolyVertex;
				secondPolyVertex = thirdPolyVertex;
				delete(localOutput);
				
			}			  

			vertexCounter++;
		}else if(rayMode == GL_TRIANGLE_FAN){

			//char tempBuf [100];
			//sprintf(tempBuf, "<%f,%f,%f>\n",localX, localY, localZ);
			//if we have all the coords writeout
			ssout<<"	<"<<localX<<","<<localY<<","<<localZ<<">\n";
			
			if(vertexCounter >2){

				thirdPolyVertex = ssout.str();
				output.append(firstPolyVertex);
				output.append(secondPolyVertex);
				output.append(thirdPolyVertex);
				output.append(firstPolyVertex);

				char* localOutput = new char[output.length()+1];
				strcpy(localOutput,output.c_str());
			
				fprintf(rayF,"polygon{\n	4,\n%s\n",localOutput);
				writeAtribs();		
				writeMatrix();

				output = "";
				secondPolyVertex = thirdPolyVertex;
				delete(localOutput);
			}else if(vertexCounter == 1){

				firstPolyVertex.append(ssout.str());
			}else{
				
				secondPolyVertex.append(ssout.str());
			}

			vertexCounter++;

		}else if(rayMode == GL_QUADS){

			//always append, we use every vertex
			ssout<<"	<"<<localX<<","<<localY<<","<<localZ<<">\n";
			output.append(ssout.str());
			//if we have all the coords writeout
			if(vertexCounter %4==0){
				output.append(firstPolyVertex);
				char* localOutput = new char[output.length()+1];
				strcpy(localOutput,output.c_str());
			
				fprintf(rayF,"polygon{\n	5,\n%s\n",localOutput);

				glEnd();
				writeAtribs();
				writeMatrix();
				glBegin(GL_QUADS);				

				output = "";
				firstPolyVertex = "";
				if(debug){cout<<"quads 6"<<endl;}
				delete(localOutput);
				if(debug){cout<<"quads 7"<<endl;}
			}else if(vertexCounter %4 == 1){
				//if we are at the first vertex track it
				firstPolyVertex.append(ssout.str());			
				if(debug){cout<<"quads 8"<<endl;}
			}
			vertexCounter++;

				
		}else if(rayMode == GL_QUAD_STRIP){

			//char tempBuf [100];
			//sprintf(tempBuf, "<%f,%f,%f>\n",localX, localY, localZ);
			//if we have all the coords writeout
			ssout<<"	<"<<localX<<","<<localY<<","<<localZ<<">\n";
			
			if(vertexCounter == 1){

				firstPolyVertex.append(ssout.str());
			}else if(vertexCounter == 2){
				
				secondPolyVertex.append(ssout.str());
			}else if(vertexCounter %2 == 1){

				thirdPolyVertex.append(ssout.str());			
			}else	if(vertexCounter %2 == 0){
				fourthPolyVertex.append(ssout.str());

				output.append(firstPolyVertex);
				output.append(secondPolyVertex);
				output.append(fourthPolyVertex);
				output.append(thirdPolyVertex);
				output.append(firstPolyVertex);

				char* localOutput = new char[output.length()+1];
				strcpy(localOutput,output.c_str());
			
				fprintf(rayF,"polygon{\n	5,\n%s\n",localOutput);
				writeAtribs();		
				writeMatrix();

				output = "";
				firstPolyVertex = thirdPolyVertex;
				secondPolyVertex = fourthPolyVertex;
				thirdPolyVertex = "";
				fourthPolyVertex = "";
				delete(localOutput);
				
			}			  

			vertexCounter++;
			
		}else if(rayMode == GL_POLYGON){

			//check if this is the first vertex, if so remember it
			//increment the vertex counter
			//write to the output string
			//warning
			//consider revising tempbuf to fit any size. consider size of doubles
			//char tempBuf [100];
			//sprintf(tempBuf,"	<%f,%f,%f>\n",localX, localY, localZ);
			ssout<<"	<"<<localX<<","<<localY<<","<<localZ<<">\n";
			if(vertexCounter == 1)
				firstPolyVertex.append(ssout.str());
			vertexCounter++;
			output.append(ssout.str());
		}
	}	
}
/*************************************************************************
function rayglVertex3d 
	this function is used to implement the standard glvertex3d and 
	to add vertices to objects in SDL
*************************************************************************/
void rayglVertex3d(GLdouble localX,GLdouble localY,GLdouble localZ){
	glVertex3d(localX,localY,localZ);
	if(renderPov){
		if(!rayF){
			int errsv = errno;
			printf("file problem error line %d errno %d %s\n", __LINE__,errsv,strerror(errno));
		}
		stringstream ssout("");
		if(rayMode == GL_POINTS){
			
			fprintf(rayF,"sphere{\n	<%.15f,%.15f,%.15f>, 1\n",localX, localY, localZ);

			glEnd();
			writeAtribs();
			writeMatrix();
			glBegin(GL_POINTS);
											
		}else if(rayMode == GL_TRIANGLES){

			ssout<<"	<"<<localX<<","<<localY<<","<<localZ<<">\n";
			output.append(ssout.str());
			
			if(vertexCounter %3==0){
				output.append(firstPolyVertex);
				char* localOutput = new char[output.length()+1];
				strcpy(localOutput,output.c_str());
			
				fprintf(rayF,"polygon{\n	4,\n%s\n",localOutput);

				glEnd();
				writeAtribs();
				writeMatrix();
				glBegin(GL_TRIANGLES);

				output = "";
				firstPolyVertex = "";
				delete(localOutput);
			}else if(vertexCounter %3 == 1){
				firstPolyVertex.append(ssout.str());			
			}
			vertexCounter++;
		}else if(rayMode == GL_TRIANGLE_STRIP){

			//if we have all the coords writeout
	  		ssout<<"	<"<<localX<<","<<localY<<","<<localZ<<">\n";
			
			if(vertexCounter == 1){

				firstPolyVertex.append(ssout.str());
			}else if(vertexCounter == 2){
				
				secondPolyVertex.append(ssout.str());
			}else if(vertexCounter %2 == 1){

				thirdPolyVertex = ssout.str();
				output.append(firstPolyVertex);
				output.append(secondPolyVertex);
				output.append(thirdPolyVertex);
				output.append(firstPolyVertex);

				char* localOutput = new char[output.length()+1];
				strcpy(localOutput,output.c_str());
			
				fprintf(rayF,"polygon{\n	4,\n%s\n",localOutput);
				writeAtribs();		
				writeMatrix();

				output = "";
				delete(localOutput);

			}else	if(vertexCounter %2 == 0){
		
				firstPolyVertex = secondPolyVertex;
				secondPolyVertex = thirdPolyVertex;
				thirdPolyVertex = ssout.str();
				output.append(secondPolyVertex);
				output.append(firstPolyVertex);
				output.append(thirdPolyVertex);
				output.append(secondPolyVertex);

				char* localOutput = new char[output.length()+1];
				strcpy(localOutput,output.c_str());
			
				fprintf(rayF,"polygon{\n	4,\n%s\n",localOutput);
				writeAtribs();		
				writeMatrix();

				output = "";
				firstPolyVertex = secondPolyVertex;
				secondPolyVertex = thirdPolyVertex;
				delete(localOutput);
				
			}			  

			vertexCounter++;
		}else if(rayMode == GL_TRIANGLE_FAN){

			ssout<<"	<"<<localX<<","<<localY<<","<<localZ<<">\n";
			
			if(vertexCounter >2){

				thirdPolyVertex = ssout.str();
				output.append(firstPolyVertex);
				output.append(secondPolyVertex);
				output.append(thirdPolyVertex);
				output.append(firstPolyVertex);

				char* localOutput = new char[output.length()+1];
				strcpy(localOutput,output.c_str());
			
				fprintf(rayF,"polygon{\n	4,\n%s\n",localOutput);
				writeAtribs();		
				writeMatrix();

				output = "";
				secondPolyVertex = thirdPolyVertex;
				delete(localOutput);
			}else if(vertexCounter == 1){

				firstPolyVertex.append(ssout.str());
			}else{
				
				secondPolyVertex.append(ssout.str());
			}

			vertexCounter++;

		}else if(rayMode == GL_QUADS){

			ssout<<"	<"<<localX<<","<<localY<<","<<localZ<<">\n";
			output.append(ssout.str());
			
			if(vertexCounter %4==0){
				output.append(firstPolyVertex);
				char* localOutput = new char[output.length()+1];
				strcpy(localOutput,output.c_str());
			
				fprintf(rayF,"polygon{\n	5,\n%s\n",localOutput);

				glEnd();
				writeAtribs();
				writeMatrix();
				glBegin(GL_QUADS);
				
				output = "";
				firstPolyVertex = "";
				delete(localOutput);
			}else if(vertexCounter %4 == 1){
				firstPolyVertex.append(ssout.str());			
			}
			vertexCounter++;
		}else if(rayMode == GL_QUAD_STRIP){

			ssout<<"	<"<<localX<<","<<localY<<","<<localZ<<">\n";
			
			if(vertexCounter == 1){

				firstPolyVertex.append(ssout.str());
			}else if(vertexCounter == 2){
				
				secondPolyVertex.append(ssout.str());
			}else if(vertexCounter %2 == 1){

				thirdPolyVertex.append(ssout.str());			
			}else	if(vertexCounter %2 == 0){
				fourthPolyVertex.append(ssout.str());

				output.append(firstPolyVertex);
				output.append(secondPolyVertex);
				output.append(fourthPolyVertex);
				output.append(thirdPolyVertex);
				output.append(firstPolyVertex);

				char* localOutput = new char[output.length()+1];
				strcpy(localOutput,output.c_str());
			
				fprintf(rayF,"polygon{\n	5,\n%s\n",localOutput);
				writeAtribs();		
				writeMatrix();

				output = "";
				firstPolyVertex = thirdPolyVertex;
				secondPolyVertex = fourthPolyVertex;
				thirdPolyVertex = "";
				fourthPolyVertex = "";
				delete(localOutput);
				
			}			  

			vertexCounter++;
			
		}else if(rayMode == GL_POLYGON){

			//check if this is the first vertex, if so remember it
			//increment the vertex counter
			//write to the output string
			//warning
			//consider revising tempbuf to fit any size. consider size of doubles
			//char tempBuf [100];
			//sprintf(tempBuf,"	<%f,%f,%f>\n",localX, localY, localZ);
			ssout<<"<"<<localX<<","<<localY<<","<<localZ<<">\n";
			if(vertexCounter == 1)
				firstPolyVertex.append(ssout.str());
			vertexCounter++;
			output.append(ssout.str());
		}
	}	
}

/***********************************************************/
/* PGM FILE IO ROUTINES                                    */
/***********************************************************/
/****************************************/
/* Read PGM formatted image.            */
/****************************************/
void pgmCommentClear(FILE *disk){
uchar  ch;
//fpos_t *pos;
   fread(&ch, 1, 1, disk);
   if (ch != '#') {
      fseek(disk, -1, SEEK_CUR);
      return;
   }
   do {
      while (ch != '\n') fread(&ch, 1, 1, disk);
   } while (ch == '#');
   pgmCommentClear(disk);
}

uchar *PGM_FILE_READ(char *FileName, GLint *Width, GLint *Height, GLint *color) { 
//int   x, y, k = 0;
GLint   pmax;
char  ch;
char  type[3];
uchar *temp;
FILE *disk;
   if ((disk = fopen(FileName, "rb")) == NULL) {
      return NULL;
   }
   fscanf(disk, "%s", (char*)&type);
   if (!strcmp(type, "P6")) *color = 1;
   else *color = 0;
   fread(&ch, 1, 1, disk); 
   pgmCommentClear(disk);
   fscanf(disk, "%d", Width);
   fscanf(disk, "%d", Height);
   fscanf(disk, "%d", &pmax);
   fread(&ch, 1, 1, disk);
   if (*color == 1) {
       temp = (uchar *)calloc(*Height**Width, 3);
       fread(temp, 1, *Height**Width*3, disk);
   } else {
       temp = (uchar *)calloc(*Height**Width, 1);
       fread(temp, 1, *Height**Width, disk);
   }
   fclose(disk);
   return temp;
}

/****************************************/
/* Write PGM formatted image.           */
/****************************************/
void PGM_FILE_WRITE(char *FileName, uchar data[], GLint targetW, GLint targetH, GLint color) {
//int   x, y, k = 0;
FILE *disk;
   disk = fopen(FileName, "wb");
   if (color == 1) fprintf(disk, "P6\n");
              else fprintf(disk, "P5\n");
   fprintf(disk, "%d %d\n", targetW, targetH);
   fprintf(disk, "255\n");
   if (color == 1) fwrite(data, 1, targetH*targetW*3, disk);
              else fwrite(data, 1, targetH*targetW, disk);
   fclose(disk);
}

int imageLoad(char* fileName, Image* image){
	GLint width, height;
	GLint color;

	cout<<"texture file name: "<<fileName<<endl;

	image->data = PGM_FILE_READ(fileName, &width, &height, &color);

	image->texFileName = (char*)malloc(strlen(fileName)+1);
	strcpy(image->texFileName,fileName);
	image->sizeX = width;
	image->sizeY = height;
	image->texFileType = "ppm";
	iVector.push_back(image);
		
	return 1;
}

/*************************************************************************
function rayglCustomLoadedTexture
	user
	support for custom loaders for non-ppm type images and such
	pass in an image struct with all the fields set and it will be added to
	the image vector
*************************************************************************/
void rayglCustomLoadedTexture(Image* image){
	iVector.push_back(image);
}

/*************************************************************************
function rayglTexImage2D
	passive
	captures glTexImage2D, maps texture name to image object
*************************************************************************/
void rayglTexImage2D(GLenum target, GLint level, GLint internalFormat, GLint width, GLint height, GLint border, GLenum format, GLenum type, const GLvoid *pixels){

	glTexImage2D(target,level,internalFormat,width,height,border,format,type,pixels);
	
	//ok, now to map the current texture to a image object.
	//we will check the memory location of data and compare it with that of each iamge struct

	bool searching = true;
	if(debug){cout<<"teximage2d"<<endl;}
	for(vector<Image*>::iterator it = iVector.begin();it!=iVector.end()&&searching;++it){

		if(debug){cout<<"teximage2d for"<<endl;}
		//if(debug){cout<<"itptr " <<(*it)->data << "pixptr "<< pixels<< endl;}
		if(debug){(*it)->toString();}
		if((*it)->data == pixels){
			if(debug){cout<<"teximage2d if"<<endl;}
			searching = false;
			if(debug){cout<<"match text "<<endl;}
			(*it)->toString();
			GLint tempint = 0;
			glGetIntegerv(GL_TEXTURE_BINDING_2D,&tempint);
			//seems screwy getting a glint when we need a gluint, roll with it
//			cout<<"signed"<<tempint<<endl;
//			cout<<"unsigned"<<(GLuint)tempint<<endl;
			(*it)->texName = (GLuint)tempint;
			(*it)->toString();
		}
	}
	if(searching){
		cout<<"texture not found on glTexImage2D!!!"<<endl;
	} 
}


/*************************************************************************
function raygluBuild2DMipmaps 
	passive
	captures gluBuild2DMipmaps
	in case user used this instead of teximage2D
*************************************************************************/
void raygluBuild2DMipmaps(GLenum target, GLint internalFormat, GLint width, GLint height, GLenum format, GLenum type, const GLvoid *pixels){

	gluBuild2DMipmaps(target,internalFormat,width,height,format,type,pixels);
	
	//ok, now to map the current texture to a image object.
	//we will check the memory location of data and compare it with that of each iamge struct

	bool searching = true;
	if(debug){cout<<"teximage2d"<<endl;}
	for(vector<Image*>::iterator it = iVector.begin();it!=iVector.end()&&searching;++it){

		if(debug){cout<<"teximage2d for"<<endl;}
		//if(debug){cout<<"itptr " <<(*it)->data << "pixptr "<< pixels<< endl;}
		if(debug){(*it)->toString();}
		if((*it)->data == pixels){
			if(debug){cout<<"teximage2d if"<<endl;}
			searching = false;
			if(debug){cout<<"match text "<<endl;}
			(*it)->toString();
			GLint tempint = 0;
			glGetIntegerv(GL_TEXTURE_BINDING_2D,&tempint);
			//seems screwy getting a glint when we need a gluint, roll with it
//			cout<<"signed"<<tempint<<endl;
//			cout<<"unsigned"<<(GLuint)tempint<<endl;
			(*it)->texName = (GLuint)tempint;
			(*it)->toString();
		}
	}
	if(searching){
		cout<<"texture not found on glBuild2DMipmaps!!!"<<endl;
	} 
}

//implemented an extremly dirty hack to get around unsafe glget for normals
//calling an empty glend/begin around the get call...  so bad, fix this, find a way to only call writeatribs in 1 spot
//writeatribs happens during glbegin/end for non polygon types...
