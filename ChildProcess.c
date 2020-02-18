/* Author: Chase Richards
   Project: Homework 2 CS4760
   Date: February 16, 2020  
   Filename: ChildProcess.c  */

#include "ChildProcess.h"

int main() 
{
    //printf("child");
    childWork();
 
    return 0;

}

void childWork()
{
    int sharedMemSegment, sharedMemDetach;
    char *sharedMemAttach; 
    //ftok gives us the key based on curent path and 'm' id
    key_t key = ftok(".",'m');

    //Allocate the shared memory with shmget
    sharedMemSegment = shmget(key, sizeof(int), IPC_CREAT | 0644);

    //If shmget returns -1 then it was unsuccessful
    if(sharedMemSegment == -1)
    {
        perror("oss: Error: shmget failed to allocate shared memory\n");
        exit(EXIT_FAILURE);
    }
    
    //Attach the memory to our space
    sharedMemAttach = shmat(sharedMemSegment, NULL, 0);
    int *childMemAttachInt = (int *)sharedMemAttach;

    //If shmat returns -1 then it was unsuccessful
    if(childMemAttachInt == (void *)-1)
    {
        perror("exe: Error: shmat failed to attach shared memory");
        if(shmctl(sharedMemSegment, IPC_RMID, NULL) == -1)
            perror("exe: Error: shmctl failed to remove the shared memory segment");
        exit(EXIT_FAILURE);
    }

    int readValue = childMemAttachInt[0];
    int readnValue = childMemAttachInt[1];
    int readnnValue = childMemAttachInt[2];
    int readnnnValue = childMemAttachInt[3];
    

    //printf("%d %d %d %d %d\n", readValue, readnValue, readnnValue, readnnnValue, readnnnnValue);
    
    //Detach and remove the shared memeory segment
    sharedMemDetach = deallocateMem(sharedMemSegment, childMemAttachInt);

    //If shmdt returns -1 then it was unsuccessful
    if(sharedMemDetach == -1)
    {
        perror("oss: Error: shmdt failed to detach shared memory\n");
        exit(EXIT_FAILURE);
    }

}

int deallocateMem(int shmid, void *shmaddr) 
{
    //If detaching fails it will return -1 so return -1 for deallocation call
    if(shmdt(shmaddr) == -1)
        return -1;
    if(shmctl(shmid, IPC_RMID, NULL) == -1) 
        return -1;
    return 0;
}
