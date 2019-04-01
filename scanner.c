#include <stdio.h>
#include <stdlib.h>  /* for exit, malloc, realloc */
#include <ctype.h>   /* for isspace */
#include <string.h>  /* for strdup */
#include "scanner.h"

static void *allocateMsg(size_t size,char *where);
static void *reallocateMsg(void *s,size_t size,char *where);

/* VERSION 1.3
 *
 * scanner.c - a collection of input routines for C
 *           - written by John C. Lusth and modified by Brandon Stevens
 *           - in general, the functions return what is implied by their names
 *
 *    readLine(FILE *fp)
 *      - reads in a line or remainder of a line
 *      - returns a malloc'd string; the newline is not included
 *        the caller should free the returned string
 *      - returns 0 if end of file; feof will subsequently return true
 *      - usage example: char *x = readLine(stdin);
 *    getValidInt()
 *      - returns integer value
 */

static void skipWhiteSpace(FILE *);
static char convertEscapedChar(int);

/********** public functions **********************/

char * readLine(FILE *fp)
{
    int ch,index;
    char *buffer;
    int size = 512;

    ch = fgetc(fp);
    if (ch == EOF) return 0;

    buffer = allocateMsg(size,"readLine");

    index = 0;
    while (ch != '\n')
    {
        if (ch == EOF) break;
        if (index > size - 2)
        {
            ++size;
            buffer = reallocateMsg(buffer,size,"readLine");
        }
        buffer[index] = ch;
        ++index;
        ch = fgetc(fp);
    }

    if (index > 0)              //there is something in the buffer
        clearerr(fp);           //so force the read to be good
    buffer[index] = '\0';
    return buffer;
}



int getValidInt()
{
    char* inputString = readLine(stdin);
    long inputNum;
    inputNum = strtol(inputString, &inputString, 10);
    return (int)inputNum;
}
/********** private functions **********************/

static void
skipWhiteSpace(FILE *fp)
{
    int ch;

    /* read chars until a non-whitespace character is encountered */

    while ((ch = fgetc(fp)) != EOF && isspace(ch))
        continue;

    /* a non-space character got us out of the loop, so push it back */

    if (ch != EOF) ungetc(ch,fp);
}

static char
convertEscapedChar(int ch)
{
    switch (ch)
    {
        case 'n':  return '\n';
        case 't':  return '\t';
        case '"':  return '\"';
        case '\\': return '\\';
    }
    return ch;
}


void *
allocateMsg(size_t size,char *where)
{
    char *s;
    s = malloc(size);
    if (s == 0)
    {
        fprintf(stderr,"%s: could not allocate string, out of memory\n",
                where);
        exit(3);
    }
    return s;
}

static void *
reallocateMsg(void *s,size_t size,char *where)
{
    char *t;
    t = realloc(s,size);
    if (t == 0)
    {
        fprintf(stderr,"%s: could not reallocate string, out of memory\n",
                where);
        exit(3);
    }
    return t;
}
