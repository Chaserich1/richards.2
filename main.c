/* Author: Chase Richards
   Project: Homework 2 CS4760
   Date February 13, 2020  
   Filename: main.c  */

#include "CommandOptions.h"

int main(int argc, char* argv[]) 
{
 
    flgsPassedIn(argc, argv);
   
    if(helpMessageFlg)
    {
        displayHelpMessage();
        exit(1);
    }



   
    return 0;
}
