/* Author: Chase Richards
   Project: Homework 2 CS4760
   Date: February 13, 2020  
   Filename: CommandOptions.h  */

#ifndef COMMANDOPTIONS_H
#define COMMANDOPTIONS_H

#include <stdio.h>
#include <stdlib.h>

extern int helpMessageFlg;    // -h
extern int maxChildrenFlg;    // -n x
extern int childrenInSysFlg;  // -s x
extern int startOfSeqFlg;     // -b B
extern int incrementFlg;      // -i I
extern int outputFileFlg;     // -o

void flgsPassedIn(int argc, char **argv);
void displayHelpMessage();

#endif
