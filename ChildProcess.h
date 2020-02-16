/* Author: Chase Richards
   Project: Homework 2 CS4760
   Date: February 16, 2020  
   Filename: ChildProcess.h  */

#ifndef CHILDPROCESS_H
#define CHILDPROCESS_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>

void childWork();
int deallocateMem(int shmid, void *shmaddr);

#endif
