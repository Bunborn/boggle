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
    int * multiPlayerScore;
};

void findAllWords(struct board *gameBoard, struct dictionary *myDict, struct game *currGame); //finds all legal words on boggle board
void search(struct board *gameBoard, struct dictionary *myDict, struct game *currGame, int row, int col, bool isFirstRun, char* path); //DFS used in findAllWords
bool isAllowed(int row, int col, struct board *gameBoard); //sees if allowed to move into this space in DFS
void buildGame(struct game *currGame, struct dictionary *myDict); //allocates memory
int findPoints(char* string); //finds points for a string
void printScore(struct game *currGame);
void fillValidWords(struct game *currGame, struct dictionary *myDict);
void setVisitedFlagsFalse(struct board *gameBoard); //sets flags for all letter cubes as false during game
void resetGame(struct game * currGame); //reset scores, etc
void resetMultiPlayerGame(struct game * currGame);
void freeGame(struct game *currGame);

#endif //BOGGLE_PLAY_H
