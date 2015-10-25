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
    fclose(fp);
    return 1;
  }
  return 0;
}

int overwritePromptYes(const char * outputfile)
{
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

        // Start decoding !!
  			decode(argv[1], argv[2]);

        fclose(fbmp);
  			fclose(fout);
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

      // Extracting header size and number of pixels available for data storage
      // Copying header to the output file verbatim
      // copying information to memory array

      // get the header and pixel data information
      bdat = check_bitmap(fbmp);

      int i;

      // Header copied the  to the output file
      for(i = 0; i <= bdat.headersize; i++)
			{
				c = fgetc(fbmp);
				fputc(c, fout);
			}

      // Number of bytes that store picture information saved
      numPBytes = bdat.numpixelbytes;

      // Memory allocation  for an array of equal size to the picture pixel array
			bitmapBuffer = (char*)malloc(numPBytes);

      // offset past the header
      fseek(fbmp, bdat.headersize, SEEK_SET);

      // Information copied from the picture pixel array to the allocated memory array
      for(i = 0; i <= numPBytes; i++)
			{
				bitmapBuffer[i] = fgetc(fbmp);
			}

      // Checking whether datafile has been opened correctly
      // Reading length of the data
			if(fdata == NULL)
			{
				printf("Error: Could not open file %s.\n", argv[2]); return 0;
			}

      // Getting size of data
      fseek(fdata, 0L, SEEK_END);
			dataFileSize = ftell(fdata);
			rewind(fdata);

      // ensuring that there is enough space
      if(dataFileSize > numPBytes - 32)
			{
				printf("Error: Bitmap too small to store data file.");
				return 0;
			}

      // Allocating memory for an array equal to the size of the data

			dataBuffer = (char*)malloc(dataFileSize);
			fread(dataBuffer, sizeof(int), dataFileSize, fdata);

      // If all tests pass, start encoding!!
      // encode size of data to first part of output pixels
      encodeDataSizeByte(dataFileSize, bitmapBuffer);
			bitsModified = encode(dataFileSize, numPBytes-32, bitmapBuffer, dataBuffer);

      // Creating offset
      fseek(fout,bdat.headersize,SEEK_SET);
			fwrite(bitmapBuffer,sizeof(char),numPBytes,fout);

      // Closing the files
			fclose(fbmp);
			fclose(fdata);
			fclose(fout);
  		// encode(argv[1], argv[2], argv[3]);

		  break;

    // If program is run with wrong arguments, print following message and terminate
		default:
			printf("Usage: (encode or decode, respectively)\n"
				"\tsteg <bmpfile> <datafile> <outputfile>\n"
				"\tsteg <bmpfile> <outputfile>\n");
			break;

	}
  //  #endif

  // Displaying number of bits modified
  printf("\nThere was a maximum of %d bits modified per byte.\n", bitsModified+1);

	return 0;
}
