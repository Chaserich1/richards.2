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
    char *outFile = "output.dat";

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
                //printf("%d\n", maxChildren);
                if(maxChildren > 20)
                {
                    printf("The max number of children allowed is 20, setting to -n 20");
                    maxChildren = 20;
                }
                break;
            case 's':
                childLimit = atoi(optarg);
                //printf("%d\n", childLimit);
                break;
            case 'b':
                startOfSeq = atoi(optarg);
                //printf("%d\n", startOfSeq);
                break;
            case 'i':
                incrementVal = atoi(optarg);
                //printf("%d\n", incrementVal);
                break;
            case 'o':
                outFile = optarg;
                //printf("%s\n", outFile);
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
 
    sharedMemoryWork(maxChildren, childLimit, startOfSeq, incrementVal, outFile); 
   
    return 0;   
}

void sharedMemoryWork(int maxChildren, int childLimit, int startOfSeq, int incrementVal, char* outFile) 
{

    int sharedMemSegment, sharedMemDetach;
    char *sharedMemAttach;
    key_t key;

    //Key returns a key based on the path and id
    key = ftok(".",'m');
    //printf("%d", key);

    //Allocate the shared memory using shmget
    sharedMemSegment = shmget(key, sizeof(struct sharedMemory), IPC_CREAT | 0644);

    //If shmget is unsuccessful it retruns -1 so check for this
    if(sharedMemSegment == -1) 
    {
        perror("exe: Error: shmget failed to allocate shared memory");
        exit(EXIT_FAILURE);
    }

    //Attach the memory to our space
    smPtr = (struct sharedMemory *)shmat(sharedMemSegment, NULL, 0);
    //int *sharedMemAttachInt = (int *)sharedMemAttach;    

    //If shmat is unsuccessful it returns -1 so check for this
    if(smPtr == (void *)-1)
    {
        perror("oss: Error: shmat failed to attach shared memory");
        //if(shmctl(sharedMemSegment, IPC_RMID, NULL) == -1)
          //  perror("exe: Error: shmctl failed to remove the shared memory segment");
        exit(EXIT_FAILURE);
    }

    /* FOR INTIAL TESTING: Use the shared memory or read it 
    if(argc >= 2) 
        strncpy(sharedMemAttach, argv[1], sizeof(int));
    printf("The segment has the following: %s\n", sharedMemAttach); */
     
    smPtr-> nanoSeconds = 0;
    smPtr-> seconds = 0;     
    
    smPtr-> childProcArr = malloc(sizeof(int) * maxChildren);
    smPtr-> childProcArr[maxChildren] = 0;   

    launchChildren(maxChildren, childLimit, outFile);

    //Detach and remove the segment of shared memory
    sharedMemDetach = deallocateMem(sharedMemSegment, (void *) smPtr);
    //If shmdt is unsuccessful it returns -1 so check for this
    if(sharedMemDetach == -1)
    {
        perror("exe: Error: shmdt failed to detach shared memory");
        exit(EXIT_FAILURE);
    }
}

void launchChildren(int maxChildren, int childLimit, char *outFile)
{
    pid_t pid;
    pid_t waitingID;
    int incrementClock; 
    int childrenInSystem = 0;
    int completedChildren = 0;
    int childCounter = 0;
    int childExec;
    int initChildID = 0;
    int initPrimeNum = 7; 
    char primeNum[100];
    char childID[20];
    snprintf(primeNum, sizeof(primeNum), "%d", initPrimeNum);

    OUTFILE = fopen(outFile, "a");

    //start timer

    while(maxChildren > completedChildren)
    {   
        timeIncrementation();
        //initChildID++;     
        //char childID[50];
        //snprintf(childID, sizeof(childID), "%d", initChildID);
   
  
        if(childCounter < maxChildren && childrenInSystem < childLimit)
        {
            childrenInSystem++;
            initChildID++;
            snprintf(childID, 20, "%d", initChildID);
            pid = fork();
            //Fork returns -1 if it fails
            if(pid == -1)
            {
                perror("oss: Error: Child process fork failed");
                exit(EXIT_FAILURE);
            }
            //Fork returns 0 to the child if successful
            else if(pid == 0) /* Child pid */
            { 
                //Use execl to run the oss executable
                childExec = execlp("./prime", "prime", childID, primeNum, NULL);
                
                if(childExec == -1)
                {
                    perror("oss: Error: Child failed to exec ls\n");
                    exit(EXIT_FAILURE);
                }
                exit(EXIT_SUCCESS);                       
            }
            childCounter++;       
        }
     
        //Wait for the child to finish
        waitingID = waitpid(-1, NULL, WNOHANG);      
 
        if(waitingID > 0)
        {   
            fprintf(OUTFILE, "Child ID: %d | Time: %d seconds, %u nanoseconds\n", childID, smPtr-> seconds, smPtr-> nanoSeconds); 
            completedChildren++;
            childrenInSystem--;
        }
    }
}

int deallocateMem(int shmid, void *shmaddr) 
{
    //If detaching fails it will return -1 so return -1 for deallocation call
    if(shmdt(shmaddr) == -1)
        return -1;
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}

void timeIncrementation()
{
    smPtr-> nanoSeconds = smPtr-> nanoSeconds + 10000;
    if(smPtr-> nanoSeconds % 1000000000 == 0)
    {
        smPtr-> seconds = smPtr-> seconds + 1;
    }
}

void sigHandler(int sig)
{
    if(sig == SIGALRM)
    {
        key_t key = ftok(".",'m');
        int sharedMemSegment;
        sharedMemSegment = shmget(key, sizeof(struct sharedMemory), IPC_CREAT | 0644);
        smPtr = (struct sharedMemory *)shmat(sharedMemSegment, NULL, 0);
        printf("Two Seconds is up.\n");
        fprintf(OUTFILE, "Killed at %d seconds and %u nanoseconds\n", smPtr-> seconds, smPtr-> nanoSeconds);
        fclose(OUTFILE);
        shmctl(sharedMemSegment, IPC_RMID, NULL);
        kill(0, SIGKILL);
        exit(EXIT_SUCCESS);
    }
    
    if(sig == SIGINT)
    {
        key_t key = ftok(".",'m');
        int sharedMemSegment;
        sharedMemSegment = shmget(key, sizeof(struct sharedMemory), IPC_CREAT | 0644);
        smPtr = (struct sharedMemory *)shmat(sharedMemSegment, NULL, 0);
        printf("Ctrl-c was entered\n");
        fprintf(OUTFILE, "Killed at %d seconds and %u nanoseconds\n", smPtr-> seconds, smPtr-> nanoSeconds);
        fclose(OUTFILE);
        shmctl(sharedMemSegment, IPC_RMID, NULL);
        kill(0, SIGKILL);
        exit(EXIT_SUCCESS);
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
