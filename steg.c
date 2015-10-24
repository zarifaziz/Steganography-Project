/*
 * steg.c
 *
 *  Created on: 12 Oct 2015
 *      Author: zarifaziz
 */

#include "encode.h"
#include "bitmap.h"
#include "steg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// build command
// gcc -Werror --std=c99 *.c -lm -o steg

// run command
// ./steg acfr...

// These functions run on both encode and decode

// Do I keep this here ????
BmpData bdat;

// checking if outputfile already exists
int checkfile(const char * outputfile)
{
  FILE *fp = fopen(outputfile, "rb");
  if (fp != NULL)
  {
    // DOES THE OPPOSITE, ASK TUTOR
    fclose(fp);
    return 1;
  }
  printf("detecting duplicate file\n");
  return 0;
}

int overwritePromptYes(const char * outputfile)
{

  printf("Running at the start of overwrite function\n");
  // initialing array
  char str[SMALLSIZE];
  printf("Output file %s already exists. Overwrite (y/n)?\n", outputfile);
	fgets(str, SMALLSIZE, stdin);

  //Checking that 'y' was entered followed by enter key
  //Otherwise, exit the program
  // changed from || to &&
	if ((str[0] == 'y' || str[0] == 'Y') && str[1] == '\n')
	{
		return 1;
	}
	return 0;
}


// This is my main file
int main(int argc, char *argv[])
{
//  #if 0
  switch (argc)
	{
		case 3:
      // Check that the output file exists
      if (checkfile(argv[2]))
    	{
        // query whether to overwrite, terminating the program
        // if the user doesn’t type ‘y’
    		if (!overwritePromptYes(argv[2]))
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

      FILE *fbmp = fopen(argv[1], "rb");
      FILE *fout = fopen(argv[2], "wb");
      //BmpData bdat;

      if(fbmp == NULL)
    	{
    		printf("Error: Could not open file %s.\n", argv[1]);
        return 0;
    	}
    	if(fout == NULL)
    	{
    		printf("Error: Could not open file %s.\n", argv[2]);
        return 0;
    	}
  			decode(argv[1], argv[2]);
  			break;



		case 4:

      // Check that the output file exists
      if (checkfile(argv[3]))
    	{
        // query whether to overwrite, terminating the program
        // if the user doesn’t type ‘y’
    		if (!overwritePromptYes(argv[3]))
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

      // not redifining variables
      fbmp = fopen(argv[1], "rb");
      fout = fopen(argv[3], "wb");
      FILE *fdata = fopen(argv[2], "rb");
      //BmpData bdat;

      if(fbmp == NULL)
    	{
    		printf("Error: Could not open file %s.\n", argv[1]);
        return 0;
    	}
    	if(fout == NULL)
    	{
    		printf("Error: Could not open file %s.\n", argv[3]);
        return 0;
    	}

      //#if 0

      // Extracting header size and number of pixels available for data storage
      // Copying header to the output file verbatim
      // copying information to memory array

      // get the header and pixel data information
      bdat = check_bitmap(fbmp);

      int i;

      for(i = 0; i <= (int)bdat.headersize; i++)
			{
				c = fgetc(fbmp);
				fputc(c, fout);
			}

      numpixelbytes = (int)bdat.numpixelbytes;
      // Memory allocation  for an array of equal size to the picture pixel array
			ptrbitmap = (char*)malloc(numpixelbytes);

      fseek(fbmp, bdat.headersize, SEEK_SET);

      for(i = 0; i <= numpixelbytes; i++)
			{
				ptrbitmap[i] = fgetc(fbmp);
			}

      // Checking whether datafile has been opened correctly
      // Reading length of the data
			if(fdata == NULL)
			{
				printf("Error: Could not open file %s.\n", argv[2]); return 0;
			}

      fseek(fdata, 0L, SEEK_END);
			dataSize = ftell(fdata);
			rewind(fdata);

      if(dataSize > numpixelbytes-32)
			{
				printf("Error: Bitmap too small to store data file.");
				return 0;
			}

			dataptr = (char*)malloc(dataSize);
			fread(dataptr, sizeof(char), dataSize, fdata);

      // If all tests pass, start encoding!!
      // encode size of data to first part of output pixels

      // Declaration functions
      encodesize(dataSize, ptrbitmap);
			modbits = encode(dataSize, numpixelbytes-32, ptrbitmap, dataptr);

      fseek(fout,bdat.headersize,SEEK_SET);
			fwrite(ptrbitmap,sizeof(char),numpixelbytes,fout);

      // Closing the files
			fclose(fbmp);
			fclose(fdata);
			fclose(fout);
  		// encode(argv[1], argv[2], argv[3]);

      // #endif

		  break;

    // If program is run with wrong arguments, print following message and terminate
		default:
			printf("Usage: (encode or decode, respectively)\n"
				"\tsteg <bmpfile> <datafile> <outputfile>\n"
				"\tsteg <bmpfile> <outputfile>\n");
			break;

	}
  //  #endif

  printf("Running after switch statement\n");

	return 0;
}
