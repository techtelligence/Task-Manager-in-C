#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <pthread.h>
#include <semaphore.h>
#include <math.h>

//Constants

#define INTERVALS 10

//GLOBAL VARIABLES

//Initializing arrays

   //Arrays to store logs
   double memVals[INTERVALS] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
   double cpuVals[INTERVALS] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
   double IOVals[INTERVALS] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
   double bandVals[INTERVALS] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

   //Arrays to store graph intervals
   double graphInterMem[11] = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};

//Mutex
pthread_mutex_t lock;

//COMMANDS

   //01 COMMANDS [ Get tasks ] : whoami | xargs top -b -n 1 -u | awk '{if(NR>7)printf "%-s %6s %-4s %-4s %-4s\n",$NF,$1,$9,$10,$2}' | sort -k 1
	//02 COMMANDS [ Get CPU% Usage ] : top -bn1 | grep "Cpu(s)" | sed "s/.*, *\([0-9.]*\)%* id.*/\1/" | awk '{printf("%0.1f",$1);}'	
	//03 COMMANDS [ Get Mem% Usage ] : free -m | grep Mem | awk '{printf("%0.1f",$3/$2*100)}'

//Get Tasks
char execTasks[] = "whoami | xargs top -b -n 1 -u | awk '{if(NR>7)printf \"%-s %6s %-4s %-4s %-4s\\n\",$NF,$1,$9,$10,$2}' | sort -k 1";

//char execTasks[] = "whoami | xargs top -b -n 1 -u | awk '{if(NR>7)printf \"%%-s %6s %%-4s %%-4s %%-4s\n\",$NF,$1,$9,$10,$2}' | sort -k 1";

//Get CPU % Usage
char execCpu[] = "top -bn1 | grep \"Cpu(s)\" | sed \"s/.*, *\\([0-9.]*\\)%* id.*/\\1/\" | awk \'{printf(\"%0.1f\",100-$1);}\'";

// Get Mem% Usage
char execMem[] = "free -m | grep Mem | awk \'{printf(\"%0.1f\",$3/$2*100)}\'";



//FUNCTIONS

//Function to round float
double roundFloat(double var) 
{ 
    // 37.66666 * 100 =3766.66 
    // 3766.66 + .5 =3767.16    for rounding off value 
    // then type cast to int so value is 3767 
    // then divided by 100 so the value converted into 37.67 
    double value = (int)(var * 100 + .5); 
    return (double)value / 100; 
} 

//Function to print a 2D graph from a given array of values
void printGraph(double arr[INTERVALS]){

   double temp = 0;

   for(int i = 10; i >= 0; i--)
   {
      printf("%f %% : ", graphInterMem[i]);
      
      for(int j = 0; j < INTERVALS; j++)
      {
         if(graphInterMem[i] == arr[j]){
            printf("*");
         }

         printf("   ");
      }
      printf("\n");
   }
}

  

//Function to add a new value to the left into the array and shift everything right
void shiftArrayRight(double arr[INTERVALS], double newVal) {

   for(int i = (INTERVALS - 1); i > 0; i--) {
      arr[i] = arr[i - 1];
   }

   arr[0] = newVal;

}

//Function to add a new value to the right into the array and shift everything left
void shiftArrayLeft(double arr[INTERVALS], double newVal) {

   int i = 0;

   for(i = 0; i < (INTERVALS - 1); i++) {
      arr[i] = arr[i + 1];
   }

   arr[INTERVALS - 1] = newVal;

}

//Function to print an array of 10 int values
void printArray(double arr[INTERVALS]){

   printf("\n");
   for(int i = 0; i < INTERVALS; i++) {
      printf(" %f ", arr[i]);
   }
   printf("\n");

}

//Function to print an array of a given size
void printArrayGen(double *arr, int size){

   printf("\n");
   for(int i = 0; i < size; i++) {
      printf(" %f ", arr[i]);
   }
   printf("\n");

}

//Function to return output of a command in float
double commandToFloat (char* passedCommand) {

    FILE *fpipe;
    char *command = passedCommand;
    char value[10];

    char c = 0;

    if (0 == (fpipe = (FILE*)popen(command, "r")))
    {
        perror("popen() failed.");
        exit(EXIT_FAILURE);
    }

   int k = 0;
    while (fread(&c, sizeof c, 1, fpipe))
    {
        value[k] = c;
        k++;
    }
    value[k] = '\0';

    pclose(fpipe);

   double fVal;
   fVal = strtof(value, NULL);

   return fVal;
}

//Function to return output of a command in int
int commandToInt (char* passedCommand) {

    FILE *fpipe;
    char *command = passedCommand;
    char value[10];

    char c = 0;

    if (0 == (fpipe = (FILE*)popen(command, "r")))
    {
        perror("popen() failed.");
        exit(EXIT_FAILURE);
    }

   int k = 0;
    while (fread(&c, sizeof c, 1, fpipe))
    {
        value[k] = c;
        k++;
    }
    value[k] = '\0';

    pclose(fpipe);
    
   int iVal;
   iVal = atoi(value);

   return iVal;
}

//Function to return output of a command in String - NOT WORKING
//BUGS TO FIX
char* commandToString (char* passedCommand) {

    FILE *fpipe;
    char *command = passedCommand;
    char *value;

    char c = 0;

    if (0 == (fpipe = (FILE*)popen(command, "r")))
    {
        perror("popen() failed.");
        exit(EXIT_FAILURE);
    }

   int k = 0;
    while (fread(&c, sizeof c, 1, fpipe))
    {
        value[k] = c;
        k++;
    }
    value[k] = '\0';

    pclose(fpipe);

   return value;
}

//Function to print commands output to console
void commandPrint(char* passedCommand) {

    FILE *fpipe;
    char *command = passedCommand;
    char *value;

    char c = 0;

    if (0 == (fpipe = (FILE*)popen(command, "r")))
    {
        perror("popen() failed.");
        exit(EXIT_FAILURE);
    }

    while (fread(&c, sizeof c, 1, fpipe))
    {
        printf("%c", c);
    }
    

    pclose(fpipe);

}

//Function to print output of a command to console
void printCommandResult(char* passedCommand) {

   printf("\n \n");
   system(passedCommand);
   printf("\n \n");

}

//Thread function to update graph intervals
void *updateGraphInter(void *arg) {
   int mem;
   //int memPrev = 0;
   int i;

   while(1) {
      mem = commandToInt(execMem);
      for(i = 0; i < 11; i++){
            graphInterMem[i] = (mem + (0.1 * i));
            graphInterMem[i] = roundFloat(graphInterMem[i]);
      }
   }
}

// Thread function to get current memory usage and update memVals array
void *getMem(void *arg) {

   double mem;

   while(1) {
      mem = commandToFloat(execMem);
      mem = roundFloat(mem);

      shiftArrayLeft(memVals, mem);

      //printf("\n Memory Log: ");
      //printArray(memVals);

      sleep(1); //Waiting for 1 second
   }

   pthread_exit(0);
}

// Thread function to get current cpu usage and update cpuVals array
void *getCpu(void *arg) {

   double cpu;

   while(1) {
      cpu = commandToFloat(execCpu);
      cpu = roundFloat(cpu);

      shiftArrayLeft(cpuVals, cpu);

      //printf("\n CPU Log: ");
      //printArray(cpuVals);

      sleep(1); //Waiting for 1 second
   }

   pthread_exit(0);
}

// Thread function to get current IO usage and update IOVals array
void *getIO(void *arg) {

   pthread_exit(0);
}

// Thread function to get current Bandwidth usage and update bandVals array
void *getBandwidth(void *arg) {

   pthread_exit(0);
}

//Thread Function to render the screen
void *renderScreen(void *arg) {

   while(1) {
      system("clear");

      printf("Current Memory Usage: %f %%", memVals[INTERVALS - 1]);
      printf("\t Current CPU Usage: %f %% \n", cpuVals[INTERVALS - 1]);
      printf("\n Memory Log: ");

      printArray(memVals);
      printGraph(memVals);

      //system(execTasks);


      //printArray(cpuVals);
      //printGraph(cpuVals);

      sleep(1);
   }

   pthread_exit(0);
}

//MAIN

int main(int argc, char *argv[]) {

   //Declaring variables
   int arg = 0;

   //Defining Threads
   pthread_t memThread;
   pthread_t cpuThread;
   pthread_t IOThread;
   pthread_t bandwidthThread;
   pthread_t renderThread;
   pthread_t updateGraphIntervThread;


   //CREATING THREADS

   //Creating memThread
   pthread_create( &memThread, NULL,  getMem , &arg);

   //Creating cpuThread
   pthread_create( &cpuThread, NULL,  getCpu , &arg);

   //Creating IOThread
   //pthread_create( &IOThread, NULL,  getIO , &arg);

   //Creating bandwidthThread
   //pthread_create( &bandwidthThread, NULL,  getBandwidth , &arg);

   //Creating updateGraphInterval thread
   pthread_create( &updateGraphIntervThread, NULL,  updateGraphInter , &arg);

   //Creating renderThread
   pthread_create( &renderThread, NULL,  renderScreen , &arg);


   //In between Code goes here

   /*        CODE START           */

   

   /*        CODE END           */


   //JOINING THREADS

   //Joinig memThread
   pthread_join(memThread, NULL);

   //Joinig cpuThread
   pthread_join(cpuThread, NULL);

   //Joinig IOThread
   //pthread_join(IOThread, NULL);

   //Joinig bandwidthThread
   //pthread_join(bandwidthThread, NULL);

   //Joinig updateGraphIntervThread
   pthread_join(updateGraphIntervThread, NULL);

   //Joinig renderThread
   pthread_join(renderThread, NULL);

   printf("\n \n");

    return 0;

}
