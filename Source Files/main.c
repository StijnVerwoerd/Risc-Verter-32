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
    free(filename);
    FILE *newFile = openFile(newFileName, "w");
    free(newFileName);

    // read and assemble file
    char **fileData = readFile(fileToAssemble);
    assembleLines(fileData, newFile);

    //printf("%s created succesfully\n", newFileName);
    // Close file and exit program succesfully
    return 0;
}





