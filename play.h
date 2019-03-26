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
};
void findAllWords(struct board *gameBoard, struct dictionary *myDict, struct game *currGame);
void search(struct board *gameBoard, struct dictionary *myDict, struct game *currGame, int row, int col, char* string);
int findPoints(char* string, struct game *currGame);
void freeGame(struct game *currGame);
#endif //BOGGLE_PLAY_H
