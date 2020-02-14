/* Author: Chase Richards
   Project: Homework 2 CS4760
   Date: February 13, 2020  
   Filename: CommandOptions.c  */

#include "CommandOptions.h"

void flgsPassedIn(int argc, char **argv) {

    // Used getopt to go through the parameter flgs that the user passes  
    int c;
    int maxChildren = 4, childLimit = 2, startOfSeq = 0, incrementVal = NULL;
    char *outFile;

    //Why didn't I use optarg on homework 1....
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
}


