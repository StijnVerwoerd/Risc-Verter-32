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

char **readFile(FILE *fileToRead, int *lineCount) {
    int arraySize = 10;
    char buffer[32];
    
    char **instructionLines = malloc(arraySize * sizeof(char*));
    if (!instructionLines) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(4);        
    }

    while (fgets(buffer, 32, fileToRead)) {
        // Remove the newline character if present
        buffer[strcspn(buffer, "\n")] = '\0';

        // Resize array if needed
        if (*lineCount >= arraySize) {
            arraySize *= 2;
            char **temp = realloc(instructionLines, arraySize * sizeof(char *));
            if (temp == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                for (size_t i = 0; i < *lineCount; i++) {
                    free(instructionLines[i]);
                }
                free(instructionLines);
                fclose(fileToRead);
                exit(4);
            }
            instructionLines = temp;
        }

        instructionLines[*lineCount] = strdup(buffer);
        if (instructionLines[*lineCount] == NULL) {
            perror("Failed to duplicate string");
            for (size_t i = 0; i < *lineCount; i++) {
                free(instructionLines[i]);
            }
            free(instructionLines);
            fclose(fileToRead);
            exit(4);
        }

        *lineCount = *lineCount + 1;
    }
    fclose(fileToRead);
    return(instructionLines);
}

void assembleLines(char **arrayOfLines, FILE *newFile, int lineCount) {
    for (int i = 0; i < lineCount; i++) {
         // splits the instruction string into individual parts
        char **tempInstruction = rearrangeString(arrayOfLines[i]);
        // checks the format type of the instruction
        int format = checkFormat(tempInstruction[0]); 
        switch (format) {
            case 0:
                AssemblerTypeR(tempInstruction, newFile);
                break;
            case 1:
                AssemblerTypeS(tempInstruction, newFile);
                break;
            case 2:
                AssemblerTypeI(tempInstruction, newFile);
                break;
            case 3:
                AssemblerTypeSB(tempInstruction, newFile);
                break;
            case 4:
                AssemblerTypeU(tempInstruction, newFile);
                break;
            case 5:
                AssemblerTypeUJ(tempInstruction, newFile);
                break;
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
    int i = 0;

    // while token is not null and i < 4
    char *token = strtok(stringCopy, " ,()x");
    while (token != NULL) {
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
    if (i < 3) {
        dissectedString[i] = NULL;
    }

    // Print tokens 
    for (int j = 0; j < 4; j++) {
        if (dissectedString[j] != NULL) {
            printf("%s ", dissectedString[j]);
        }
    }

    //return splitted string
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
    if (strchr(intStr, '-')) {
        negative = 1;
        i++;
    }

    // Turns string into int
    for (; intStr[i] != '\0'; i++) {
        result = result * 10 + (intStr[i] - '0');
    }

    // Convert number to binary representation
    i = 0; // Reset i to reuse for binary conversion
    while (result > 0 && i < 32) {
        binaryNum[31 - i] = result % 2;
        result /= 2;
        i++;
    }

    // If the number was negative, take two's complement
    if (negative == 1) {
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

void AssemblerTypeR(char **strings, FILE *newFile) {
    char assembledInstruction[34] = {0};
    char rs2[6] = {0};
    char rs1[6] = {0};
    char rd[6] = {0};

    intToBinaryStr(strings[3], 5, rs2);
    intToBinaryStr(strings[2], 5, rs1);
    intToBinaryStr(strings[1], 5, rd);

    // Find correct codes in formats and insert correct values in the final string
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

void AssemblerTypeS(char **strings, FILE *newFile) {
    char assembledInstruction[34] = {0};
    char imm[13] = {0};
    char imm0_4[6] = {0};
    char imm5_11[8] = {0};
    char rs2[6] = {0};
    char rs1[6] = {0};


    intToBinaryStr(strings[1], 5, rs2);
    intToBinaryStr(strings[3], 5, rs1);
    intToBinaryStr(strings[2], 12, imm);

    // Put code that puts the correct part of the strings in each respective place
    strncpy(imm5_11, imm, 7);
    imm5_11[7] = '\0';
    strncpy(imm0_4, imm + 7, 5);
    imm0_4[5] = '\0';

    // Find correct codes in formats and insert correct values in the final string
    for (int i = 0; i < FORMATLEN; i++) {
        for (int j = 0; j < formatSizes[i]; j++) {
            if (strcmp(strings[0], pFormats[i][j]) == 0) { 
                strncpy(assembledInstruction, imm5_11, 7);                  // imm 11:5
                strncpy(assembledInstruction + 7, rs2, 5);                  // rs2
                strncpy(assembledInstruction + 12, rs1, 5);                 // rs1                
                strncpy(assembledInstruction + 17, SformatFunct3[j], 3);    // funct3
                strncpy(assembledInstruction + 20, imm0_4, 5);              // imm 0:4                
                strncpy(assembledInstruction + 25, SformatOpcode[j], 7);    // Opcode
            }
        }
    }

    assembledInstruction[32] = '\n';
    assembledInstruction[33] = '\0';
    

    // write assembledInstruction to file
    fprintf(newFile, assembledInstruction);
}

void AssemblerTypeI(char **strings, FILE *newFile) {
    char assembledInstruction[34] = {0};
    char imm[13] = {0};
    char rs1[6] = {0};
    char rd[6] = {0};

    intToBinaryStr(strings[3], 12, imm);
    intToBinaryStr(strings[2], 5, rs1);
    intToBinaryStr(strings[1], 5, rd);

    // Find correct codes in formats and insert correct values in the final string
    for (int i = 0; i < FORMATLEN; i++) {
        for (int j = 0; j < formatSizes[i]; j++) {
            if (strcmp(strings[0], pFormats[i][j]) == 0) { 
                strncpy(assembledInstruction, imm, 12);                     // imm 11:0
                strncpy(assembledInstruction + 12, rs1, 5);                 // rs1                
                strncpy(assembledInstruction + 17, IformatFunct3[j], 3);    // funct3
                strncpy(assembledInstruction + 20, rd, 5);                  // rd                
                strncpy(assembledInstruction + 25, IformatOpcode[j], 7);    // Opcode
            }
        }
    }

    assembledInstruction[32] = '\n';
    assembledInstruction[33] = '\0';

    // write assembledInstruction to file
    fprintf(newFile, assembledInstruction);
}

void AssemblerTypeSB(char **strings, FILE *newFile) { 
    char assembledInstruction[34] = {0};
    char imm[14] = {0};         // the immediate of SB types actually have 13 bits, the first bit just does not get included (as you can only jump on at minimum half word lengths)
    char imm1_4[5] = {0};       // so that means that we will take out bit #0 out later, it's implied.
    char imm5_10[7] = {0};
    char imm11[2] = {0};
    char imm12[2] = {0};
    char rs2[6] = {0};
    char rs1[6] = {0};

    intToBinaryStr(strings[2], 5, rs2);
    intToBinaryStr(strings[1], 5, rs1);
    intToBinaryStr(strings[3], 13, imm);

    // Put code that puts the correct part of the strings in each respective place
    strncpy(imm12, imm, 1);
    imm12[1] = '\0';
    strncpy(imm11, imm + 1, 1);
    imm11[1] = '\0';
    strncpy(imm5_10, imm + 2, 6);
    imm5_10[6] = '\0';
    strncpy(imm1_4, imm + 8, 4);
    imm1_4[4] = '\0';

    // Find correct codes in formats and insert correct values in the final string
    for (int i = 0; i < FORMATLEN; i++) {
        for (int j = 0; j < formatSizes[i]; j++) {
            if (strcmp(strings[0], pFormats[i][j]) == 0) { 
                strncpy(assembledInstruction, imm12, 1);                    // imm 12
                strncpy(assembledInstruction + 1, imm5_10, 6);              // imm 10:5
                strncpy(assembledInstruction + 7, rs2, 5);                  // rs2
                strncpy(assembledInstruction + 12, rs1, 5);                 // rs1                
                strncpy(assembledInstruction + 17, SBformatFunct3[j], 3);   // funct3
                strncpy(assembledInstruction + 20, imm1_4, 4);              // imm 4:1 
                strncpy(assembledInstruction + 24, imm11, 1);               // imm 11               
                strncpy(assembledInstruction + 25, SBformatOpcode[j], 7);   // Opcode
            }
        }
    }

    assembledInstruction[32] = '\n';
    assembledInstruction[33] = '\0';
    
    // write assembledInstruction to file
    fprintf(newFile, assembledInstruction);
}

void AssemblerTypeU(char **strings, FILE *newFile) { // remember that SB has an implied 0 where bit 7 should be, it's simply not included
    char assembledInstruction[34] = {0};
    char imm[21] = {0};
    char rd[6] = {0};
    intToBinaryStr(strings[1], 5, rd);
    intToBinaryStr(strings[2], 20, imm);

    // Find correct codes in formats and insert correct values in the final string
    for (int i = 0; i < FORMATLEN; i++) {
        for (int j = 0; j < formatSizes[i]; j++) {
            if (strcmp(strings[0], pFormats[i][j]) == 0) { 
                strncpy(assembledInstruction, imm, 20);                     // imm 12
                strncpy(assembledInstruction + 20, rd, 5);                  // rd            
                strncpy(assembledInstruction + 25, UformatOpcode[j], 7);    // Opcode
            }
        }
    }

    assembledInstruction[32] = '\n';
    assembledInstruction[33] = '\0';
    
    // write assembledInstruction to file
    fprintf(newFile, assembledInstruction);
}

void AssemblerTypeUJ(char **strings, FILE *newFile) { // UJ type has a similar type of encoding as SB where the 0th bit is an implied 0
    char assembledInstruction[34] = {0};
    char imm[22] = {0};         
    char imm1_10[11] = {0};
    char imm11[2] = {0};
    char imm12_19[9] = {0};       
    char imm20[2] = {0};
    char rd[6] = {0};

    intToBinaryStr(strings[1], 5, rd);
    intToBinaryStr(strings[2], 21, imm);

    // Put code that puts the correct part of the strings in each respective place
    strncpy(imm20, imm, 1); // puts imm[0] in imm20
    imm20[1] = '\0';
    strncpy(imm12_19, imm + 1, 8); // puts imm[1] till imm[8] in imm12_19
    imm12_19[9] = '\0';
    strncpy(imm11, imm + 9, 1); // puts imm[9] in imm11
    imm11[1] = '\0';
    strncpy(imm1_10, imm + 10, 10); //puts imm[10] till imm[19] in imm1_10
    imm1_10[10] = '\0';

    // Find correct codes in formats and insert correct values in the final string
    for (int i = 0; i < FORMATLEN; i++) {
        for (int j = 0; j < formatSizes[i]; j++) {
            if (strcmp(strings[0], pFormats[i][j]) == 0) { 
                strncpy(assembledInstruction, imm20, 1);                    // imm 20
                strncpy(assembledInstruction + 1, imm1_10, 10);             // imm 1:10               
                strncpy(assembledInstruction + 11, imm11, 1);               // imm 11
                strncpy(assembledInstruction + 12, imm12_19, 8);            // imm 12:19 
                strncpy(assembledInstruction + 20, rd, 5);                  // rd              
                strncpy(assembledInstruction + 25, UJformatOpcode[j], 7);   // Opcode
            }
        }
    }

    assembledInstruction[32] = '\n';
    assembledInstruction[33] = '\0';
    
    // write assembledInstruction to file
    fprintf(newFile, assembledInstruction);
}
