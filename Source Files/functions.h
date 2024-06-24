#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdlib.h>
#include <stdio.h>

extern int lineCount;

// Function declarations
char *requestFileName(void); // To point out the obvious, this requests a user for the file
FILE *openFile(char *fileToOpen, const char *mode); //opens or creates the assembly instructions file "r" for read "w" for write "a" for add data to file
char *checkFormat(const char *instruction); //checks the format of the instruction
char *newFileNameGenerator(char *tempName); //creates a new file name
char *readFile(FILE *fileToRead); // reads each line and stores it in an array
void assembleLines(const char *arrayOfLines); //goes through each line, deconstructs it and writes it to the new file
char *rearrangeString(const char *string); // turns a single string into an array of individual strings


// Assembler functions
void AssemblerTypeR(char *strings);
void AssemblerTypeS(char *strings);
void AssemblerTypeI(char *strings);
void AssemblerTypeSB(char *strings);
void AssemblerTypeU(char *strings);
void AssemblerTypeUJ(char *strings);

#endif