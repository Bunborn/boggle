//
// Created by Brandon on 3/25/2019.
//

#ifndef BOGGLE_WORDS_H
#define BOGGLE_WORDS_H
struct dictionary
{
    int numWords;
    FILE *dictionaryPtr;
    bool* isFound;
    char** words;
};

void buildDictionary(struct dictionary *myDictionary);
int countDictionaryWords(struct dictionary *myDictionary);
void readDictionaryFile(struct dictionary *myDictionary);
void freeDictionary(struct dictionary *myDictionary);

#endif //BOGGLE_WORDS_H
