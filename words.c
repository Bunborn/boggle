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
    myDict->root = NULL; // will be built after readDictionaryFile
}
struct TrieNode *newTrieNode()
{
    struct TrieNode *node = calloc(1, sizeof(struct TrieNode));
    node->is_word = false;
    node->word_index = -1;
    return node;
}

void insertTrie(struct TrieNode *root, char *word, int index)
{
    struct TrieNode *curr = root;
    for(int i = 0; word[i] != '\0'; i++)
    {
        int c = tolower((unsigned char)word[i]) - 'a';
        if(c < 0 || c >= ALPHABET_SIZE) return; // skip words with non-alpha chars
        if(curr->children[c] == NULL)
            curr->children[c] = newTrieNode();
        curr = curr->children[c];
    }
    curr->is_word = true;
    curr->word_index = index;
}

void buildTrie(struct dictionary *myDict)
{
    myDict->root = newTrieNode();
    for(int i = 0; i < myDict->numWords; i++)
    {
        if(strLength(myDict->words[i]) >= 3)
            insertTrie(myDict->root, myDict->words[i], i);
    }
}

void freeTrie(struct TrieNode *node)
{
    if(node == NULL) return;
    for(int i = 0; i < ALPHABET_SIZE; i++)
        freeTrie(node->children[i]);
    free(node);
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
    freeTrie(myDict->root);
    free(myDict);
}