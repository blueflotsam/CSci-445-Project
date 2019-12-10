#include "movieRAM.h"

/***********************************************************/
/* COMMAND PARSE ROUTINES                                  */
/***********************************************************/
/****************************************/
/* Read movieRAM.ini file.              */
/****************************************/
void READ_INI_FILE(char vencoderString[256], char mencoderString[256]) {
int  i, j;
char header[80];
char junk;
FILE *ini;
   printf("*************************************************\n");
   printf("* MESSAGE - Reading movieRAM.ini file.          *\n");
   printf("*************************************************\n");
   if (!(ini=fopen("./movieRAM.ini", "r"))) {
      printf("ERROR - No movieRAM.ini file.\n");
      exit(0);
   } else {
      fscanf(ini, "%s %s", header, includeTitle);
      fscanf(ini, "%s %s", header, framePath);
      fscanf(ini, "%s %s", header, titleFrame);
      fscanf(ini, "%s %d", header, &titleFrameTime);
      fscanf(ini, "%s %s", header, UNDFrame);
      fscanf(ini, "%s %d", header, &UNDFrameTime);
      fscanf(ini, "%s %s", header, creditsFrame);
      fscanf(ini, "%s %d", header, &creditsFrameTime);
      fscanf(ini, "%s %s", header, closeFrame);
      fscanf(ini, "%s %d", header, &closeFrameTime);
      fscanf(ini, "%s %s", header, soundPath);
      fscanf(ini, "%s %s", header, sound);
      fscanf(ini, "%s %s", header, video);
      fscanf(ini, "%s %s", header, movie);
      fscanf(ini, "%s", header);
      fscanf(ini, "%s", header);
      fscanf(ini, "%s", header);
      fscanf(ini, "%c", &junk);
      i = 0;
      do {
         fscanf(ini, "%c", &vencoderString[i]);
         i++;
      } while (vencoderString[i-1] != '\n');
      vencoderString[--i] = '\0';

      fscanf(ini, "%s", header);
      fscanf(ini, "%s", header);
      fscanf(ini, "%s", header);
      fscanf(ini, "%c", &junk);

      i = 0;
      do {
         fscanf(ini, "%c", &mencoderString[i]);
         i++;
      } while (!feof(ini));
      mencoderString[--i] = '\0';
   }

   fclose(ini);
}


/****************************************/
/* Make extra frames.                   */
/****************************************/
void MAKE_EXTRA_FRAMES(void) {
int  i, j;
int  stop;
char title[80];
char number[16];
char sysCommand[80];
   printf("*************************************************\n");
   printf("* MESSAGE - Generating title & credits frames.  *\n");
   printf("*************************************************\n");
   for (j = 0; j < 4; j++) {
       if (j == 0) {
          stop = titleFrameTime*25;
          strcpy(title, titleFrame);
       }
       if (j == 1) {
          stop = UNDFrameTime*25;
          strcpy(title, UNDFrame);
       }
       if (j == 2) {
          stop = creditsFrameTime*25;
          strcpy(title, creditsFrame);
       }
       if (j == 3) {
          stop = closeFrameTime*25;
          strcpy(title, closeFrame);
       }
       for (i = 0; i < stop; i++) {
           strcpy(sysCommand, "cp ");
           strcat(sysCommand, framePath);
           strcat(sysCommand, "/");
           strcat(sysCommand, title);
           strcat(sysCommand, ".png");
           strcat(sysCommand, " ");
           strcat(sysCommand, title);
           sprintf(number, "%d", i);
           if (i < 10) strcat(sysCommand, "0");
           strcat(sysCommand, number);
           strcat(sysCommand, ".png");
           system(sysCommand);
       }
   }
}

/****************************************/
/* Generate video.                      */
/*                                      */
/* http://www.mplayerhq.hu/             */
/****************************************/
void ENCODE_VIDEO(char encoderString[256]) {
int  i, j;
char encoderCommand[256];
   printf("*************************************************\n");
   printf("* MESSAGE - Encoding video.                     *\n");
   printf("*************************************************\n");
   i = 0;
   j = 0;
   do {
      if (encoderString[i] == '$') break;
      encoderCommand[j] = encoderString[i];
      i++;
      j++;
   } while (encoderString[i] != '\0');
   encoderCommand[j] = '\0';
   strcat(encoderCommand, video);
   j += strlen(video);
   i+=2;
   do {
      encoderCommand[j] = encoderString[i];
      i++;
      j++;
   } while (encoderString[i] != '\0');
   encoderCommand[j] = '\0';

printf("%s\n", encoderString);

   system(encoderCommand);
}

/****************************************/
/* Merge sound & video.                 */
/*                                      */
/* http://www.mplayerhq.hu/             */
/****************************************/
void ENCODE_MOVIE(char encoderString[256]) {
int  i, j;
char encoderCommand[256];
   printf("*************************************************\n");
   printf("* MESSAGE - Merging video and sound.            *\n");
   printf("*************************************************\n");
   i = 0;
   j = 0;
   do {
      if (encoderString[i] == '$') break;
      encoderCommand[j] = encoderString[i];
      i++;
      j++;
   } while (encoderString[i] != '\0');
   encoderCommand[j] = '\0';
   strcat(encoderCommand, movie);
   j += strlen(movie);
   i+=2;
   do {
      if (encoderString[i] == '$') break;
      encoderCommand[j] = encoderString[i];
      i++;
      j++;
   } while (encoderString[i] != '\0');
   encoderCommand[j] = '\0';
   strcat(encoderCommand, soundPath);
   strcat(encoderCommand, "/");
   strcat(encoderCommand, sound);
   j += strlen(soundPath);
   j += strlen("/");
   j += strlen(sound);
   i+=2;
   do {
      if (encoderString[i] == '$') break;
      encoderCommand[j] = encoderString[i];
      i++;
      j++;
   } while (encoderString[i] != '\0');
   encoderCommand[j] = '\0';
   strcat(encoderCommand, video);
   j += strlen(video);
   encoderCommand[j] = '\0';
   system(encoderCommand);
}


/***********************************************************/
/* MAIN.                                                   */
/***********************************************************/
/****************************************/
/* Main.                                */
/****************************************/
int main(int argc, char **argv) {
int  eStart, eStop;
char title[80];
char vencoderString[256], mencoderString[256];
   // Make title.
   system("clear");
   strcpy(title, "movie-R.A.M. (V. ");
   strcat(title, version);
   strcat(title, ") by R. Marsh.");
   printf("%s\n", title);
   printf("\n");
   // Help menu.
   if (argc != 2) {
      printf("                         MOVIERAM HELP                          \n");
      printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
      printf("+ Step 1: Copy movieRAM into the directory where the data files+\n");
      printf("+         are located.                                         +\n");
      printf("+ Step 2: Copy movieRAM.INI into the directory where the data  +\n");
      printf("+         files are located.                                   +\n");
      printf("+ Step 3: Modify the movieRAM.INI file as required by your     +\n");
      printf("+         installation.                                        +\n");
      printf("+ Step 4: CD to the directory where the data files are located.+\n");
      printf("+ Step 5: Run movieRAM by entering:                            +\n");
      printf("+         ./movieRAM param                                     +\n");
      printf("+     Where param is:                                          +\n");
      printf("+         encode - Encodes .png files into .avi file.          +\n");
      printf("+         sound  - Merges sound and video making movie.        +\n");
      printf("+         both   - Encode and merges sound making movie.       +\n");
      printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
      exit(0);
   }
   READ_INI_FILE(vencoderString, mencoderString);
   eStart = time(NULL);
   if (!strcmp(argv[1], "encode") || !strcmp(argv[1], "both")) {
      if (!strcmp(includeTitle, "yes")) MAKE_EXTRA_FRAMES();
      ENCODE_VIDEO(vencoderString);
   }
   if (!strcmp(argv[1], "sound") || !strcmp(argv[1], "both")) {
      ENCODE_MOVIE(mencoderString);
   }
   eStop = time(NULL);
   printf("Encode time: %d.\n", (eStop - eStart));

   return 1;
}
