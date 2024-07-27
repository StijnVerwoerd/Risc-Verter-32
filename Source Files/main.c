#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "functions.h"

int main (void) {
    //request file and open it
    char *filename = requestFileName();
    FILE *fileToAssemble = openFile(filename, "r");

    // Ask if you want to encode or decode, and wether it's in binary or hexadecimal form.

    // create destination file
    char *newFileName = newFileNameGenerator(filename);
    free(filename);
    FILE *newFile = openFile(newFileName, "w");
    free(newFileName);

    // read and assemble file
    int lineCount = 0;
    char **fileData = readFile(fileToAssemble, &lineCount);
    assembleLines(fileData, newFile, lineCount);

    // Close file and exit program succesfully
    return 0;
}





