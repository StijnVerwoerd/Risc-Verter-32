#ifndef FORMATS_H
#define FORMATS_H

#include <stddef.h>

#define FORMATLEN 6

// List of formats
extern const int formatNumber[6];
extern const char *Formats[];
extern const char **pFormats[]; // format pointers
extern const size_t formatSizes[]; //size of arrays

// R-type format
extern const char *Rformat[];
extern const char *RformatOpcode[];
extern const char *RformatFunct3[];
extern const char *RformatFunct7[];


// S-type format
extern const char *Sformat[];
extern const char *SformatOpcode[];
extern const char *SformatFunct3[];


// I-type format
extern const char *Iformat[];
extern const char *IformatOpcode[];
extern const char *IformatFunct3[];


// SB-type format
extern const char *SBformat[];
extern const char *SBformatOpcode[];
extern const char *SBformatFunct3[];


// U-type format
extern const char *Uformat[];
extern const char *UformatOpcode[];


// UJ-type format
extern const char *UJformat[];
extern const char *UJformatOpcode[];


#endif // FORMATS_H