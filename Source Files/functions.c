#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>
#include "functions.h"
#include "formats.h"


char *requestFileName(void) { 
    char *file = malloc(100 * sizeof(char));
    if (!file) {
        fprintf(stderr,"memory allocation unsucessful");
        exit(1);
    }
    printf("What file would you like assembled?\nProvide the name with extension or the file path as such: /home/Documents/example.txt\n");
    if (fgets(file, 100, stdin ) != NULL) {
        printf("Opening: %s", file);
        return file; 
    } else { // Exit if the filename or path doesn't exist
        fprintf(stderr,"Error reading input.\n");
        free(file);
        exit(1);
    }
}

FILE *openFile(char *fileToOpen, const char *mode) { 
    FILE *ASMfile = fopen(fileToOpen, mode);
    if (!ASMfile) {
        if (mode == "r"){
        fprintf(stderr,"Could not open file.\n");
        } else if (mode == "w") {
        fprintf(stderr,"Could not create file.\n");
        }
        exit(2); // Exit if file cannot be opened or created
    }
    return ASMfile;
}


char *newFileNameGenerator(char *tempName) {
    char toAppend[] = "_assembled";
    int newLength = strlen(tempName) + strlen(toAppend) + 1;

    char *newString = malloc(newLength * sizeof(char));
    if (!newString) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(3);
    }

    strcpy(newString, tempName);

    // Insert "_assembled" before ".txt"
    char *dotPosition = strrchr(newString, '.'); // Find the last occurrence of '.'
    if (dotPosition != NULL) {
        // Calculate the position of ".txt"
        int position = dotPosition - newString;

        // Move the ".txt" part to the end
        memmove(newString + position + strlen(toAppend), newString + position, strlen(dotPosition) + 1);

        // Insert the message
        memcpy(newString + position, toAppend, strlen(toAppend));
    } else {
        fprintf(stderr, "incorrect file format\n");
        exit(3);
    }
    return newString;
}

char *readFile(FILE *fileToRead) {
    int lineCount = 0;
    int arraySize = 10;
    char buffer[32];
    
    char **instructionLines = malloc(arraySize * sizeof(char));
    if (!instructionLines) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(4);        
    }

    while (fgets(buffer, 32, fileToRead)) {
        // Remove the newline character if present
        buffer[strcspn(buffer, "\n")] = '\0';

        // Resize array if needed
        if (lineCount >= arraySize) {
            arraySize *= 2;
            char **temp = realloc(instructionLines, arraySize * sizeof(char *));
            if (temp == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                for (size_t i = 0; i < lineCount; i++) {
                    free(instructionLines[i]);
                }
                free(instructionLines);
                fclose(fileToRead);
                exit(4);
            }
            instructionLines = temp;
        }

        instructionLines[lineCount] = strdup(buffer);
        if (instructionLines[lineCount] == NULL) {
            perror("Failed to duplicate string");
            for (size_t i = 0; i < lineCount; i++) {
                free(instructionLines[i]);
            }
            free(instructionLines);
            fclose(fileToRead);
            exit(4);
        }

        lineCount++;
    }
    fclose(fileToRead);
    return(instructionLines);
}

void assembleLines(const char *arrayOfLines) {
    for (int i = 0; i < lineCount; i++) {
        char *tempInstruction[4];
        // put function here to process the string
        char *tempInstruction[] = rearrangeString(arrayOfLines);

        int format = checkFormat(tempInstruction[0]);

        switch (format) {
            case 0:
                AssemblerTypeR(tempInstruction);
            case 1:
                AssemblerTypeS(tempInstruction);
            case 2:
                AssemblerTypeI(tempInstruction);
            case 3:
                AssemblerTypeSB(tempInstruction);
            case 4:
                AssemblerTypeU(tempInstruction);
            case 5:
                AssemblerTypeUJ(tempInstruction);
        }
    }
}

char *rearrangeString(const char *string) { // ***FUNCTION NEEDS TO BE MADE

}

int checkFormat(const char *instruction) { 
    for (int i = 0; i < FORMATLEN; i++) {
        for (int j = 0; j < formatSizes[i]; j++) {
            if (strcmp(instruction, pFormats[i][j]) == 0) { 
                return formatNumber[i];
            }
        }
    }
}

void AssemblerTypeR(char *strings) {

}

void AssemblerTypeS(char *strings) {

}

void AssemblerTypeI(char *strings) {

}

void AssemblerTypeSB(char *strings) {

}

void AssemblerTypeU(char *strings) {

}

void AssemblerTypeUJ(char *strings) {
    
}