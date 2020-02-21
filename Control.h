/* Author: Chase Richards
   Project: Homework 2 CS4760
   Date: February 15, 2020
   Filename: Control.h  */

#ifndef CONTROL_H
#define CONTROL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <time.h>
#include <signal.h>

struct sharedMemory
{
    int nanoSeconds;
    int seconds;
    char duration[64];
    int *childProcArr;
};

struct sharedMemory* smPtr;

FILE *OUTFILE;

//Prototype for deallocating the shared memory
int deallocateMem(int shmid, void *shmaddr);

//Prototype for working with the shared memory from master
void sharedMemoryWork();

void launchChildren(int maxChildren, int childLimit, char *outFile);

void sigHandler(int sig);

void timeIncrementation();

void displayHelpMessage();

#endif
