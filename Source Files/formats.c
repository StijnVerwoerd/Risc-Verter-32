#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "formats.h"


// R-type format
const char *Rformat[] = {"ADD", "SUB", "SLT", "SLTU", "AND", "OR", "XOR", "SLL", "SRL", "SRA"};
const char *RformatOpcode[] = {"0110011", "0110011", "0110011", "0110011", "0110011", "0110011", "0110011", "0110011", "0110011", "0110011"};
const char *RformatFunct3[] = {"000", "000", "010", "011", "111", "110", "100", "001", "101", "101"};
const char *RformatFunct7[] = {"0000000", "0100000", "0000000", "0000000", "0000000", "0000000", "0000000", "0000000", "0000000", "0100000"};


// S-type format
const char *Sformat[] = {"SW", "SH", "SB"};
const char *SformatOpcode[] = {"0100011", "0100011", "0100011"};
const char *SformatFunct3[] = {"010", "001", "000"};


// I-type format
const char *Iformat[] = {"ADDI", "SLTI", "SLTIU", "ANDI", "ORI", "SLLI", "SRLI", "SRAI", "LW", "LH", "LB", "LHU", "LBU", "JALR"};
const char *IformatOpcode[] = {"0010011", "0010011", "0010011", "0010011", "0010011", "0010011", "0010011", "0010011", "0000011", "0000011", "0000011", "0000011", "0000011"};
const char *IformatFunct3[] = {"000", "010", "011", "111", "110", "001", "101", "101", "010", "001", "000", "101", "100", "000"};


// SB-type format
const char *SBformat[] = {"BEQ", "BNE", "BGE", "BGEU", "BLT", "BLTU"};
const char *SBformatOpcode[] = {"1100011", "1100011", "1100011", "1100011", "1100011", "1100011"};
const char *SBformatFunct3[] = {"000", "001", "101", "111", "100", "110"};


// U-type format
const char *Uformat[] = {"LUI", "AUIPC"};
const char *UformatOpcode[] = {"0110111", "0010111"};


// UJ-type format
const char *UJformat[] = {"JAL"};
const char *UJformatOpcode[] = {"1101111"};

// List of formats
const int formatNumber[6] = {0, 1, 2, 3, 4, 5};
const char *Formats[] = {"R", "S", "I", "SB", "U", "UJ"};
const char **pFormats[] = {Rformat, Sformat, Iformat, SBformat, Uformat, UJformat};
const size_t formatSizes[] = {10, 3, 14, 6, 2, 1};
