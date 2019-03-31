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
    int* isOnBoard;
    char** words;
};

void buildDictionary(struct dictionary *myDictionary);
int countDictionaryWords(struct dictionary *myDictionary);
void readDictionaryFile(struct dictionary *myDictionary);
void freeDictionary(struct dictionary *myDictionary);
int findValidWord(char* input, struct dictionary *myDict);
int strLength(char* input);

#endif //BOGGLE_WORDS_H
