#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "../raylib/src/raylib.h"
#include "../raygui/src/raygui.h"
#include "../raygui/src/gui_window_file_dialog.h"


// new file
extern FILE *newFile;

//-------------------------------------------------------------------------------------------------------------------//
// GUI                                                                                                               //
//-------------------------------------------------------------------------------------------------------------------//

    // main gui call ------------------------------------------------------------------------
    void initGui(void);
    void updateGui(void);
    void drawGui(void);

    // button struct ------------------------------------------------------------------------
    typedef struct {
        Rectangle bounds;
        const char *text;
        bool clicked;
    } button;

    // window struct ------------------------------------------------------------------------

    typedef struct {
        Rectangle bounds;
        const char *text;
        bool active;
    } window;

    //buttons -------------------------------------------------------------------------------

    extern button openFileButton;
    extern button convertToBinaryButton;
    extern button saveBinaryButton;
    extern button copyBinaryButton;
    extern button convertToHexButton;
    extern button saveHexButton;
    extern button copyHexButton;

    // windows ------------------------------------------------------------------------------

    extern window loadFileWindow;
    extern window saveBinaryWindow;
    extern window saveHexWindow; 

    // Gui functions ------------------------------------------------------------------------

    void textScrollWindow(Rectangle controlBounds, const char *textField, int textLines, int *scrollDistance, bool *scrollActive);  // opens a scrollbox with text
    void openFileWindow();                                                                                                          // opens a load file window
    void saveFileWindow(char type);                                                                                                 // opens a save file window, type is 'h' for hex and 'b' for binary

//-------------------------------------------------------------------------------------------------------------------//
// Background Functions                                                                                              //
//-------------------------------------------------------------------------------------------------------------------//
    void freeMemory(void);
    char *combineStrings(char **strings, int count);
    /*
    FILE *openFileWindow(void);
    char **convertToBinary(void);
    char **convertToHex(void);
    */  

    // --------------------------------------------------------------------------------

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