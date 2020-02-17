/* Author: Chase Richards
   Project: Homework 2 CS4760
   Date February 13, 2020  
   Filename: Control.c  */

#include "Control.h"

int main(int argc, char* argv[]) 
{
    //flgsPassedIn(argc, argv);
    
    int c;
    int maxChildren = 4, childLimit = 2, startOfSeq = 0, incrementVal = NULL;
    char *outFile;

    /* Why didn't I use optarg on homework 1.... */
    while((c = getopt(argc, argv, "hn:s:b:i:o:")) != -1) 
    {
        switch(c) 
        {
            case 'h':
                displayHelpMessage();
                break;
            case 'n':
                maxChildren = atoi(optarg);
                printf("%d\n", maxChildren);
                if(maxChildren > 20)
                {
                    printf("The max number of children allowed is 20, setting to -n 20");
                    maxChildren = 20;
                }
                break;
            case 's':
                childLimit = atoi(optarg);
                printf("%d\n", childLimit);
                break;
            case 'b':
                startOfSeq = atoi(optarg);
                printf("%d\n", startOfSeq);
                break;
            case 'i':
                incrementVal = atoi(optarg);
                printf("%d\n", incrementVal);
                break;
            case 'o':
                outFile = optarg;
                printf("%s\n", outFile);
                break;
            case '?':
                fprintf(stderr, "%s: Error: Invalid option, use -h to see the available options.\n", argv[0]);
                perror("");
                exit(1); 
        }
    }   
 
    signal(SIGALRM, sigHandler);
    alarm(2);

    signal(SIGINT, sigHandler);   
 
    sharedMemoryWork(maxChildren); 
   
    return 0;   
}

void sharedMemoryWork(int maxChildren) 
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
     

    sharedMemAttachInt[0] = maxChildren;

    while (childCounter < maxChildren)
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

            /*  Testing for Signals
               for(;;)
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

void displayHelpMessage() 
{
    printf("\n---------------------------------------------------------\n");
    printf("See below for the options:\n\n");
    printf("-h    : Instructions for running the project.\n");
    printf("-n x  : Maximum number of child processes oss will ever create (Default 4).\n"); 
    printf("-s x  : Number of children allowed to exist in the system at same time (Default 2).\n");
    printf("-b B  : Start of the sequence of numbers to be tested for primality.\n");
    printf("-i I  : Increment between numbers that we test.\n");
    printf("-o filename  : Output file.\n");
    printf("\n---------------------------------------------------------\n"); 
    exit(0);
}
