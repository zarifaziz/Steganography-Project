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

// representing a bitMask of 1's at different positions in binary
const unsigned char bitMask[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

/*
 * encodes each bit
 */
void encodeBit(unsigned int bitnumber, unsigned char binary)
{
	if (binary == 0)
		dataPtr[bufCount] &= ~bitMask[bitnumber] ;
	else
		dataPtr[bufCount] |= bitMask[bitnumber] ;
}
/*
 *  takes one byte and encodes it by passing it to another function
 */
void encodeByte(unsigned long dataSize)
{
	int i;
	for (i=7;i>-1;i--)
	{
		encodeBit(0, dataSize&bitMask[i]);
		bufCount++;
	}
}

// Increases the dataPtr position every time
void stepUpPtr()
{
	bufCount++;
	if (bufCount >= bdat.numpixelbytes)
	{
		bufCount = 32;
		bufferbitPtr++;
	}
}

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

// Prompts the user to overwrite the output file
// input: pointer to the outputfile
// output: returns an int to signal whether wants to overwrite or not
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

        bdat = check_bitmap(fbmp);

        int i;
        int c = 0;

        for (i=0;i<bdat.headersize;i++)
        { /* Copy header verbatim */
           c = fgetc(fbmp);
         fputc(c, fout);
        }

        // store bytes from the data file
        dataPtr = (unsigned char *)malloc(bdat.numpixelbytes * sizeof(unsigned char));

        //store bitmap in array
        fread(dataPtr, 1, bdat.numpixelbytes, fbmp);

        //check size of data
        fseek(fdata, 0L, SEEK_END);
        dataSize = ftell(fdata);
        rewind(fdata);

        loops = ( (8*dataSize)/(bdat.numpixelbytes -32) )+1;//
        if (loops>8)
        {
          printf("Error: Bitmap too small to store data file.\n");
          return(0);
        }
        // Modifying the bits to store the data file
        int modBits = ((8*dataSize + bdat.numpixelbytes - 1) / bdat.numpixelbytes);
        // Displaying maximum number of modified bits
        printf("There was a maximum of %d bits modified per byte.\n", modBits);

        // Clearing output dataPtr
        fflush(stdout);

        bufCount=0;	bufferbitPtr =0;

         //encode 4 bytes data size
        encodeByte(dataSize>>24);
        // encodes next
        encodeByte(dataSize>>16);
        // next
        encodeByte(dataSize>>8);
        encodeByte(dataSize);

        int z =0;
        // Loop till EOF is reached
        while ( (z=fgetc(fdata)) != EOF )
        {
          for(i=7;i>-1;i--)
          {
            // bitMask it
            encodeBit(bufferbitPtr, z&bitMask[i]);
            // as it loops through it masks the bitMask[]
            // increases the position of the pointer
              stepUpPtr();
          }
        }

        // dataPtr to fout
        fwrite(dataPtr, 1, bdat.numpixelbytes, fout);

		  break;

    // If program is run with wrong arguments, print following message and terminate
		default:
			printf("Usage: (encode or decode, respectively)\n"
				"\tsteg <bmpfile> <datafile> <outputfile>\n"
				"\tsteg <bmpfile> <outputfile>\n");
			break;
	}
	return 0;
}
