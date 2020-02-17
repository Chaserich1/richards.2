/* Author: Chase Richards
   Project: Homework 2 CS4760
   Date: February 13, 2020  
   Filename: CommandOptions.h  */

#ifndef COMMANDOPTIONS_H
#define COMMANDOPTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern int maxChildren;
extern int childLimit;
extern int startOfSeq;
extern int incrementVal;

void flgsPassedIn(int argc, char **argv);
void displayHelpMessage();

#endif
