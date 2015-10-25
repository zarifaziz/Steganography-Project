/*
* encode.h
* header file
*/

#include <stdio.h>
#include <stdlib.h>

// Declaring functions

void encodeDataSizeByte(int dataSize,char * bitmapBuffer );

int encode( int dataFileSize, int bytepixel, char * bitmapBuffer, char * dataBuffer);
