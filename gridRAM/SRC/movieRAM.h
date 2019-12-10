#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

/***********************************************************/
/* GLOBAL DEFINES                                          */
/***********************************************************/
#define version      "1.0"

/***********************************************************/
/* GLOBAL VARIABLES                                        */
/***********************************************************/
char includeTitle[10];
char framePath[80];    
char soundPath[80];
char titleFrame[80];   
int  titleFrameTime;
char UNDFrame[80];     
int  UNDFrameTime;
char creditsFrame[80]; 
int  creditsFrameTime;
char closeFrame[80];   
int  closeFrameTime; 
char sound[80];        
char video[80];        
char movie[80];

/***********************************************************/
/* PROTOTYPES                                              */
/***********************************************************/
void READ_INI_FILE(char[], char[]);
void MAKE_EXTRA_FRAMES(void);
void ENCODE_VIDEO(char[]);
void ENCODE_MOVIE(char[]);
