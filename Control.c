/* Author: Chase Richards
   Project: Homework 2 CS4760
   Date February 13, 2020  
   Filename: Control.c  */

#include "Control.h"

int main(int argc, char* argv[]) 
{
    flgsPassedIn(argc, argv);
    
    signal(SIGALRM, sigHandler);
    alarm(2);

    signal(SIGINT, sigHandler);   
 
    sharedMemoryWork(); 
   
    return 0;   
}

void sharedMemoryWork() 
{

    int sharedMemSegment, sharedMemDetach;
    int childExec;
    char *sharedMemAttach;
    key_t key;
    struct timespec startTimer = {0,0}, stopTimer = {0,0};
    double totalChildTime = 0;
    int childCounter = 0;

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
    int *sharedMemAttachInt = (int *)sharedMemAttach;    

    //If shmat is unsuccessful it returns -1 so check for this
    if(sharedMemAttachInt == (void *)-1)
    {
        perror("exe: Error: shmat failed to attach shared memory");
        if(shmctl(sharedMemSegment, IPC_RMID, NULL) == -1)
            perror("exe: Error: shmctl failed to remove the shared memory segment");
        exit(EXIT_FAILURE);
    }

    /* FOR INTIAL TESTING: Use the shared memory or read it 
    if(argc >= 2) 
        strncpy(sharedMemAttach, argv[1], sizeof(int));
    printf("The segment has the following: %s\n", sharedMemAttach); */
     

    sharedMemAttachInt[0] = 1;

    while (childCounter < 2)
    {
        //Fork and exec one child process and attach it to the shared memory
        pid_t childpid;
        //Start the timer then fork
        clock_gettime(CLOCK_MONOTONIC, &startTimer);
        childpid = fork();
        wait(NULL);
        childCounter++;
        //Fork returns -1 if it fails
        if(childpid == -1)
        {
            perror("oss: Error: Child process fork failed");
            exit(EXIT_FAILURE);
        }

        //Fork returns 0 to the child if successful
        if(childpid == 0) /* Child pid */
        { 
            /* FOR INITIAL TESTING: print the child pid 
            printf("The child pid is %ld\n", (long)getpid()); */
        
            // FOR INITIAL TESTING: Use execl to have the child run ls -l 
            //childExec = execl("/bin/ls", "ls", "-l", NULL); 

            /* for(;;)
                   ;  */

            //Use execl to run the oss executable
            childExec = execl("./prime", "prime", NULL);

            if(childExec == -1)
            {
                perror("oss: Error: Child failed to exec ls\n");
                exit(EXIT_FAILURE);
            }       
        }
        /* Parent pid */
            //printf("The parent pid is %ld\n", (long)getpid());
            
        //Wait for the child to finish
        wait(NULL);
        //Stop the timer
        clock_gettime(CLOCK_MONOTONIC, &stopTimer);
        //Get the total time by subtracting the starting time from the stoping time
        totalChildTime = ((double)stopTimer.tv_sec + 1.0e-9 * stopTimer.tv_nsec) - ((double)startTimer.tv_sec + 1.0e-9*startTimer.tv_nsec);
        printf("Total time for the child process %d: %.5f seconds\n", childpid, totalChildTime);    
    }

    sharedMemDetach = shmdt(sharedMemAttachInt);
    //Detach and remove the segment of shared memory 
    //sharedMemDetach = deallocateMem(sharedMemSegment, sharedMemAttachInt);

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

void sigHandler(int sig)
{
    if(sig == SIGALRM)
    {
        printf("Two Seconds is up.\n");
        exit(0);
    }
    
    if(sig == SIGINT)
    {
        printf("Ctrl-c was entered\n");
        exit(0);
    }
}
