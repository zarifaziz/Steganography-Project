/*
 * decode.c
 *
 *  Created on: 19 Oct 2015
 *      Author: zarifaziz
 */

#include "steg.h"
#include "bitmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>




int decode(const char *bmpfile, const char *outputfile)
{
  FILE *fbmp = fopen(bmpfile, "rb");
  FILE *fout = fopen(outputfile, "wb");
  BmpData bdat;

  // get the header and pixel data information
  bdat = check_bitmap(fbmp);

  // Skipping over the header block
  fseek(fbmp, bdat.headersize, SEEK_SET);

  // tells you the size of your message in bytes
  int bytSize = 0;
  int writeStart = 32;
      // Trying to pull out the first 32 bytes for the size of data
      // display 1 or 0 based on the modulus of the byte/2
      // c is in decimal system, it's an int
      // MAYBE CHANGE 32 TO WRITESTART
      for (int bytPos = 0; bytPos <= 31; bytPos++) {

          // getting next bytes
          int c = fgetc(fbmp);
          // throw an error if EOF is reached
          assert(c != EOF);

          // need new system for multiple bits
          // what's c divided by 0 ?
          // if modulus is not = 0, the LSB is 1
          if(c % 2 != 0) {
            // the current LSB is 1, so store that
            bytSize = bytSize + 1;
          }
          // Store LSB to second LSB
          // do not perform left shift on the last bit
          if (bytPos != 31) {
            // introduce new zero to the LSB
            // shifts the 1 or 0 to the left
            bytSize = bytSize << 1;
          }
      }

    // converting from bytes to bits
  int bitSize = bytSize * 8;

  /*
  * Need to do a calculation to figure out how many times
  * we are going to need to loop through if at all
  */

// FOR LOOPING
  // going to take the number of the bytes of the image minus 32
  // number of bits divided by that value
  // what we get is the number of bits per byte that we have to manipulate
  // if that number is greater than 8, then we get an error


  // Here is where we loop through the image and pull out all the bits
  // Computing how many times we need to loop
  int loops = floor((bitSize-1) / (bdat.numpixelbytes - writeStart)) + 1;
  // need more than 8 bits per byte
  if (loops > 8) {
    printf("Error: Expected data size is larger than available space in bitmap.\n");
    // terminate the program
    exit(0);
  }

  // Current message byte we are pulling out
  int current_byte = 0;

  // byte position
  int pos = 1;
      // Now for the rest of the data until the number given above
      // we need to go through and pull out the LSB
      for (int j = 1; j <= bitSize; j++) {

        // getting next bytes
        int c = fgetc(fbmp);
        // throw an error if EOF is reached
        assert(c != EOF);

        // perform left shift by n-1
        c = c >> (pos-1);

        // This is where I would implement stuff to move around
        // if we need to go through data more than once

        // need new system for multiple bits
        if(c % 2 != 0) {
          // the current LSB is 1, so store that
          current_byte = current_byte + 1;
        }

        // every time we go through 8 bytes of the image to get 8 bits
        // which means we have one byte of the message, we push that
        // byte out to the file
        // reset the current to 0 and go through the next 8
        if (j % 8 == 0) {
          fputc(current_byte, fout); // every 8 bits we need to push to file
          current_byte = 0; // and reset the byte
        }
        // But otherwise we just perform the bit ops and keep going
        else
        {
        current_byte = current_byte << 1;
        }
        // As long as file is still being written
        if ((j/pos)+writeStart == bdat.numpixelbytes && loops > 1)
        {
          // offset to writeable bytes
          fseek(fbmp, bdat.headersize+writeStart, SEEK_SET);
          // do this for the number of loops
          loops = loops - 1;
          // next bit
          pos = pos + 1;
        }
      }

  printf("Running after end of decode\n");
  return 0;
}

// TIPS FOR ENCODING
// first figure out the binary

//
