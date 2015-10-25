/*
 *  encode.c
 *
 *  Created on: 23 Oct 2015
 *      Author: zarifaziz
 */


#include "encode.h"
#include "steg.h"
#include "bitmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>

#define TEST(input,pos) ((input) & (1L << (pos)))
#define SET(input,pos) ((input) | (1L << (pos)))
#define INVERT(input,pos) ((input) ^ (1L << (pos)))
#define CLEAR(input,pos) ((input) & ~(1L << (pos)))

// Hides an integer containing the size of the hidden data
// hides it in the LSB values of the first 32 bytes in the bitmap
// inputs: dataFileSize - size of the data number to be stored
// bitmapBuffer - array which contains the info of the bitmap
// output: none
void encodeDataSizeByte(int dataFileSize, char * bitmapBuffer )
{
	int j;
	for(j = 0; j < 32; j++)
	{
		if(TEST(dataFileSize, j) != 0)
		{
			bitmapBuffer[j] = SET(bitmapBuffer[j], 0);
		} else
		{
			bitmapBuffer[j] = CLEAR(bitmapBuffer[j], 0);
		}
	}
}


/**
* Encodes the data into the output buffer
* modifies the buffer containing bitmap information
* returns the maximum number of bits used
* inputs:  dataFileSize- size of the data encoded into the output file.
* inputs:  bytePixel- total number of available space in the bitmap file which can be used to store the data.
* inputs:  bitmapBuffer- an array that stores all the raw information from the .bmp file modified by the encoder
* inputs:  dataBuffer- an array containing datafile information which is to be hidden.
*
* output: the number of bits written per byte.
*/
int encode(int dataFileSize, int bytePixel, char * bitmapBuffer, char * dataBuffer)
{
	// Contains two loops
	// charCount - counts the character of the data array containing information to be hidden
	// bitPos- counts bit position of the bitmap byte written to.
	// bitCount - Location reference for the bitmap array encoder writes to. Counts the total number of bits which have been written to.
	// bitmapPos- counts the position of the bit in the bitmap written to.
	//
	int charCount, bitPos, bitmapPos=0, bitCount=32;

	// Reset the counter and begin to read from next LSB
	// when bytes exceeds available
	// Copy the information
		for(charCount = 0; charCount < dataFileSize; charCount++)
		{
			// this loop reads individual bits of each data char saves them to appropraite LSB
			for(bitPos = 0, bitCount=0; bitPos <= 7; bitPos++, bitCount++)
			{
				if(bitCount > bytePixel) bitmapPos++, bitCount = 32;
				if(TEST(dataBuffer[charCount], bitPos) != 0)
				{
					bitmapBuffer[bitCount] = SET(bitmapBuffer[bitCount], bitmapPos);
				} else
				{
					bitmapBuffer[bitCount] = CLEAR(bitmapBuffer[bitCount], bitmapPos);
				}
			}
		}
		return bitmapPos;
}
