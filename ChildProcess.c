/* Author: Chase Richards
   Project: Homework 2 CS4760
   Date: February 16, 2020  
   Filename: ChildProcess.c  */

#include "ChildProcess.h"

int main(int argc, char* argv[]) 
{

    int sharedMemSegment, sharedMemDetach;
    char *sharedMemAttach; 
    //ftok gives us the key based on curent path and 'm' id
    key_t key = ftok(".",'m');

    //Allocate the shared memory with shmget
    sharedMemSegment = shmget(key, sizeof(struct sharedMemory), IPC_CREAT | 0644);

    //If shmget returns -1 then it was unsuccessful
    if(sharedMemSegment == -1)
    {
        perror("oss: Error: shmget failed to allocate shared memory\n");
        exit(EXIT_FAILURE);
    }
    
    //Attach the memory to our space
    smPtr = (struct sharedMemory *)shmat(sharedMemSegment, NULL, 0);
    //int *childMemAttachInt = (int *)sharedMemAttach;

    //If shmat returns -1 then it was unsuccessful
    if(smPtr == (void *)-1)
    {
        perror("exe: Error: shmat failed to attach shared memory");
        if(shmctl(sharedMemSegment, IPC_RMID, NULL) == -1)
            perror("exe: Error: shmctl failed to remove the shared memory segment");
        exit(EXIT_FAILURE);
    }

     
    //printf("Start of Sequence: %d\n", smPtr-> nanoSeconds);
    //printf("Increment Value: %d\n", smPtr-> i);
    //printf("ChildID: %d\n", atoi(argv[1]));
    //printf("Prime Number to Check: %d\n", atoi(argv[2]));
    
    int i; 
    //childID = atoi(argv[1]);
    //numToCheck = atoi(argv[2]);
    //printf("%d", childID);
    //smPtr-> childProcArr = malloc(sizeof(int) * 20);
    //Check from 2 to the number we are checking divided by 2 

    printf("%d\n", atoi(argv[2]));

    smPtr-> childProcArr[2] = atoi(argv[2]);

    for(i = 2; i <= sqrt(atoi(argv[2])); i++)
    {
        //If n is divided by any to the number from 2 to numToCheck/2 it isn't prime
        if((atoi(argv[2])) % i == 0)
            smPtr-> childProcArr[atoi(argv[1])] = ((atoi(argv[2])) * -1);
        else
            smPtr-> childProcArr[atoi(argv[1])] = atoi(argv[2]);
            
    } 
  
    //Detach and remove the shared memeory segment
    sharedMemDetach = deallocateMem(sharedMemSegment, smPtr);

    //If shmdt returns -1 then it was unsuccessful
    if(sharedMemDetach == -1)
    {
        perror("oss: Error: shmdt failed to detach shared memory\n");
        exit(EXIT_FAILURE);
    }

    return 0;

}

int deallocateMem(int shmid, void *shmaddr) 
{
    //If detaching fails it will return -1 so return -1 for deallocation call
    if(shmdt(shmaddr) == -1)
        return -1;
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
