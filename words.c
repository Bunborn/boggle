//
// Created by Brandon on 3/25/2019.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include "scanner.h"
#include "words.h"


int countDictionaryWords(struct dictionary *myDictionary)
{
    int totalWords = 0;
    char * read = "";
    while(read != 0)
    {
        read = readLine(myDictionary->dictionaryPtr);
        totalWords++;
    }
    totalWords--; //most files will have an empty line at the end, so let's disregard that line
    myDictionary->numWords = totalWords;
    return totalWords;
}
void readDictionaryFile(struct dictionary *myDictionary)
{
    rewind((myDictionary->dictionaryPtr)); //points back to top of text file
    for(int i=0; i < myDictionary->numWords; i++)
    {
        myDictionary->words[i] = readLine(myDictionary->dictionaryPtr);
    }
}



void buildDictionary(struct dictionary *myDict)
{
    myDict->words = (char**) calloc(myDict->numWords, sizeof(char*));
    for ( int i = 0; i < myDict->numWords; i++ )
    {
        myDict->words[i] = (char*) calloc(25, sizeof(char));
    }
}