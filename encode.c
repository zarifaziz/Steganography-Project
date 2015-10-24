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

#define SET(input,pos) ((input) | (1L << (pos)))
#define CLEAR(input,pos) ((input) & ~(1L << (pos)))
#define INVERT(input,pos) ((input) ^ (1L << (pos)))
#define TEST(input,pos) ((input) & (1L << (pos)))

void encodesize(int dataFileSize, char * bitarr )
{
	/**
	* Encodesize hides an integer containing the size of the hidden data.
	* Encodesize sets the LSB values of the first 32 bytes in the bitmap array.
	* @param dataFileSize-size of data number to be stored
	* @param bitarr- array storing the information of the bitmap which will be written to the output bitmap.
	*/
	int s;
	for(s = 0; s < 32; s++)
	{
		if(TEST(dataFileSize, s) != 0)
		{
			bitarr[s] = SET(bitarr[s], 0);
		} else
		{
			bitarr[s] = CLEAR(bitarr[s], 0);
		}
	}
}

int encode(int dataFileSize, int bytePixel, char * bitarr, char * dataarr)
{
	/**
	* Encoder encrypts the data into a specified output bitmap file
	* This is done by modifying the array containing bitmap information and returns the maximum number of bits used.
	*
	* @param dataFileSize- size of the data encoded into the output file.
	* @param bytePixel- total number of available space in the bitmap file which can be used to store the data.
	* @param bitarr- an array that stores all the raw information from the .bmp file modified by the encoder
	* @param dataarr- an array containing datafile information which is to be hidden.
	*
	* @return d- the number of bits written per byte.
	*/

int a, b, d=0, c=32;
	/** Encoder utilizes an inner and outer loop.
	* 		The outer loop iterates the dataFileSize to be hidden
	* 		The inner loop reads the individual bits of each data charater and saves them to the relevant least significant bit in an array.
	*
	* a- counts the character of the data array containing information to be hidden
	* b- counts bit position of the bitmap byte written to.
	* c- Location reference for the bitmap array encoder writes to. Counts the total number of bits which have been written to.
	* d- counts the position of the bit in the bitmap written to.

	* Reset the counter and begin to read from the next least significant bit when the total number of bytes exceeds the number of bytes available
	* Copy the information by checking the value of each bit.
	*/

	for(a = 0; a < dataFileSize; a++)
	{
// c= 0
		for(b = 0, c=0; b <= 7; b++, c++)
		{
			if(c > bytePixel) d++, c = 32;
			if(TEST(dataarr[a], b) != 0)
			{
				bitarr[c] = SET(bitarr[c], d);
			} else
			{
				bitarr[c] = CLEAR(bitarr[c], d);
			}
		}
	}
	return d;
}
