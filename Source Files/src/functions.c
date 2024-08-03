//-------------------------------------------------------------------------------------------------------------------//
// Libraries & definitions                                                                                           //
//-------------------------------------------------------------------------------------------------------------------//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>
#include "functions.h"
#include "formats.h"
#include "../raylib/src/raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "../raygui/src/raygui.h"
#undef RAYGUI_IMPLEMENTATION
#define GUI_WINDOW_FILE_DIALOG_IMPLEMENTATION
#include "../raygui/src/gui_window_file_dialog.h"
#include "../raygui/styles/dark/style_dark.h"

//-------------------------------------------------------------------------------------------------------------------//
// Setup & global variables                                                                                          //
//-------------------------------------------------------------------------------------------------------------------//

// Screen dimensions 
//---------------------------------------------------------------------------------------
const int screenWidth = 784;
const int screenHeight = 792;

// Custom file dialog 
//---------------------------------------------------------------------------------------
GuiWindowFileDialogState fileDialogState;
char fileNameToLoad[512];

// initialize buttons 
//---------------------------------------------------------------------------------------
button openFileButton = (button){ .bounds = (Rectangle){28, 24, 352, 24}, .text = "OPEN FILE", .clicked = false};
button convertToBinaryButton = (button){ .bounds = (Rectangle){404, 24, 228, 24}, .text = "CONVERT TO BINARY", .clicked = false};
button convertToHexButton = (button){ .bounds = (Rectangle){404, 408, 228, 24}, .text = "CONVERT TO HEX", .clicked = false};
button saveBinaryButton = (button){ .bounds = (Rectangle){634, 24, 60, 24}, .text = "SAVE", .clicked = false};
button copyBinaryButton = (button){ .bounds = (Rectangle){696, 24, 60, 24}, .text = "COPY", .clicked = false};
button saveHexButton = (button){ .bounds = (Rectangle){634, 408, 60, 24}, .text = "SAVE", .clicked = false};
button copyHexButton = (button){ .bounds = (Rectangle){696, 408, 60, 24}, .text = "COPY", .clicked = false};
bool guiButtonsActive = true;

// initialize file windows 
//---------------------------------------------------------------------------------------
window saveBinaryWindow = (window){ .bounds = (Rectangle){(screenWidth / 2 - 250), (screenHeight / 2 - 75), 500, 150}, .text = "SAVE BINARY FILE", .active = false};
window saveHexWindow = (window){ .bounds = (Rectangle){(screenWidth / 2 - 250), (screenHeight / 2 - 75), 500, 150}, .text = "SAVE HEX FILE", .active = false};
int textPosX = (screenWidth / 2) -230;

// Scroll variables 
//---------------------------------------------------------------------------------------
int scrollDistanceASM = 0;
int scrollDistanceBin = 0;
int scrollDistanceHex = 0;
bool scrollButtonPressed = false;
bool assemblyScrollActive = false;
bool binaryScrollActive = false;
bool hexScrollActive = false;

// amount of lines of code of the file to be assembled 
//---------------------------------------------------------------------------------------
int lineCount = 0;
char *lineNumbers;

// checks if the binary and hex values are already converted 
//---------------------------------------------------------------------------------------
bool binaryLoaded = false;
bool hexLoaded = false;

// holds the filedata 
//---------------------------------------------------------------------------------------
char **fileData = NULL;
char *processedFileData = NULL;
char **binaryFileData = NULL;
char *processedBinaryFileData = NULL;
char **hexFileData = NULL;
char *processedHexFileData = NULL;


//-------------------------------------------------------------------------------------------------------------------//
// GUI Functions                                                                                                     //
//-------------------------------------------------------------------------------------------------------------------//

void initGui(void) {

    // Window initialization 
    //---------------------------------------------------------------------------------------
    SetTargetFPS(60);
    InitWindow(screenWidth, screenHeight, "RV32I Assembler");
    GuiLoadStyleDark();

    // Custom file dialog
    //---------------------------------------------------------------------------------------
    fileDialogState = InitGuiWindowFileDialog(GetWorkingDirectory());
}

void updateGui(void) {

    // changes button states & triggers functions
    //---------------------------------------------------------------------------------------
    if (guiButtonsActive){

        if (GuiButton(openFileButton.bounds, openFileButton.text)) {
            openFileButton.clicked = true;
            fileDialogState.windowActive = true;
            convertToBinaryButton.clicked = false;
            convertToHexButton.clicked = false;
            
        }
        if (GuiButton(convertToBinaryButton.bounds, convertToBinaryButton.text)) {
            convertToBinaryButton.clicked = true;
            if (fileData != NULL){
                binaryFileData = assembleLines(fileData, lineCount);
                processedBinaryFileData = combineStrings(binaryFileData, lineCount);
                binaryLoaded = true;
            }
        }
        if (GuiButton(convertToHexButton.bounds, convertToHexButton.text)) {
            convertToHexButton.clicked = true;
            if (binaryLoaded) {
                hexFileData = binToHexConvert(binaryFileData,  lineCount);
                processedHexFileData = combineStrings(hexFileData, lineCount);
                hexLoaded = true;
            }
        }
        if (GuiButton(saveBinaryButton.bounds, saveBinaryButton.text) && binaryLoaded) {
            saveBinaryButton.clicked = true;
        }
        if (GuiButton(copyBinaryButton.bounds, copyBinaryButton.text) && binaryLoaded) {
            copyBinaryButton.clicked = true;
        }
        if (GuiButton(saveHexButton.bounds, saveHexButton.text) && hexLoaded) {
            saveHexButton.clicked = true;
        }
        if (GuiButton(copyHexButton.bounds, copyHexButton.text) && hexLoaded) {
            copyHexButton.clicked = true;
        }
    }

    // open file dialog 
    //---------------------------------------------------------------------------------------
    if (!fileDialogState.windowActive) {
        openFileButton.clicked = false;
        guiButtonsActive = true;
    }
    if (fileDialogState.SelectFilePressed) {

        // Load text file (if supported extension) 
        //---------------------------------------------------------------------------------------
        if (IsFileExtension(fileDialogState.fileNameText, ".txt")) {
            
            // copy file name into fileNameToLoad 
            //---------------------------------------------------------------------------------------
            strcpy(fileNameToLoad, TextFormat("%s" PATH_SEPERATOR "%s", fileDialogState.dirPathText, fileDialogState.fileNameText));

            // Load text 
            //---------------------------------------------------------------------------------------
            FILE *fileToAssemble = openFile(fileNameToLoad, "r");
            fileData = readFile(fileToAssemble, &lineCount);
            processedFileData = combineStrings(fileData, lineCount);

            // create number list 
            //---------------------------------------------------------------------------------------
            lineNumbers = createNumberedString(1, lineCount);
        }
    }
}

void openFileWindow(void) {

    // Cast dark layer over the rest of the UI
    //---------------------------------------------------------------------------------------
    DrawRectangle(0,0,screenWidth,screenHeight, (Color){ 0, 0, 0, 100 });

    // Draw file dialog 
    //---------------------------------------------------------------------------------------
    GuiWindowFileDialog(&fileDialogState);

    // disable previously loaded conversions
    //---------------------------------------------------------------------------------------
    binaryLoaded = false;
    hexLoaded = false;
}

void saveFileWindow(char type) {

    // Cast dark layer over the rest of the UI 
    //---------------------------------------------------------------------------------------
    DrawRectangle(0,0,screenWidth,screenHeight, (Color){ 0, 0, 0, 100 });

    // closes save file window if pressed on x
    //---------------------------------------------------------------------------------------
    if (type == 'b') {
        if (GuiWindowBox(saveBinaryWindow.bounds, saveBinaryWindow.text)) {
            guiButtonsActive = true;
            saveBinaryButton.clicked = false;
        }
    }
    if (type == 'h') {
        if (GuiWindowBox(saveHexWindow.bounds, saveHexWindow.text)) {
            guiButtonsActive = true;
            saveHexButton.clicked = false;
        }
    }

    // Draw Name of file
    //---------------------------------------------------------------------------------------
    int borderColor;
    if (GetMouseX() > (screenWidth/2)-234 && GetMouseX() < (screenWidth/2)+234 && GetMouseY() > (screenHeight/2)-16 && GetMouseY() < (screenHeight/2)+16) {
        textPosX = GetMouseX() - 200;
        borderColor = BORDER_COLOR_FOCUSED;
    } else {
        borderColor = BORDER_COLOR_NORMAL;
    }
    char *fileName = newFileNameGenerator(fileNameToLoad, type);
    GuiDrawText("FILEPATH:", (Rectangle){(screenWidth/2)-234, (screenHeight/2)-32, 100, 24}, 0, WHITE);
    GuiDrawRectangle((Rectangle){(screenWidth/2)-234, (screenHeight/2)-8, 468, 24}, 1, GetColor(GuiGetStyle(DEFAULT, borderColor)), BLANK);
    BeginScissorMode((screenWidth/2)-234, (screenHeight/2)-16, 468, 32);
    GuiDrawText(fileName, (Rectangle){textPosX, (screenHeight/2) -8, 1000, 24}, 0, GREEN);
    EndScissorMode();
    
    // Save data to file
    //---------------------------------------------------------------------------------------
    if(GuiButton((Rectangle){(screenWidth / 2)+164, (screenHeight / 2)+36, 72, 24}, "SAVE FILE" )) {
        if (type == 'b') {
            fputs(processedBinaryFileData, openFile(fileName, "w"));
            saveBinaryButton.clicked = false; 
        }
        if (type == 'h') {
            fputs(processedHexFileData, openFile(fileName, "w"));
            saveHexButton.clicked = false;
        }
    }

    //
}

void textScrollWindow(Rectangle controlBounds, const char *textField, int textLines, int *scrollDistance, bool *scrollActive) {

    // scroll variables 
    //---------------------------------------------------------------------------------------
    int textHeight = textLines * 24;
    int scrollBarLength = (textHeight - controlBounds.height);
    int scissorX = controlBounds.x + 8, scissorY = controlBounds.y + 12, scissorWidth = controlBounds.width - 32, scissorHeight = controlBounds.height - 16;
    Rectangle textBounds = {controlBounds.x + 12, controlBounds.y + 16, scissorWidth, textHeight};
    Rectangle scrollBarBounds = {(controlBounds.x + controlBounds.width -24), controlBounds.y + 8, 16, controlBounds.height - 16};
    
    // If the mouse is pressed down and within bounds, it can drag the scroll bar 
    //---------------------------------------------------------------------------------------
    if (GetMouseY() > scrollBarBounds.y - 1&& 
        GetMouseY() < scrollBarBounds.y + scrollBarBounds.height + 4 && 
        IsMouseButtonDown(MOUSE_LEFT_BUTTON) &&
        GetMouseX() > scrollBarBounds.x - 8 &&
        GetMouseX() < scrollBarBounds.x + scrollBarBounds.width + 8) {

        scrollButtonPressed = true;
        *scrollActive = true;
    } else if (!IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
        scrollButtonPressed = false;
        *scrollActive = false;
    }

    // if the scroll button was pressed and the scroll set to active  
    // update the text and scrollwheel accordingly
    //---------------------------------------------------------------------------------------
    if (scrollButtonPressed && *scrollActive == true) {
        *scrollDistance = (GetMouseY() - scrollBarBounds.y) * ((scrollBarLength) / scrollBarBounds.height);
        if (*scrollDistance < 0) {
            *scrollDistance = 0;
        } else if (*scrollDistance > scrollBarLength) {
            *scrollDistance = scrollBarLength + 12;
        }
    }
    
    // dynamic text colors 
    //---------------------------------------------------------------------------------------
    int Color1 = (GetMouseX() / 8) + 150;
    int Color2 = (GetMouseY() / 8) + 150;
    int Color3 = (Color1 + Color2 - 50) / 3;
    int Color4 = (Color1 + Color2 - 50) / 4;

    // if there are less than 14 lines, do not draw a scrollbar 
    //---------------------------------------------------------------------------------------
    if (textLines < 14) {
        BeginScissorMode(scissorX, scissorY, scissorWidth, scissorHeight);
            GuiDrawText(textField, (Rectangle){textBounds.x+24, textBounds.y, textBounds.width, textBounds.height}, 0, WHITE );
            GuiDrawText(lineNumbers, (Rectangle){textBounds.x-4, textBounds.y, textBounds.width, textBounds.height}, 0, (Color){Color1, Color2, Color3, 255} );
        EndScissorMode();    
    } else {
        GuiScrollBar(scrollBarBounds, *scrollDistance, 0, scrollBarLength);
        BeginScissorMode(scissorX, scissorY, scissorWidth, scissorHeight);
            GuiDrawText(textField, (Rectangle){textBounds.x+24, scrollBarBounds.y - *scrollDistance, textBounds.width, textBounds.height}, 0, WHITE );
            GuiDrawText(lineNumbers, (Rectangle){textBounds.x-4, scrollBarBounds.y - *scrollDistance, textBounds.width, textBounds.height}, 0, (Color){Color1, Color2, Color3, 255} );
        EndScissorMode();    
    }
}

void drawGui(void) {
        
        // Disables the GUI underneath the file windows if they are enabled 
        //---------------------------------------------------------------------------------------
        if (openFileButton.clicked | saveBinaryButton.clicked | saveHexButton.clicked) {
            GuiDisable();
        } else {
            GuiEnable();
        }

        // Draw background color 
        //---------------------------------------------------------------------------------------
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

        // Rectangle colors 
        //---------------------------------------------------------------------------------------
        int Color1 = 150;
        int Color2 = 75;
        int Color3 = (Color1 + Color2 - 50) / 2;
        
        // draw dynamic gradient background 
        //---------------------------------------------------------------------------------------
        DrawRectangleGradientEx((Rectangle){0,0,screenWidth,screenHeight},
            (Color){50, 150, 100, 50},
            (Color){100, 50, 150, 50},
            (Color){150, 100, 50, 50},
            (Color){50, 150, 100, 50});

        // draw button panels 
        //---------------------------------------------------------------------------------------
        GuiDrawRectangle((Rectangle){400, 20, 360, 32}, 2, (Color){Color1, Color2, Color3, Color3},(Color){Color1, Color2, Color3, 150});    // binary button box
        GuiDrawRectangle((Rectangle){400, 404, 360, 32}, 2, (Color){Color1, Color2, Color3, Color3},(Color){Color1, Color2, Color3, 150});   // hexadecimal button box
        GuiDrawRectangle((Rectangle){24, 20, 360, 32}, 2, (Color){Color1, Color2, Color3, Color3},(Color){Color1, Color2, Color3, 150});      // open file box
        
        // Text box groups  
        //---------------------------------------------------------------------------------------
        GuiGroupBox((Rectangle){24, 64, 360, 704 }, "ASSEMBLY CODE");
        GuiGroupBox((Rectangle){400, 64, 360, 328 }, "BINARY");
        GuiGroupBox((Rectangle){400, 448, 360, 320 }, "HEXADECIMAL");

        // draw buttons 
        //---------------------------------------------------------------------------------------
        GuiButton(openFileButton.bounds, openFileButton.text);
        GuiButton(convertToBinaryButton.bounds, convertToBinaryButton.text);
        GuiButton(convertToHexButton.bounds, convertToHexButton.text);
        GuiButton(saveBinaryButton.bounds, saveBinaryButton.text);
        GuiButton(copyBinaryButton.bounds, copyBinaryButton.text);
        GuiButton(saveHexButton.bounds, saveHexButton.text);
        GuiButton(copyHexButton.bounds, copyHexButton.text);

        // If the Gui was disabled by a file window 
        //---------------------------------------------------------------------------------------
        GuiEnable();

        // button effects 
        //---------------------------------------------------------------------------------------
        if (convertToBinaryButton.clicked) {
            if (lineCount > 0 &&  binaryFileData != NULL) {
                textScrollWindow((Rectangle){400, 64, 360, 328}, processedBinaryFileData, lineCount, &scrollDistanceBin, &binaryScrollActive);
            } else {
                GuiDrawText("Open a file first.", (Rectangle) {524, 200, 200, 24}, 0, RED);
            }
        }
        if (convertToHexButton.clicked) {
            if (lineCount > 0 && hexFileData != NULL && binaryLoaded) {
                textScrollWindow((Rectangle){400, 448, 360, 320}, processedHexFileData, lineCount, &scrollDistanceHex, &hexScrollActive);
            } else if (!binaryLoaded && lineCount == 0) {
                GuiDrawText("Open a file first.", (Rectangle) {524, 584, 200, 24}, 0, RED);
            }
            } else if (!binaryLoaded && lineCount > 0){
                GuiDrawText("Convert to binary first.", (Rectangle) {500, 584, 200, 24}, 0, RED);
        }
        if (fileDialogState.SelectFilePressed) {
            if (lineCount != 0){
                textScrollWindow((Rectangle){24, 64, 360, 704}, processedFileData, lineCount, &scrollDistanceASM, &assemblyScrollActive);
            }
        }
        if (openFileButton.clicked) {
            // Reset file data
            if (lineCount > 0) {
                lineCount = 0;
                char **fileData = NULL;
                char *processedFileData = NULL;
                char **binaryFileData = NULL;
                char *processedBinaryFileData = NULL;
                char **hexFileData = NULL;
                char *processedHexFileData = NULL;
            }
            guiButtonsActive = false;
            openFileWindow();
        }
        if (saveBinaryButton.clicked) {
            guiButtonsActive = false;
            saveFileWindow('b');
        }
        if (saveHexButton.clicked) {
            guiButtonsActive = false;
            saveFileWindow('h');
        }
        if (copyBinaryButton.clicked) {
            SetClipboardText(processedBinaryFileData);
            copyBinaryButton.clicked = false;
        }
        if (copyHexButton.clicked) {
            SetClipboardText(processedHexFileData);
            copyHexButton.clicked = false;
        }
    }

//-------------------------------------------------------------------------------------------------------------------//
// Background Functions                                                                                              //
//-------------------------------------------------------------------------------------------------------------------//

void freeMemory(void){
    free(fileData);
    free(processedFileData);
    free(binaryFileData);
    free(hexFileData);
    free(lineNumbers);
}

char* createNumberedString(int start, int end) {

    // Calculate the size needed for the string 
    //---------------------------------------------------------------------------------------
    int bufferSize = 0;
    for (int i = start; i <= end; i++) {
        bufferSize += snprintf(NULL, 0, "%d.\n", i);
        if (i < end) {
            bufferSize += 1; // For the newline character between numbers
        }
    } 
    
    // Allocate memory for the string 
    //---------------------------------------------------------------------------------------
    char* result = (char*)malloc((bufferSize + 1) * sizeof(char)); // +1 for null terminator
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    // Create the numbered string without trailing newline 
    //---------------------------------------------------------------------------------------
    char* ptr = result;
    for (int i = start; i <= end; i++) {
        ptr += sprintf(ptr, "%d.", i);
        if (i < end) {
            *(ptr++) = '\n'; // Add newline except after the last number
        }
    }
    *ptr = '\0';
    return result;
}

char *combineStrings(char **strings, int count) {

    // Calculate the total length required
    //---------------------------------------------------------------------------------------
    int total_length = 0;
    for (int i = 0; i < count; i++) {
        if (strings[i] == NULL) {
            fprintf(stderr, "Null pointer encountered at index %d\n", i);
            exit(EXIT_FAILURE);
        }
        total_length += strlen(strings[i]);
    }
    // Add space for separators (spaces in this case) and the null terminator
    //---------------------------------------------------------------------------------------
    total_length += (count - 1) + 1;

    // Allocate memory for the combined string
    //---------------------------------------------------------------------------------------
    char *combined_string = malloc(total_length * sizeof(char));
    if (combined_string == NULL) {
        perror("Unable to allocate memory");
        exit(EXIT_FAILURE);
    }

    // Initialize the combined string
    //---------------------------------------------------------------------------------------
    combined_string[0] = '\0';

    // Use a pointer to keep track of where to append the next string
    //---------------------------------------------------------------------------------------
    char *current_position = combined_string;

    // Concatenate all strings into the combined string
    //---------------------------------------------------------------------------------------
    for (int i = 0; i < count; i++) {
        strcpy(current_position, strings[i]);
        current_position += strlen(strings[i]);
        if (i < count - 1) {
            *current_position = '\n';
            current_position++;
        }
    }

    // Null terminate the final string
    //---------------------------------------------------------------------------------------
    *current_position = '\0';

    return combined_string;
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

char *newFileNameGenerator(char *tempName, char type) {

    // depending on data type chooses which string to append
    //---------------------------------------------------------------------------------------
    char *toAppend;
    if (type == 'b') {
        toAppend = "_assembled_binary";
    } else if (type == 'h') {
        toAppend = "_assembled_hex";
    }
    int newLength = strlen(tempName) + strlen(toAppend) + 1;

    char *newString = malloc(newLength * sizeof(char));
    if (!newString) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(3);
    }

    strcpy(newString, tempName);

    // Insert "_assembled" before ".txt"
    //---------------------------------------------------------------------------------------
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
    
    // Initializes variables & memory
    //---------------------------------------------------------------------------------------
    *lineCount = 0;
    int arraySize = 10;
    char buffer[32];
    
    char **instructionLines = malloc(arraySize * sizeof(char*));
    if (!instructionLines) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(4);        
    }

    // Main file read loop
    //---------------------------------------------------------------------------------------
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

        // Adds the buffer to the instructionLines
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

char **assembleLines(char **arrayOfLines, int lineCount) {
    
    // Initializes variables & memory
    //---------------------------------------------------------------------------------------
    int arraySize = 10;
    char buffer[32];

    char **binaryTempFileData = malloc(arraySize * sizeof(char*));
    if (!binaryTempFileData) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(4);        
    }

    for (int i = 0; i < lineCount; i++) {

        // increases the size of heap memory for the array 
        //---------------------------------------------------------------------------------------
        if (i >= arraySize) {
            arraySize *= 2;
            char **temp = realloc(binaryTempFileData, arraySize * sizeof(char*));
            if (!temp) {
                fprintf(stderr, "Memory reallocation failed\n");
                exit(4);
            }
            binaryTempFileData = temp;
        }

        // splits the instruction string into individual parts
        //---------------------------------------------------------------------------------------
        char **tempInstruction = rearrangeString(arrayOfLines[i]);

        // Allocate memory for each line in binaryTempFileData
        //---------------------------------------------------------------------------------------
        binaryTempFileData[i] = malloc(33 * sizeof(char));
        if (!binaryTempFileData[i]) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(4);
        }
        // checks the format type of the instruction
        //---------------------------------------------------------------------------------------
        int format = checkFormat(tempInstruction[0]); 

        // Calls the right assembler function depending on format of the instruction
        //---------------------------------------------------------------------------------------
        switch (format) {
            case 0:
                strncpy(binaryTempFileData[i], AssemblerTypeR(tempInstruction), 33);
                break;
            case 1:
                strncpy(binaryTempFileData[i], AssemblerTypeS(tempInstruction), 33);
                break;
            case 2:
                strncpy(binaryTempFileData[i], AssemblerTypeI(tempInstruction), 33);
                break;
            case 3:
                strncpy(binaryTempFileData[i], AssemblerTypeSB(tempInstruction), 33);
                break;
            case 4:
                strncpy(binaryTempFileData[i], AssemblerTypeU(tempInstruction), 33);
                break;
            case 5:
                strncpy(binaryTempFileData[i], AssemblerTypeUJ(tempInstruction), 33);
                break;
            default:
                exit(5);

        }
        free(tempInstruction);
    }
    return (binaryTempFileData);
}

char **rearrangeString(const char *string) {

    // Initializes variables & memory
    //---------------------------------------------------------------------------------------
    char **dissectedString = malloc(4 * sizeof(char*));
    char *stringCopy = strdup(string);
    int i = 0;

    // Splits the string into a maximum of 4 tokens
    //---------------------------------------------------------------------------------------
    char *token = strtok(stringCopy, " ,()xX");
    while (token != NULL) {
        dissectedString[i] = strdup(token);
        if (dissectedString[i] == NULL) {
            perror("Failed to remove tokens");
            for (int j = 0; j < i; j++) {
                free(dissectedString[j]);
            }
            free(dissectedString);
            free(stringCopy);
            exit(6);
        }
        i++;
        token = strtok(NULL, " ,()xX");
    }
    free(stringCopy);

    // Make sure the array is null-terminated if the instruction is shorter than 4 tokens
    //---------------------------------------------------------------------------------------
    if (i < 3) {
        dissectedString[i] = '\0';
    }

    //return splitted string
    return dissectedString;
}

int checkFormat(const char *instruction) { 

    // Loops through all formats and compare the instruction to a look up map of instructions
    //---------------------------------------------------------------------------------------
    for (int i = 0; i < FORMATLEN; i++) {
        for (int j = 0; j < formatSizes[i]; j++) {
            if (pFormats[i][j] == NULL) {
                fprintf(stderr, "Error: pFormats[%d][%d] is NULL\n", i, j);
                continue;}
            if (strcmp(instruction, pFormats[i][j]) == 0) { 
                return formatNumber[i];
            } 
        }
    }
}

char *intToBinaryStr(char *intStr, int bitLen, char *buffer) {

    // Initializes variables
    //---------------------------------------------------------------------------------------
    int binaryNum[32] = {0};
    int i = 0, negative = 0, result = 0;

    // Checks if the number is negative
    //---------------------------------------------------------------------------------------
    if (strchr(intStr, '-')) {
        negative = 1;
        i++;
    }

    // Turns string into int
    //---------------------------------------------------------------------------------------
    for (; intStr[i] != '\0'; i++) {
        result = result * 10 + (intStr[i] - '0');
    }

    // Convert number to binary representation
    //---------------------------------------------------------------------------------------
    i = 0; // Reset i to reuse for binary conversion
    while (result > 0 && i < 32) {
        binaryNum[31 - i] = result % 2;
        result /= 2;
        i++;
    }

    // If the number was negative, take two's complement
    //---------------------------------------------------------------------------------------
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
    //---------------------------------------------------------------------------------------
    for (i = 0; i < bitLen; i++) {
        buffer[i] = (char)(binaryNum[32 - bitLen + i] + '0');
    }

    // Null terminate string
    //---------------------------------------------------------------------------------------
    buffer[bitLen] = '\0';
    
    return buffer;
}

char **binToHexConvert(char **binaryFileData,  int lines){

    // look up tables 
    //---------------------------------------------------------------------------------------
    char *BinaryTable[] = {
        "0000", "0001", "0010", "0011",
        "0100", "0101", "0110", "0111",
        "1000", "1001", "1010", "1011",
        "1100", "1101", "1110", "1111"};
    char HexTable[] = {
        '0', '1', '2', '3',
        '4', '5', '6', '7', 
        '8', '9', 'A', 'B', 
        'C', 'D', 'E', 'F'};

    // memory variables 
    //---------------------------------------------------------------------------------------
    int arraySize = 10;
    char buffer[9];
    char binBuffer[5];

    // allocate memory to hexData 
    //---------------------------------------------------------------------------------------
    char **hexData = malloc(arraySize * sizeof(char*));
    if (!hexData) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(4);        
    }

    // Main loop 
    //---------------------------------------------------------------------------------------
    for (int i = 0; i < lines; i++) {
        if (i >= arraySize) {
            arraySize *= 2;
            char **temp = realloc(hexData, arraySize * sizeof(char*));
            if (!temp) {
                fprintf(stderr, "Memory reallocation failed\n");
                exit(4);
            }
            hexData = temp;
        }

        // Allocate memory for each line in hexData 
        //---------------------------------------------------------------------------------------
        hexData[i] = malloc(9 * sizeof(char));
        if (!hexData[i]) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(4);
        }
        // Turn binary data into hex data 
        //---------------------------------------------------------------------------------------
        for (int j = 0; j < 8; j++) {
            for (int k = 0; k < 4; k++) {
                binBuffer[k] = binaryFileData[i][j*4+k];
            }
            binBuffer[4] = '\0';
            for (int x = 0; x < 16; x++){
                if (strcmp(binBuffer, BinaryTable[x]) == 0) {
                    buffer[j] = HexTable[x];
                }
            }
        }

        // Copy the buffer into the hexData array & null terminate string
        //---------------------------------------------------------------------------------------
        buffer[8]  = '\0';
        strcpy(hexData[i], buffer);
    }
    return hexData;
}

char* AssemblerTypeR(char **strings) {

    // Initialize variables & memory
    //---------------------------------------------------------------------------------------
    char *assembledInstruction = malloc(33 * sizeof(char));
    if (assembledInstruction == NULL) {
        return NULL;
    }
    memset(assembledInstruction, 0, 33);

    char rs2[6] = {0};
    char rs1[6] = {0};
    char rd[6] = {0};

    // Turn integers into binary strings
    //---------------------------------------------------------------------------------------
    intToBinaryStr(strings[3], 5, rs2);
    intToBinaryStr(strings[2], 5, rs1);
    intToBinaryStr(strings[1], 5, rd);

    // Find correct codes in formats and insert correct values in the final string
    //---------------------------------------------------------------------------------------
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

    // Null terminate string
    //---------------------------------------------------------------------------------------
    assembledInstruction[32] = '\0';

    return(assembledInstruction);
}

char* AssemblerTypeS(char **strings) {

    // Initialize variables & memory
    //---------------------------------------------------------------------------------------
    char *assembledInstruction = malloc(33 * sizeof(char));
    if (assembledInstruction == NULL) {
        return NULL;
    }
    
    memset(assembledInstruction, 0, 33);

    char imm[13] = {0};
    char imm0_4[6] = {0};
    char imm5_11[8] = {0};
    char rs2[6] = {0};
    char rs1[6] = {0};

    // Turn integers into binary strings
    //---------------------------------------------------------------------------------------
    intToBinaryStr(strings[1], 5, rs2);
    intToBinaryStr(strings[3], 5, rs1);
    intToBinaryStr(strings[2], 12, imm);

    // Put code that puts the correct part of the strings in each respective place
    //---------------------------------------------------------------------------------------
    strncpy(imm5_11, imm, 7);
    imm5_11[7] = '\0';
    strncpy(imm0_4, imm + 7, 5);
    imm0_4[5] = '\0';

    // Find correct codes in formats and insert correct values in the final string
    //---------------------------------------------------------------------------------------
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

    // Null terminate string
    //---------------------------------------------------------------------------------------
    assembledInstruction[32] = '\0';
    
    return(assembledInstruction);
}

char* AssemblerTypeI(char **strings) {

    // Initialize variables & memory
    //---------------------------------------------------------------------------------------
    char *assembledInstruction = malloc(33 * sizeof(char));
    if (assembledInstruction == NULL) {
        return NULL;
    }
    memset(assembledInstruction, 0, 33);
    
    char imm[13] = {0};
    char rs1[6] = {0};
    char rd[6] = {0};


    // check what type of I instruction it is
    //---------------------------------------------------------------------------------------
    char *instruction[] = {"LD", "LW", "LH", "LB", "LWU", "LHU", "LBU", "JALR"};
    bool found = false;
    for (int i = 0; i < 8; i++) {
        if (strcmp(strings[0], instruction[i]) == 0) {
            found = true;
        }
    }

    // Turn integer into binary string
    //---------------------------------------------------------------------------------------
    if (found) {
        intToBinaryStr(strings[2], 12, imm);
        intToBinaryStr(strings[3], 5, rs1);
    } else {
        intToBinaryStr(strings[3], 12, imm);
        intToBinaryStr(strings[2], 5, rs1);
    }

    intToBinaryStr(strings[1], 5, rd);

    // Find correct codes in formats and insert correct values in the final string
    //---------------------------------------------------------------------------------------
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

    // Null terminate string
    //---------------------------------------------------------------------------------------
    assembledInstruction[32] = '\0';

    return(assembledInstruction);
}

char* AssemblerTypeSB(char **strings) { 

    // Initialize variables & memory
    //---------------------------------------------------------------------------------------
    char *assembledInstruction = malloc(33 * sizeof(char));
    if (assembledInstruction == NULL) {
        return NULL;
    }
    memset(assembledInstruction, 0, 33);
    
    char imm[14] = {0};         // the immediate of SB types actually have 13 bits, the first bit just does not get included (as you can only jump on at minimum half word lengths)
    char imm1_4[5] = {0};       // so that means that we will take out bit #0 out later, it's implied.
    char imm5_10[7] = {0};
    char imm11[2] = {0};
    char imm12[2] = {0};
    char rs2[6] = {0};
    char rs1[6] = {0};

    // Turns integer into binary string
    //---------------------------------------------------------------------------------------
    intToBinaryStr(strings[2], 5, rs2);
    intToBinaryStr(strings[1], 5, rs1);
    intToBinaryStr(strings[3], 13, imm);

    // Put code that puts the correct part of the strings in each respective place
    //---------------------------------------------------------------------------------------
    strncpy(imm12, imm, 1);
    imm12[1] = '\0';
    strncpy(imm11, imm + 1, 1);
    imm11[1] = '\0';
    strncpy(imm5_10, imm + 2, 6);
    imm5_10[6] = '\0';
    strncpy(imm1_4, imm + 8, 4);
    imm1_4[4] = '\0';

    // Find correct codes in formats and insert correct values in the final string
    //---------------------------------------------------------------------------------------
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

    // Null terminate string
    //---------------------------------------------------------------------------------------
    assembledInstruction[32] = '\0';
    
    return(assembledInstruction);
}

char* AssemblerTypeU(char **strings) { 
    
    // Initialize variables & memory
    //---------------------------------------------------------------------------------------
    char *assembledInstruction = malloc(33 * sizeof(char));
    if (assembledInstruction == NULL) {
        return NULL;
    }
    memset(assembledInstruction, 0, 33);
    
    char imm[21] = {0};
    char rd[6] = {0};

    // Turn integer into binary string
    //---------------------------------------------------------------------------------------
    intToBinaryStr(strings[1], 5, rd);
    intToBinaryStr(strings[2], 20, imm);

    // Find correct codes in formats and insert correct values in the final string 
    //---------------------------------------------------------------------------------------
    for (int i = 0; i < FORMATLEN; i++) {
        for (int j = 0; j < formatSizes[i]; j++) {
            if (strcmp(strings[0], pFormats[i][j]) == 0) { 
                strncpy(assembledInstruction, imm, 20);                     // imm 12
                strncpy(assembledInstruction + 20, rd, 5);                  // rd            
                strncpy(assembledInstruction + 25, UformatOpcode[j], 7);    // Opcode
            }
        }
    }

    // Null terminate string
    //---------------------------------------------------------------------------------------
    assembledInstruction[32] = '\0';

    return(assembledInstruction);
}

char* AssemblerTypeUJ(char **strings) { 

    // Initialize variables & memory
    //---------------------------------------------------------------------------------------
    char *assembledInstruction = malloc(33 * sizeof(char));
    if (assembledInstruction == NULL) {
        return NULL;
    }
    memset(assembledInstruction, 0, 33);
    
    char imm[22] = {0};         
    char imm1_10[11] = {0};
    char imm11[2] = {0};
    char imm12_19[9] = {0};       
    char imm20[2] = {0};
    char rd[6] = {0};

    // Turn integer into binary string
    //---------------------------------------------------------------------------------------
    intToBinaryStr(strings[1], 5, rd);
    intToBinaryStr(strings[2], 21, imm);

    // Put code that puts the correct part of the strings in each respective place 
    //---------------------------------------------------------------------------------------
    strncpy(imm20, imm, 1); // puts imm[0] in imm20
    imm20[1] = '\0';
    strncpy(imm12_19, imm + 1, 8); // puts imm[1] till imm[8] in imm12_19
    imm12_19[9] = '\0';
    strncpy(imm11, imm + 9, 1); // puts imm[9] in imm11
    imm11[1] = '\0';
    strncpy(imm1_10, imm + 10, 10); //puts imm[10] till imm[19] in imm1_10
    imm1_10[10] = '\0';

    // Find correct codes in formats and insert correct values in the final string 
    //---------------------------------------------------------------------------------------
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

    // Null terminate string
    //---------------------------------------------------------------------------------------
    assembledInstruction[32] = '\0';
    
    return(assembledInstruction);
}
