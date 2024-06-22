#include <stdio.h>
#include <string.h>


int main (void) {

    //List of formats
    const char Rformat[] = {'ADD', 'SUB', 'SLT', 'SLTU', 'AND', 'OR', 'XOR', 'SLL', 'SRL', 'SRA'};
    const char Sformat[] = {'SD', 'SW', 'SH', 'SB'};
    const char Iformat[] = {'ADDI', 'SLTI', 'SLTIU', 'ANDI','ORI','SLLI','SRLI','SRAI','LD','LW','LH','LB','LWU','LHU','LBU','JALR',};
    const char SBformat[] = {'BEQ', 'BNE', 'BGE', 'BGEU', 'BLT', 'BLTU'};
    const char Uformat[] = {'LUI', 'AUIP'};
    const char UJformat[] = {'JAL'};

    const char *Formats[] = {Rformat, Sformat, Iformat, SBformat, Uformat, UJformat};

    //here comes the loop which goes through each instruction in the file and assembles them
    const char *instruction = ' ';

    return 0;
}

//opens the assembly instructions
void openFile(void) {
    char file[150];

    printf("What file would you like assembled? Provide the name with extension or the file path as such:\n /home/Documents/example.txt\n");
    if (fgets(file, sizeof(file), stdin) != NULL) {
        // Remove the newline character at the end if present
        size_t len = strlen(file);
        if (len > 0 && file[len-1] == '\n') {
            file[len-1] = '\0';
        }
        printf("Assembling: %s\n", file);
    } else {
        printf("Error reading input.\n");
    }

    FILE *ASMfile = fopen(file, "r");
    if (ASMfile == NULL) {
        printf("Could not open file.\n");
        return 1; // Exit if file cannot be opened
    }

}

//reads through each line in the file
void analyzeFile(ASMfile) {
    // Buffer to hold each line
    char buffer[256];

    // Read and print each line from the file
    int lineNumber = 1;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        // Print the line
        printf("Line %d: %s", lineNumber, buffer);
        lineNumber++;
    }

    // Close the file
    fclose(file);
}

// function to get number of items in array
int arraySize(array) {
    return sizeof(array) / sizeof(array[0]); // Get the size of the array
}    

//checks the format of the instruction
char checkFormat (instruction) {

    for (int i = 0; arraySize(Formats[i]); i++) {

        for (int j = 0; j < arraySize(Formats[i]); j++) {

            if (instruction == Formats[i][j]) {

                return Formats[i];
            }
        }
    }
}

