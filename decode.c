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

// REMOVE THIS FUNCTION
// int bin_dec(char *size_bin);

// checking if outputfile already exists
int checkfile(const char * outputfile)
{
  FILE *fp;
  if (fp == fopen(outputfile, "rb"))
  {
    // DOES THE OPPOSITE, ASK TUTOR
    fclose(fp);
    return 0;
  }
  printf("detecting duplicate file\n");
  return 1;
}

int overwritePromptYes(const char * outputfile)
{

  printf("Running at the start of overwrite function\n");


  char str[SMALLSIZE];
  printf("Output file %s already exists. Overwrite (y/n)?\n", outputfile);
	fgets(str, 3, stdin);

  //Checking that 'y' was entered followed by enter key
  //Otherwise, exit the program
  // changed from || to &&
	if (str[0] == 'y' && str[1] == '\n')
	{
		return 1;
	}
	return 0;
}

int decode(const char *bmpfile, const char *outputfile)
{
  // Check that the output file exists
  if (checkfile(outputfile))
	{
    // query whether to overwrite, terminating the program
    // if the user doesn’t type ‘y’
		if (!overwritePromptYes(outputfile))
		{
      // breaks out of the function if
      // overwrite prompt is no
      // changing from return 1 to an exit
      printf("About to exit!\n");
			exit(0);
		}
    // else
    // {
    //   // otherwise, terminate the program
    //   break;
    // }
	}

  FILE *fbmp = fopen(bmpfile, "rb");
  FILE *fout = fopen(outputfile, "wb");
  BmpData bdat;

  // Check that the pointers above actually point to something
  if (fbmp == NULL || fout == NULL ) {
      printf("Error: Could not open files.\n"); // Needs to be replaced with proper message?
      return 1;
  }

  // get the header and pixel data information
  bdat = check_bitmap(fbmp);

  while (bdat.headersize--)
  { /* skip over the header */
      //int c = 0 ??
      int c = fgetc(fbmp);
  }

  int size_dec = 0;

      // Trying to pull out the first 32 bytes for the size of data
      // display 1 or 0 based on the modulus of the byte/2
      for (int i = 0; i <= 31; i++) {
          int c = fgetc(fbmp);
          assert(c != EOF);

          // need new system for multiple bits
          // what's c divided by 0 ?
          if(c % 2 != 0) {
            size_dec += 1; // the current LSB is 1 to bring that across
          }
          if (i != 31) {
            size_dec = size_dec << 1; // dont do bitwise on the last one
          }
      }

  int num_bits = size_dec *8;

  /*
  * Need to do a calculation to figure out how many times
  * we are going to need to loop through if at all
  */

  // Here is where we loop through the image and pull out all the bits
  int current_byte = 0; // Current message byte we are pulling out

      // Now for the rest of the data until the number given above
      // we need to go through and pull out the LSB
      for (int i = 1; i<= num_bits; i++) {
        int c = fgetc(fbmp);
        assert(c != EOF); // change this to print a message?

        // This is where I would implement stuff to move around
        // if we need to go through data more than once

        // need new system for multiple bits
        if(c % 2 != 0) {
          current_byte += 1; // the current LSB is 1 to bring that across
        }

        if (i % 8 == 0) {
          fputc(current_byte, fout); // every 8 bits we need to push to file
          current_byte = 0; // and reset the byte
        }
        else { // But otherwise we just perform the bit ops and keep going
        current_byte = current_byte << 1;
        }
      }
  printf("Running after end of decode\n");
  return 0;
}
