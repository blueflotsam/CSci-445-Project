#include "gridRAM.h"

/***********************************************************/
/* SETUP ROUTINES                                          */
/***********************************************************/
/****************************************/
/* READ_INI_FILE                        */
/* Reads gridRAM.ini file.              */
/****************************************/
void READ_INI_FILE(char iniString[256]) {
int  i, j;
char header[80];
char junk;
FILE *stream;
   printf("*************************************************\n");
   printf("* MESSAGE - Reading gridRAM.ini file.           *\n");
   printf("*************************************************\n");
   if ((stream = fopen("./gridRAM.ini", "r")) == NULL) {
      printf("ERROR - No gridRAM.ini file.\n");
      exit(0);
   } else {
      fscanf(stream, "%s %s", header, inputExtension);
      fscanf(stream, "%s %s", header, outputExtension);
      fscanf(stream, "%s %d", header, &CorePerNode);
      fscanf(stream, "%s %s", header, dataDir);
      fscanf(stream, "%s", header);
      fscanf(stream, "%s", header);
      fscanf(stream, "%s", header);
      fscanf(stream, "%c", &junk);
      i = 0;
      do {
         fscanf(stream, "%c", &iniString[i]);
         i++;
      } while (!feof(stream));
      iniString[--i] = '\0';
   }
   fclose(stream);
}

/****************************************/
/* MAKE_POV_LIST                        */
/* Creates a list of .pov files.        */
/****************************************/
void MAKE_POV_LIST(void) {
char   sysCommand[256];
   printf("*************************************************\n");
   printf("* MESSAGE - Generating list of *.pov files.     *\n");
   printf("*************************************************\n");
   strcpy(sysCommand, "ls *");
   strcat(sysCommand, inputExtension);
   strcat(sysCommand, " > povFileList");
   system(sysCommand);
}

/****************************************/
/* CREATE_COMMAND                       */
/* Creates processing command.          */
/****************************************/
void CREATE_COMMAND(FILE *disk, char iniString[256], char povCommand[256], int sent) {
int  i, j;
char fileName[80];
   i = 0;
   j = 0;
   do {
      if (iniString[i] == '$') break;
      povCommand[j] = iniString[i];
      i++;
      j++;
   } while (iniString[i] != '\0');
   povCommand[j] = '\0';
   // Get next data file from list.
   fscanf(disk, "%s", fileName); 	
   if (feof(disk)) {
      strcpy(povCommand, "NA");
      if (sent == 0) {
         printf("ERROR - No data files to process.\n");
         exit(0);
      }
      return;
   }
   strcat(povCommand, dataDir);
   strcat(povCommand, "/");
   strcat(povCommand, fileName);
   j += strlen(dataDir);
   j += strlen("/");
   j += strlen(fileName);
   i+=2;
   do {
      if (iniString[i] == '$') break;
      povCommand[j] = iniString[i];
      i++;
      j++;
   } while (iniString[i] != '\0');
   povCommand[j] = '\0';
   // Create output file.
   strcat(fileName, outputExtension);
   strcat(povCommand, fileName);
}

/***********************************************************/
/* TEST ROUTINES                                           */
/***********************************************************/
/****************************************/
/* MAKE_PNG_LIST                        */
/* Creates a list of .png files.        */
/****************************************/
void MAKE_PNG_LIST(void) {
char   sysCommand[256];
   printf("*************************************************\n");
   printf("* MESSAGE - Generating list of *.png files.     *\n");
   printf("*************************************************\n");
   strcpy(sysCommand, "ls *");
   strcat(sysCommand, outputExtension);
   strcat(sysCommand, " > pngFileList");
   system(sysCommand);
}

/****************************************/
/* COMPARE_POV_PNG_LISTS                */
/* Creates a list of .png files.        */
/****************************************/
void COMPARE_POV_PNG_LISTS(void) {
int  povCnt, pngCnt;
char junk[80];
FILE *disk;
   printf("*************************************************\n");
   printf("* MESSAGE - Comparing *.pov and *.png lists.    *\n");
   printf("*************************************************\n");
   // Process povFileList;
   if ((disk = fopen("./povFileList", "r")) == NULL) {
      printf("ERROR - No data file [povFileList] list.\n");
      exit(0);
   } 
   povCnt = 0;
   while (1) {
      fscanf(disk, "%s", junk);
      if (feof(disk)) break;
      povCnt++;
   }

   // Process pngFileList;
   if ((disk = fopen("./pngFileList", "r")) == NULL) {
      printf("ERROR - No data file [pngFileList] list.\n");
      exit(0);
   } 
   pngCnt = 0;
   while (1) {
      fscanf(disk, "%s", junk);
      if (feof(disk)) break;
      pngCnt++;
   }
   // Test results.
   if (povCnt == pngCnt) {
      printf("SUCCESS - All pov files were processed.\n");
   } else {
      printf("WARNING - Not all pov files were processed.\n");
   }
}

/***********************************************************/
/* SINGLE MACHINE ROUTINES                                 */
/***********************************************************/
/****************************************/
/* LOAD_BALANCE                         */
/* Load balance over single CPU.        */
/****************************************/
void LOAD_BALANCE(char iniString[256]) {
int    processed;
char   sysCommand[256];
FILE   *disk;
   printf("*************************************************\n");
   printf("* MESSAGE - Processing data.                    *\n");
   printf("*************************************************\n");
   // Open povFileList file.
   if ((disk = fopen("./povFileList", "r")) == NULL) {
      printf("ERROR - No data file [povFileList] list.\n");
      exit(0);
   } 
   // Process data.
   processed = 0;
   while (1) {
      CREATE_COMMAND(disk, iniString, sysCommand, processed);
      if (!strcmp(sysCommand, "NA")) break;
      strcat(sysCommand, " 2> /dev/null");
      system(sysCommand);
      printf("Package %4d processed.\n", processed);
      processed++;
   }
   fclose(disk);
}

/***********************************************************/
/* MULTI-CORE MACHINE ROUTINES                             */
/***********************************************************/
/****************************************/
/* PROCESS                              */
/* Calls command via system().          */
/****************************************/
void *PROCESS(void *arg) {
int    exitCode = 1;
struct thread_data *data;
   data = ((thread_data*)arg);
   system(data->sysCommand);
   data->status = 1;
   pthread_exit(&exitCode);
}

/****************************************/
/* THREAD_LOAD_BALANCE                  */
/* Load balance over multi-core CPU.    */
/****************************************/
void THREAD_LOAD_BALANCE(char iniString[256]) {
int    t, frame, retn, send;
int    dataSize = 100000000;
int    threads = CorePerNode;
struct thread_data data[threads];
pthread_t thread[threads];
char   sysCommand[256];
FILE   *disk;
   printf("*************************************************\n");
   printf("* MESSAGE - Processing data.                    *\n");
   printf("*************************************************\n");
   // Open povFileList file.
   if (!(disk=fopen("./povFileList", "r"))) {
      printf("ERROR - No data file [povFileList] list.\n");
      exit(0);
   } 
   // Send each command to a thread.
   frame = 0;
   for (t = 0; t < threads; t++) {
       CREATE_COMMAND(disk, iniString, sysCommand, frame);
       if (!strcmp(sysCommand, "NA")) { 
          dataSize = frame;
          printf("End of frames reached at: %d.\n", dataSize);
          break;
       }
       strcat(sysCommand, " 2> /dev/null");
       strcpy(data[t].sysCommand, sysCommand);
       data[t].frame  = frame;
       data[t].status = 0;
       pthread_create(&thread[t], NULL, PROCESS, (void *)&data[t]); 
       pthread_detach(thread[t]);
       frame++;
   }
   // Wait for any thread to complete and resend new data.
   t    = 0;
   retn = 0;
   send = 1;
   while (1) {
      if (data[t].status == 1) {
         data[t].status = 0;
         printf("Frame %4d processed.\n", data[t].frame);
         retn++;
         // Keep sending data?
         if (send == 1) {
            CREATE_COMMAND(disk, iniString, sysCommand, frame);
            if (!strcmp(sysCommand, "NA")) { 
               dataSize = frame;
               printf("End of frames reached at: %d.\n", dataSize);
            }
            strcat(sysCommand, " 2> /dev/null");
            strcpy(data[t].sysCommand, sysCommand);
            data[t].frame = frame;
            data[t].status = 0;
            pthread_create(&thread[t], NULL, PROCESS, (void *)&data[t]); 
            pthread_detach(thread[t]);
            frame++;
         }
         if (frame >= dataSize) send = 0;
         if (retn  >= dataSize) break;
      }
      t++;
      if (t == threads) t = 0;
   }
   fclose(disk);
}

/***********************************************************/
/* MAIN.                                                   */
/***********************************************************/
/****************************************/
/* Main.                                */
/****************************************/
int main(int argc, char **argv) {
int  nodesPossible = 0;
int  nodesAvailable= 0;
int  CPUsAvailable = 0;
int  CPUS;
int  inputBad;
int  rStart, rStop;
char title[80];
char iniString[256];
   // Make title.
   system("clear");
   strcpy(title, "grid-R.A.M. (V. ");
   strcat(title, version);
   strcat(title, ") by R. Marsh.");
   printf("%s\n", title);
   printf("\n");
   // Help menu.
   if (argc != 2) {
      printf("                          GRIDRAM HELP                          \n");
      printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
      printf("+ Step 1: Copy gridRAM into the directory where the data files +\n");
      printf("+         are located.                                         +\n");
      printf("+ Step 2: Copy gridRAM.INI into the directory where the data   +\n");
      printf("+         files are located.                                   +\n");
      printf("+ Step 3: Modify the gridRAM.INI file as required by your      +\n");
      printf("+         installation.                                        +\n");
      printf("+ Step 4: CD to the directory where the data files are located.+\n");
      printf("+ Step 5: Run gridRAM by entering:                             +\n");
      printf("+         ./gridRAM single - To run on single CPU.             +\n");
      printf("+         ./gridRAM thread - To run on multi-core.             +\n");
      printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
      exit(0);
   }
   // Initialize data.
   READ_INI_FILE(iniString);
   MAKE_POV_LIST();
   // Single CPU mode.
   if (!strcmp(argv[1], "single")) {
      rStart = time(NULL);
      printf("MESSAGE - Number of available cores: 1\n");
      LOAD_BALANCE(iniString);
      rStop  = time(NULL);
      printf("Render time (sec): %d\n", (rStop - rStart));
   }
   // Multi-core CPU mode.
   if (!strcmp(argv[1], "thread")) {
      rStart = time(NULL);
      printf("MESSAGE - Number of available cores: %d\n", CorePerNode);
      THREAD_LOAD_BALANCE(iniString);
      rStop  = time(NULL);
      printf("Render time (sec): %d\n", (rStop - rStart));
   }

   return 1;
}
