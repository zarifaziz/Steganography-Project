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
#include <ctype.h>



int checkfile(const char * outputfile)
{
  FILE *fp;
  if (fp == fopen(outputfile, "rb"))
  {
    fclose(fp);
    return 1;
  }
  return 0;
}

int overwritePrompt(const char * outputfile)
{
  char str[SIZE];
  printf("Output file %s already exists. Overwrite (y/n)?\n", outputfile);
	fgets(str, 3, stdin);

  //Checking that 'y' was entered followed by enter key
  //Otherwise, exit the program
	if (str[0] != 'y' || str[1] != '\n')
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
		if (!overwritePrompt(outputfile))
		{
      // breaks out of the function ??
			return 1;
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
      int c = fgetc(fbmp);
  }

  // I BETTER SEE YOU
  return 0;
}
