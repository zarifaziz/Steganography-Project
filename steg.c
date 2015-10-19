/*
 * steg.c
 *
 *  Created on: 12 Oct 2015
 *      Author: zarifaziz
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bitmap.h"
//#include "steg.h"

// build command
// gcc -Werror --std=c99 *.c -lm

//Declaring function
int test(void);

// This is my main file
int main(int argc, char *argv[])
{
  #if 0
  switch (argc)
	{
		case 3:
			decode(argv[1], argv[2]);
			break;

		case 4:
			encode(argv[1], argv[2], argv[3]);
			break;
		case 1:
		{
			userInterface();
			break;
		}
		default:
			printf("Usage: (encode or decode, respectively)\n"
				"\tsteg <bmpfile> <datafile> <outputfile>\n"
				"\tsteg <bmpfile> <outputfile>\n\n"
				"OR type \"open\" for user interface");
			break;
	}
  #endif

  printf("Hello World\n");

  test();

	return 0;
}
