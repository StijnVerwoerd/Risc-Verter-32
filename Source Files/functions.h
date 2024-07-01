#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdlib.h>
#include <stdio.h>

// amount of lines that have to be assembled
extern int lineCount;

// new file
extern FILE *newFile;

// Function declarations

// To point out the obvious, this requests a user for the file
char *requestFileName(void); 
//opens or creates the assembly instructions file "r" for read "w" for write "a" for add data to file
FILE *openFile(char *fileToOpen, const char *mode); 
//checks the format of the instruction
int checkFormat(const char *instruction); 
//creates a new file name
char *newFileNameGenerator(char *tempName);
// reads each line and stores it in an array 
char **readFile(FILE *fileToRead, int *lineCount);
//goes through each line, deconstructs it and writes it to the new file
void assembleLines(char **arrayOfLines, FILE *newFile, int lineCount);
// turns a single string into an array of individual strings
char **rearrangeString(const char *string); 
// turns an integer in string format into a binary code in string format
char *intToBinaryStr(char *intStr, int bitLen, char *buffer);
// shuffles the digits according to the format rules
char **binaryStringShuffler(char *binaryString);


// Assembler functions 
void AssemblerTypeR(char **strings, FILE *newFile); 
void AssemblerTypeS(char **strings, FILE *newFile);
void AssemblerTypeI(char **strings, FILE *newFile);
void AssemblerTypeSB(char **strings, FILE *newFile);
void AssemblerTypeU(char **strings, FILE *newFile); 
void AssemblerTypeUJ(char **strings, FILE *newFile);

#endif