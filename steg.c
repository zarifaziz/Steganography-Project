/*
 * steg.c
 *
 *  Created on: 12 Oct 2015
 *      Author: zarifaziz
 */

#include "bitmap.h"
#include "steg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// build command
// gcc -Werror --std=c99 *.c -lm

// run command
// ./a.out


// This is my main file
int main(int argc, char *argv[])
{
//  #if 0
  switch (argc)
	{
		case 3:
			decode(argv[1], argv[2]);
			break;
		case 4:
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

  printf("Running\n");

	return 0;
}
