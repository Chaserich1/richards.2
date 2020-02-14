/* Author: Chase Richards
   Project: Homework 2 CS4760
   Date: February 13, 2020  
   Filename: CommandOptions.c  */

#include "CommandOptions.h"

//Initialize all flags as false until they are entered
int helpMessageFlg = 0;    // -h
int maxChildrenFlg = 0;    // -n x
int childrenInSysFlg = 0;  // -s x
int startOfSeqFlg = 0;     // -b B
int incrementFlg = 0;      // -i I
int outputFileFlg = 0;     // -o

void flgsPassedIn(int argc, char **argv) {

    // Used getopt to go through the parameter flgs that the user passes  
    int c = 0;

    while((c = getopt(argc, argv, "hn:s:b:i:o:")) != -1) 
    {
        switch(c) 
        {
            case 'h':
                helpMessageFlg = 1;
                break;
            case 'n':
                maxChildrenFlg = 1;
                break;
            case 's':
                childrenInSysFlg = 1;
                break;
            case 'b':
                startOfSeqFlg = 1;
                break;
            case 'i':
                incrementFlg = 1;
                break;
            case 'o':
                outputFileFlg = 1;
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


