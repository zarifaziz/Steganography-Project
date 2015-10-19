#ifndef BITMAP_DEFS_HEADER
#define BITMAP_DEFS_HEADER

#include <stdio.h> 

typedef struct 
{
    unsigned int headersize;    /* Number of bytes composing the total header block */
    unsigned int numpixelbytes; /* Number of bytes composing the total data block */
} BmpData;

BmpData check_bitmap(FILE *fp);

#endif
