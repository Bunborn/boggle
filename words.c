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
    myDict->isFound = malloc(sizeof(bool) * myDict->numWords);
    myDict->words = (char**) calloc(myDict->numWords, sizeof(char*));
    for ( int i = 0; i < myDict->numWords; i++ )
    {
        myDict->words[i] = (char*) calloc(40, sizeof(char));
        myDict->isFound[i] = false;
    }
}

bool isValidWord(char* input, struct dictionary *myDict)
{
    int length = strLength(input);
    if(length < 3)
    {
        return false;
    }
    for(int i=0; i<myDict->numWords; i++)
    {
        if(strcmp(input, myDict->words[i]) == 0 && myDict->isFound[i] == false)
        {
            myDict->isFound[i] = true;
            return true;
        }
    }
    return false;
}
int strLength(char* input)
{
    int len = 0;
    while(input[len] != '\0')
    {
        len++;
    }
    return len;
}
void freeDictionary(struct dictionary *myDict)
{
    for ( int i = 0; i < myDict->numWords; i++ )
    {
        free(myDict->words[i]);
    }
    free(myDict->words);
}