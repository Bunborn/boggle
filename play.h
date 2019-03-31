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
    int highScore;
    char ** validWordList;
    int * beenGuessed;
};



void findAllWords(struct board *gameBoard, struct dictionary *myDict, struct game *currGame);
void search(struct board *gameBoard, struct dictionary *myDict, struct game *currGame, int row, int col, bool isFirstRun, char* path);
void buildGame(struct game *currGame, struct dictionary *myDict);
int findPoints(char* string);
void printScore(struct game *currGame);
void fillValidWords(struct game *currGame, struct dictionary *myDict);
void setVisitedFlagsFalse(struct board *gameBoard);
bool isAllowed(int row, int col, struct board *gameBoard);

void freeGame(struct game *currGame);
#endif //BOGGLE_PLAY_H
