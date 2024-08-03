#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "../raylib/src/raylib.h"
#include "../raygui/src/raygui.h"
#include "../raygui/src/gui_window_file_dialog.h"

//-------------------------------------------------------------------------------------------------------------------//
// Variables                                                                                                         //
//-------------------------------------------------------------------------------------------------------------------//

// button struct 
//---------------------------------------------------------------------------------------
typedef struct {
    Rectangle bounds;
    const char *text;
    bool clicked;
} button;

// window struct
//---------------------------------------------------------------------------------------
typedef struct {
    Rectangle bounds;
    const char *text;
    bool active;
} window;

//buttons
//---------------------------------------------------------------------------------------
extern button openFileButton;
extern button convertToBinaryButton;
extern button saveBinaryButton;
extern button copyBinaryButton;
extern button convertToHexButton;
extern button saveHexButton;
extern button copyHexButton;

// windows 
//---------------------------------------------------------------------------------------
extern window loadFileWindow;
extern window saveBinaryWindow;
extern window saveHexWindow; 


//-------------------------------------------------------------------------------------------------------------------//
// Functions                                                                                                         //
//-------------------------------------------------------------------------------------------------------------------//

// Gui functions
//---------------------------------------------------------------------------------------
void initGui(void);                                                 // Initializes Gui values & elements
void updateGui(void);                                               // Updates the Gui elements based on (button) triggers
void drawGui(void);                                                 // Draws the Gui elements
void textScrollWindow(                                              // opens a scrollbox with text
    Rectangle controlBounds, const char *textField, 
    int textLines, int *scrollDistance, bool *scrollActive);  
void openFileWindow();                                              // Load new file window
void saveFileWindow(char type);                                     // opens a save file window, type is 'h' for hex and 'b' for binary

// String manipulation functions
//---------------------------------------------------------------------------------------
void freeMemory(void);                                              // Frees all allocated heap memory
char *combineStrings(char **strings, int count);                    // Combines an array of strings into one long string seperated by '\n' tokens
char *createNumberedString(int start, int end);                     // Creates a string of nummbers (i.e. 1. 2. 3. etc.) that can be placed besides the text
char **binToHexConvert(char **binaryFileData,  int lines);          // converts an array of binary strings to an array of hexadecimal strings
char **rearrangeString(const char *string);                         // tokenizes an instruction string
char *intToBinaryStr(char *intStr, int bitLen, char *buffer);       // turns an integer in string format into a binary code in string format

// File functions
//---------------------------------------------------------------------------------------
FILE *openFile(char *fileToOpen, const char *mode);                 // opens or creates the assembly instructions file "r" for read "w" for write "a" for add data to file 
int checkFormat(const char *instruction);                           // checks the format of the instruction 
char *newFileNameGenerator(char *tempName, char type);              // creates a new file name
char **readFile(FILE *fileToRead, int *lineCount);                  // reads each line inside the file and stores it in an array 

// Assembler functions
//--------------------------------------------------------------------------------------- 
char **assembleLines(char **arrayOfLines, int lineCount);           // goes through each line, deconstructs it and writes it to the new file
char *AssemblerTypeR(char **strings);                               // Assembles each line in the R format                               
char *AssemblerTypeS(char **strings);                               // Assembles each line in the S format
char *AssemblerTypeI(char **strings);                               // Assembles each line in the I format
char *AssemblerTypeSB(char **strings);                              // Assembles each line in the SB format
char *AssemblerTypeU(char **strings);                               // Assembles each line in the U format
char *AssemblerTypeUJ(char **strings);                              // Assembles each line in the UJ format

#endif