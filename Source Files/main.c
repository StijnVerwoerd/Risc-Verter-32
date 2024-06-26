#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "functions.h"

int main (void) {
    //request file and open it
    char *filename = requestFileName();
    FILE *fileToAssemble = openFile(filename, "r");

    // create destionation file
    char *newFileName = newFileNameGenerator(filename);
    FILE *newFile = openFile(newFileName, "w");
    free(filename);
    free(newFileName);


    // function to go over each line untill there are no more lines
    // function should split each line and place it in an array
    // format of the instruction should be called through function
    // correct function should be called to assemble the data and write it to the file(newFile)
    char **fileData = readFile(fileToAssemble);
    assembleLines(fileData, newFile);


    // Close file and exit program succesfully
    return 0;
}





