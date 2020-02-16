/* Author: Chase Richards
   Project: Homework 2 CS4760
   Date February 13, 2020  
   Filename: Control.c  */

#include "Control.h"

int main(int argc, char* argv[]) 
{
    flgsPassedIn(argc, argv);
    
    sharedMemoryWork(); 
   
    return 0;   
}

void sharedMemoryWork() 
{

    int sharedMemSegment, sharedMemDetach;
    char *sharedMemAttach;
    key_t key;

    //Key returns a key based on the path and id
    key = ftok(".",'m');
    //printf("%d", key);

    //Allocate the shared memory using shmget
    sharedMemSegment = shmget(key, sizeof(int), IPC_CREAT | 0644);

    //If shmget is unsuccessful it retruns -1 so check for this
    if(sharedMemSegment == -1) 
    {
        perror("exe: Error: shmget failed to allocate shared memory");
        exit(EXIT_FAILURE);
    }

    //Attach the memory to our space
    sharedMemAttach = shmat(sharedMemSegment, NULL, 0);

    //If shmat is unsuccessful it returns -1 so check for this
    if((int *)sharedMemAttach == (void *)-1)
    {
        perror("exe: Error: shmat failed to attach shared memory");
        if(shmctl(sharedMemSegment, IPC_RMID, NULL) == -1)
            perror("exe: Error: shmctl failed to remove the shared memory segment");
        exit(EXIT_FAILURE);
    }

    /* FOR INTIAL TESTING: Use the shared memory or read it 
    if(argc >= 2) 
        strncpy(sharedMemAttach, argv[1], sizeof(int));
    printf("The segment has the following: %s\n", sharedMemAttach);
    */ 

    //Fork and exec one child process and attach it to the shared memory
    pid_t childpid;
    childpid = fork();

    //Fork returns -1 if it fails
    if(childpid == -1)
    {
        perror("oss: Error: Child process fork failed");
        exit(EXIT_FAILURE);
    }

    //Fork returns 0 to the child if successful
    if(childpid == 0) /* Child pid */
        printf("The child pid is %ld\n", (long)getpid());
    else /* Parent pid */
        printf("The parent pid is %ld\n", (long)getpid());

    //Detach and remove the segment of shared memory 
    sharedMemDetach = deallocateMem(sharedMemSegment, sharedMemAttach);

    //If shmdt is unsuccessful it returns -1 so check for this
    if(sharedMemDetach == -1)
    {
        perror("exe: Error: shmdt failed to detach shared memory");
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
