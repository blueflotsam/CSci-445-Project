#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

/***********************************************************/
/* GLOBAL DEFINES                                          */
/***********************************************************/
#define version   "1.6"

/***********************************************************/
/* GLOBAL VARIABLES                                        */
/***********************************************************/
int  CorePerNode;
char inputExtension[10];
char outputExtension[10];
char dataDir[80];

struct thread_data {
   char sysCommand[256];
   int frame;
   int status;
};

/***********************************************************/
/* PROTOTYPES                                              */
/***********************************************************/
void READ_INI_FILE(char[]);
void MAKE_POV_LIST(void);
void MAKE_PNG_LIST(void);
void COMPARE_POV_PNG_LISTS(void);
void CREATE_COMMAND(FILE*, int*, char[], char[], int);
