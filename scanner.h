#ifndef SCANNER_H
#define SCANNER_H

/* VERSION 1.3
 *
 * scanner.h - public interface to scanner.c, the scanner module
 * By Dr. Lusth at University of Alabama, modified by Brandon Stevens
 */

#include <stdio.h>  //for use of FILE

extern char *readLine(FILE *); //reads line and keeps expanding buffer
int getValidInt(); //gets input from user and verifies it is an integer

#endif
