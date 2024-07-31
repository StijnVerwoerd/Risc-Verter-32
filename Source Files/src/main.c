#include "functions.h"
#include "../raylib/src/raylib.h"
#include "../raygui/src/raygui.h"

int main (void) {

    // Initialize GUI
    initGui();

    // Main program loop
    while (!WindowShouldClose()) { // exits if window is closed or escape is pressed

        BeginDrawing();
        updateGui();
        drawGui();
        EndDrawing();
    }

    // De-initialize program
    CloseWindow();CloseWindow();
    freeMemory();

    // end succesfully
    return 0;



    //-------------------------------------------------------------------------------------------
    /*
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
    */
}





