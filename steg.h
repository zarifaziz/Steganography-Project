/*
 * steg.h
 *
 *  Created on: 19 Oct 2015
 *      Author: Zarif Aziz
 */

#ifndef STEG_H_
#define STEG_H_

/* Declaring rules: Constants, arrays, integers, functions
 */
#define SMALLSIZE 5

const char *bmpfile;
const char *outputfile;

//Declaring functions
int decode(const char *bmpfile, const char *outputfile);

// Checking whether output file already exists
// if yes, will return 1. Otherwise will return 0
int checkfile(const char *filename);

// Asks whether user wants to output file
// if yes, will return 1. Otherwise will return 0
int overwritePromptYes(const char *outputfile);

#endif /* STEG_H_ */
