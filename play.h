//
// Created by Brandon on 3/25/2019.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "scanner.h"
#include "board.h"
#include "words.h"
#ifndef BOGGLE_PLAY_H
#define BOGGLE_PLAY_H
struct game
{
    int score;
    int numValidWords;
    int totalPossibleScore;
    char ** validWordList;
    bool * beenGuessed;
};
void findAllWords(struct board *gameBoard, struct dictionary *myDict, struct game *currGame);
void search(struct board *gameBoard, struct dictionary *myDict, struct game *currGame, int row, int col, char* string);
void buildGame(struct game *currGame, struct dictionary *myDict);
int findPoints(char* string);
void printScore(struct game *currGame);
void fillValidWords(struct game *currGame, struct dictionary *myDict);
//print all possible words
void freeGame(struct game *currGame);
#endif //BOGGLE_PLAY_H
