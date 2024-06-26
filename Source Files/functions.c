#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>
#include "functions.h"
#include "formats.h"

int lineCount = 0;


char *requestFileName(void) { 
    char *file = malloc(100 * sizeof(char));
    if (!file) {
        fprintf(stderr,"memory allocation unsucessful");
        exit(1);
    }
    printf("What file would you like assembled?\nProvide the name with extension or the file path as such: /home/Documents/example.txt\n");
    if (fgets(file, 100, stdin ) != NULL) {
        printf("Opening: %s", file);
    } else { // Exit if the filename or path doesn't exist
        fprintf(stderr,"Error reading input.\n");
        free(file);
        exit(1);
    }
    size_t len = strlen(file);
    if (len > 0 && file[len - 1] == '\n') {
        file[len - 1] = '\0';
    }
    return(file);
}

FILE *openFile(char *fileToOpen, const char *mode) { 
    FILE *ASMfile = fopen(fileToOpen, mode);
    if (ASMfile ==  NULL) {
        if (strcmp(mode, "r") == 0){
            fprintf(stderr,"Could not open file.\n");
        } else if (strcmp(mode, "w") == 0) {
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

char **readFile(FILE *fileToRead) {
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

void assembleLines(char **arrayOfLines, FILE *newFile) {
    for (int i = 0; i < lineCount; i++) {
         // splits the instruction string into individual parts
        char **tempInstruction = rearrangeString(arrayOfLines[i]);
        // checks the format type of the instruction
        int format = checkFormat(tempInstruction[0]); 

        switch (format) {
            case 0:
                AssemblerTypeR(tempInstruction, newFile);
                break;/*
            case 1:
                AssemblerTypeS(tempInstruction);
                break;
            case 2:
                AssemblerTypeI(tempInstruction);
                break;
            case 3:
                AssemblerTypeSB(tempInstruction);
                break;
            case 4:
                AssemblerTypeU(tempInstruction);
                break;
            case 5:
                AssemblerTypeUJ(tempInstruction);
                break; */
            default:
                exit(5);

        }
        free(tempInstruction);
        printf("\n");
    }
}

char **rearrangeString(const char *string) {
    char **dissectedString = malloc(4 * sizeof(char*));
    char *stringCopy = strdup(string);
    char *token = strtok(stringCopy, " (");
    int i = 0;
    while (token != NULL && i < 4) {
        dissectedString[i] = strdup(token);
        if (dissectedString[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(dissectedString[j]);
            }
            free(dissectedString);
            free(stringCopy);
            exit(6);
        }
        i++;
        token = strtok(NULL, " ,()x");
    }
    free(stringCopy);

    // Make sure the array is null-terminated
    if (i < 4) {
        dissectedString[i] = NULL;
    }
    printf("%s %s %s %s", dissectedString[0],dissectedString[1],dissectedString[2],dissectedString[3]);
    return dissectedString;
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

char *intToBinaryStr(char *intStr, int bitLen, char *buffer) {
    int binaryNum[32] = {0};
    int i = 0, negative = 0, result = 0;

    // Checks if the number is negative
    if (intStr[0] == '-') {
        negative = 1;
        i++;
    }

    // Turns string into int
    for (; intStr[i] != '\0'; i++) {
        result = result * 10 + (intStr[i] - '0');
    }

    // If the number is negative, take two's complement
    if (negative) {
        result = -result;
    }

    // Convert number to binary representation
    i = 0; // Reset i to reuse for binary conversion
    while (result > 0 && i < 32) {
        binaryNum[31 - i] = result % 2;
        result /= 2;
        i++;
    }

    // If the number was negative, take two's complement
    if (negative) {
        for (i = 0; i < 32; i++) {
            binaryNum[i] = !binaryNum[i];
        }
        // Add 1 to the binary number to complete two's complement
        for (i = 31; i >= 0; i--) {
            if (binaryNum[i] == 0) {
                binaryNum[i] = 1;
                break;
            } else {
                binaryNum[i] = 0;
            }
        }
    }

    // Prepare the binary string of length bitLen
    for (i = 0; i < bitLen; i++) {
        buffer[i] = (char)(binaryNum[32 - bitLen + i] + '0');
    }
    
    buffer[bitLen] = '\0';

    return buffer;
}

char **binaryStringShuffler(char *binaryString) {
    // takes a binary string, cuts it into pieces and according to each format, shuffles them
}

void AssemblerTypeR(char **strings, FILE *newFile) {
    char assembledInstruction[34] = {0};
    char rs2[6] = {0};
    char rs1[6] = {0};
    char rd[6] = {0};

    intToBinaryStr(strings[3], 5, rs2);
    intToBinaryStr(strings[2], 5, rs1);
    intToBinaryStr(strings[1], 5, rd);

    for (int i = 0; i < FORMATLEN; i++) {
        for (int j = 0; j < formatSizes[i]; j++) {
            if (strcmp(strings[0], pFormats[i][j]) == 0) { 
                strncpy(assembledInstruction, RformatFunct7[j], 7);         // funct7
                strncpy(assembledInstruction + 7, rs2, 5);                  //rs2
                strncpy(assembledInstruction + 12, rs1, 5);                 //rs1                
                strncpy(assembledInstruction + 17, RformatFunct3[j], 3);    // funct3
                strncpy(assembledInstruction + 20, rd, 5);                  //rd                
                strncpy(assembledInstruction + 25, RformatOpcode[j], 7);    // Opcode
            }
        }
    }
    assembledInstruction[32] = '\n';
    assembledInstruction[33] = '\0';

    // write assembledInstruction to file
    fprintf(newFile, assembledInstruction);
}
/*
void AssemblerTypeS(char **strings) { 
    int assembledInstruction[34];
    assembledInstruction[33] = '\0';
    assembledInstruction[32] = '\n';

    for (int i = 0; i < FORMATLEN; i++) {
        for (int j = 0; j < formatSizes[i]; j++) {
            if (strcmp(strings[0], pFormats[i][j]) == 0) { 
                //  make a function here to split the binary string and return it as an array of strings
                strncpy(assembledInstruction + 7, intToBinaryStr(strings[1]), 5);   // immediate[11:5]
                strncpy(assembledInstruction + 7, intToBinaryStr(strings[1], 5, 'u'), 5);   //rs2
                strncpy(assembledInstruction + 12, intToBinaryStr(strings[1], 5, 'u'), 5);  //rs1                
                strncpy(assembledInstruction, RformatFunct3[j], 3);                 // funct3
                strncpy(assembledInstruction + 12, intToBinaryStr(strings[1]), 5);  //rs1                
                strncpy(assembledInstruction, RformatOpcode[j], 7);                 // immediate[4:0]
            }
        }
    }

    // write assembledInstruction to file
    fprintf(newFile, assembledInstruction);
}

//  All functions below have to still be changed entirely ###########################################
void AssemblerTypeI(char **strings) {
    int assembledInstruction[34];
    assembledInstruction[33] = '\0';
    assembledInstruction[32] = '\n';

    for (int i = 0; i < FORMATLEN; i++) {
        for (int j = 0; j < formatSizes[i]; j++) {
            if (strcmp(strings[0], pFormats[i][j]) == 0) { 
                strncpy(assembledInstruction, RformatFunct7[j], 7);                 // immediate[11:0]
                strncpy(assembledInstruction + 7, intToBinaryStr(strings[1]), 5);   //rs1
                strncpy(assembledInstruction, RformatFunct3[j], 3);                 // funct3
                strncpy(assembledInstruction + 12, intToBinaryStr(strings[1]), 5);  //rd                
                strncpy(assembledInstruction, RformatOpcode[j], 7);                 // Opcode
            }
        }
    }

    // write assembledInstruction to file
    fprintf(newFile, assembledInstruction);
}

void AssemblerTypeSB(char **strings) {
    int assembledInstruction[34];
    assembledInstruction[33] = '\0';
    assembledInstruction[32] = '\n';

    for (int i = 0; i < FORMATLEN; i++) {
        for (int j = 0; j < formatSizes[i]; j++) {
            if (strcmp(strings[0], pFormats[i][j]) == 0) { 
                strncpy(assembledInstruction, RformatFunct7[j], 7);                 // immediate[12]
                strncpy(assembledInstruction, RformatFunct7[j], 7);                 // immediate[10:5]
                strncpy(assembledInstruction + 7, intToBinaryStr(strings[1]), 5);   // rs2
                strncpy(assembledInstruction + 12, intToBinaryStr(strings[1]), 5);  // rs1                
                strncpy(assembledInstruction, RformatFunct3[j], 3);                 // funct3
                strncpy(assembledInstruction + 12, intToBinaryStr(strings[1]), 5);  // immediate[4:1]                
                strncpy(assembledInstruction, RformatOpcode[j], 7);                 // immediate[11]
                strncpy(assembledInstruction, RformatFunct7[j], 7);                 // Opcode

            }
        }
    }

    // write assembledInstruction to file
    fprintf(newFile, assembledInstruction);
}

void AssemblerTypeU(char **strings) {
    int assembledInstruction[34];
    assembledInstruction[33] = '\0';
    assembledInstruction[32] = '\n';

    for (int i = 0; i < FORMATLEN; i++) {
        for (int j = 0; j < formatSizes[i]; j++) {
            if (strcmp(strings[0], pFormats[i][j]) == 0) { 
                strncpy(assembledInstruction, RformatFunct7[j], 7);                 // immediate [31:12]
                strncpy(assembledInstruction + 7, intToBinaryStr(strings[1]), 5);   // rd
                strncpy(assembledInstruction, RformatOpcode[j], 7);                 // Opcode
            }
        }
    }

    // write assembledInstruction to file
    fprintf(newFile, assembledInstruction);
}

void AssemblerTypeUJ(char **strings) {
    int assembledInstruction[34];
    assembledInstruction[33] = '\0';
    assembledInstruction[32] = '\n';

    for (int i = 0; i < FORMATLEN; i++) {
        for (int j = 0; j < formatSizes[i]; j++) {
            if (strcmp(strings[0], pFormats[i][j]) == 0) { 
                strncpy(assembledInstruction, RformatFunct7[j], 7);                 // immediate [20]
                strncpy(assembledInstruction + 7, intToBinaryStr(strings[1]), 5);   // immediate [10:1]
                strncpy(assembledInstruction + 12, intToBinaryStr(strings[1]), 5);  // immediate [11]   
                strncpy(assembledInstruction, RformatFunct3[j], 3);                 // immediate [19:12]
                strncpy(assembledInstruction + 12, intToBinaryStr(strings[1]), 5);  // rd                
                strncpy(assembledInstruction, RformatOpcode[j], 7);                 // Opcode
            }
        }
    }

    // write assembledInstruction to file
    fprintf(newFile, assembledInstruction);
}
*/