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
struct gameState
{
    int score;
};
void findAllWords(struct board *gameBoard, struct dictionary *myDict);
void search(struct board *gameBoard, struct dictionary *myDict, int col, int row, char* string);
#endif //BOGGLE_PLAY_H
