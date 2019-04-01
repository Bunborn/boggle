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
    myDict->isFound = malloc(sizeof(int) * myDict->numWords);
    myDict->words = (char**) calloc((unsigned)myDict->numWords, sizeof(char*));
    for ( int i = 0; i < myDict->numWords; i++ )
    {
        myDict->words[i] = (char*) calloc(40, sizeof(char));
        myDict->isFound[i] = false;
    }
}
bool couldBeValid(char* string, struct dictionary *myDict, int strLength)
{
    for(int i=0; i<myDict->numWords; i++) //goes through dictionary
    {
        if(strncmp(string, myDict->words[i], strLength) == 0) //if this string matches with at least part of one string (from the front) of dictionary
        {
            return true;
        }
    }
    return false;
}
int findValidWord(char* input, struct dictionary *myDict) //checks if word is valid and matches dictionary
{
    int length = strLength(input);
    if(length < 3) //words must be greater than 3 letters
    {
        return false;
    }
    for(int i=0; i<myDict->numWords; i++)
    {
        if((strncmp(input, myDict->words[i], 50) == 0) && myDict->isFound[i] == false) //compares to see if already found and if matches dictionary. No 50+ letter word in English lagnuage
        {
            myDict->isFound[i] = true;
            return i;
        }
    }
    return 0;
}
int strLength(char* input) //calculates length of s tring
{
    int length = 0;
    while(input[length] != '\0')
    {
        length++;
    }
    return length;
}
void freeDictionary(struct dictionary *myDict) //memory deallocation
{
    for ( int i = 0; i < myDict->numWords; i++ )
    {
        free(myDict->words[i]);
    }
    free(myDict->words);
    free(myDict);
}