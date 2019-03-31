#ifndef SCANNER_H
#define SCANNER_H

/* VERSION 1.3
 *
 * scanner.h - public interface to scanner.c, the scanner module
 *
 */

#include <stdio.h>  //for use of FILE

extern int readInt(FILE *);
extern double readReal(FILE *);
extern char readChar(FILE *);
extern char readRawChar(FILE *);
extern char *readString(FILE *);
extern char *readToken(FILE *);
extern char *readLine(FILE *);
extern int stringPending(FILE *);

int getValidInt(); //gets input from user and verifies it is an integer
char getValidChar();
#endif
