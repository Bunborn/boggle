//
// Created by Brandon on 3/25/2019.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#ifndef BOGGLE_WORDS_H
#define BOGGLE_WORDS_H

#define ALPHABET_SIZE 26

struct TrieNode
{
    struct TrieNode *children[ALPHABET_SIZE];
    bool is_word;
    int word_index;
};

struct dictionary
{
    int numWords;
    FILE *dictionaryPtr;
    int* isFound;
    char** words; //matrix of strings
    struct TrieNode *root;
};

void buildDictionary(struct dictionary *myDictionary);
int countDictionaryWords(struct dictionary *myDictionary);
void readDictionaryFile(struct dictionary *myDictionary);
int strLength(char* input);
void freeDictionary(struct dictionary *myDictionary);

struct TrieNode *newTrieNode();
void insertTrie(struct TrieNode *root, char *word, int index);
void buildTrie(struct dictionary *myDict);
void freeTrie(struct TrieNode *node);

#endif //BOGGLE_WORDS_H
