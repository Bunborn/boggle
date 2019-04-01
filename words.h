//
// Created by Brandon on 3/25/2019.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#ifndef BOGGLE_WORDS_H
#define BOGGLE_WORDS_H
struct dictionary
{
    int numWords;
    FILE *dictionaryPtr;
    int* isFound;
    char** words; //matrix of strings
};

void buildDictionary(struct dictionary *myDictionary);
int countDictionaryWords(struct dictionary *myDictionary);
void readDictionaryFile(struct dictionary *myDictionary);
bool couldBeValid(char* string, struct dictionary *myDict, int strLength);  //if this string matches with at least part of one string (from the front) of dictionary
int findValidWord(char* input, struct dictionary *myDict); //checks if word is valid and matches dictionary
int strLength(char* input);
void freeDictionary(struct dictionary *myDictionary);

#endif //BOGGLE_WORDS_H
