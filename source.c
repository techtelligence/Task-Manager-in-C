#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <pthread.h>
#include <semaphore.h>

//GLOBAL VARIABLES

//Mutex
pthread_mutex_t lock;

//FUNCTIONS

void print2DArray(int arr[10][10]) {

   for(int i = 0; i < 10; i++)
   {
      for(int j = 0; j < 10; j++)
      {
         //Print
      }
      printf("\n");
   }

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

   //Defining Threads
   pthread_t memThread;
   pthread_t cpuThread;
   pthread_t IOThread;
   pthread_t bandwidthThread;

   printf("Welcome to Task Manager !");
   printf("\n");

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
