#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <pthread.h>
#include <semaphore.h>

//Constants

#define INTERVALS 10

//GLOBAL VARIABLES

//Mutex
pthread_mutex_t lock;

//COMMANDS

   //01 COMMANDS [ Get tasks ] : whoami | xargs top -b -n 1 -u | awk '{if(NR>7)printf "%-s %6s %-4s %-4s %-4s\n",$NF,$1,$9,$10,$2}' | sort -k X
	//02 COMMANDS [ Get CPU% Usage ] : top -bn1 | grep "Cpu(s)" | sed "s/.*, *\([0-9.]*\)%* id.*/\1/" | awk '{printf("%0.1f",$1);}'	
	//03 COMMANDS [ Get Mem% Usage ] : free -m | grep Mem | awk '{printf("%0.1f",$3/$2*100)}'

//Get Tasks
char execTasks[] = "whoami | xargs top -b -n 1 -u | awk '{if(NR>7)printf \"%-s %6s %-4s %-4s %-4s\n\",$NF,$1,$9,$10,$2}' | sort -k 1";

//Get CPU % Usage
char execCpu[] = "top -bn1 | grep \"Cpu(s)\" | sed \"s/.*, *\\([0-9.]*\\)%* id.*/\\1/\" | awk \'{printf(\"%0.1f\",100-$1);}\'";

// Get Mem% Usage
char execMem[] = "free -m | grep Mem | awk \'{printf(\"%0.1f\",$3/$2*100)}\'";



//FUNCTIONS

//Function to print a 2D graph from a given array of values
void printGraph(int arr[INTERVALS]){

   int len = 0;

   for(int i = 10; i >= 0; i--)
   {
      if(i >= 10){
         printf("%d %% : ", (i * 10));
      }
      else {
         printf("%d %%  : ", (i * 10));
      }
      

      for(int j = 0; j < INTERVALS; j++)
      {
         if(i == arr[j]){
            printf("*");
         }

         printf("   ");
      }
      printf("\n");
   }
}

//Function to add a new value to the left into the array and shift everything right
void shiftArrayRight(int arr[INTERVALS], int newVal) {

   for(int i = (INTERVALS - 1); i > 0; i--) {
      arr[i] = arr[i - 1];
   }

   arr[0] = newVal;

}

//Function to add a new value to the right into the array and shift everything left
void shiftArrayLeft(int arr[INTERVALS], int newVal) {

   int i = 0;

   for(i = 0; i < (INTERVALS - 1); i++) {
      arr[i] = arr[i + 1];
   }

   arr[INTERVALS - 1] = newVal;

}

//Function to print an array of 10 int values
void printArray(int arr[INTERVALS]){

   printf("\n");
   for(int i = 0; i < INTERVALS; i++) {
      printf(" %d ", arr[i]);
   }
   printf("\n");

}

void *getMem(void *arg) {

   pthread_exit(0);
}

void *getCpu(void *arg) {

   pthread_exit(0);
}

void *getIO(void *arg) {

   pthread_exit(0);
}

void *getBandwidth(void *arg) {

   pthread_exit(0);
}


//MAIN

int main(int argc, char *argv[]) {

   //Declaring variables
   int arg = 0;

   //Initializing arrays
   int memVals[INTERVALS] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
   int cpuVals[INTERVALS] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

   //int vals[INTERVALS] = {5, 7, 2, 4, 6, 7, 2, 3, 4, 5};
   //int vals[INTERVALS] = {3, 4, 5, 4, 3, 7, 2, 3, 4, 5};
   //int vals[INTERVALS] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
   int vals[INTERVALS] = {-1, -1, -1, -1, -1, -1, -1, 2, 3, 4};

   //Defining Threads
   pthread_t memThread;
   pthread_t cpuThread;
   pthread_t IOThread;
   pthread_t bandwidthThread;


   //CREATING THREADS

   //Creating memThread
   pthread_create( &memThread, NULL,  getMem , &arg);

   //Creating cpuThread
   pthread_create( &cpuThread, NULL,  getCpu , &arg);

   //Creating IOThread
   pthread_create( &IOThread, NULL,  getIO , &arg);

   //Creating bandwidthThread
   pthread_create( &bandwidthThread, NULL,  getBandwidth , &arg);


   //In between Code goes here

   /*        CODE START           */

   printf("\t **** WELCOME TO TASK MANAGER **** \n");
   printf("\n");

   //Printing array vals
   printf("Printing the values in the array \n \n");
   printArray(vals);

   //Graph from array vals
   printf("\n Printing Graph from values.. \n \n");
   printGraph(vals);

   //Adding new val to right
   printf("Adding a new value '2' to right shifting the array to left \n \n");
   shiftArrayLeft(vals, 2);
   printArray(vals);

   //Graph from array vals
   printf("\n Printing the Graph from new values.. \n \n");
   printGraph(vals);

   //Adding new val to left
   printf("Adding a new value '5' to left shifting the array to right \n \n");
   shiftArrayRight(vals, 5);
   printArray(vals);

   //Graph from array vals
   printf("\n Printing the Graph from new values.. \n \n");
   printGraph(vals);

   //Printing Commands

   //ExecTasks
   printf("\n Printing the execTasks command \n \n");
   system(execTasks);
   printf("\n \n");

   //ExecCpu
   printf("\n Printing the execCpu command \n \n");
   system(execCpu);
   printf("\n \n");

   //ExexMem
   printf("\n Printing the execMem command \n \n");
   system(execMem);
   printf("\n \n");


   /*        CODE END           */


   //JOINING THREADS

   //Joinig memThread
   pthread_join(memThread, NULL);

   //Joinig cpuThread
   pthread_join(cpuThread, NULL);

   //Joinig IOThread
   pthread_join(IOThread, NULL);

   //Joinig bandwidthThread
   pthread_join(bandwidthThread, NULL);


    return 0;

}
