//
// Created by Brandon on 3/25/2019.
//
#include "words.h"
#ifndef BOGGLE_BOARD_H
#define BOGGLE_BOARD_H
struct board
{
    unsigned int rows;
    unsigned int cols;
    char** cubes; //letter cubes
    int** isVisited; //tracks if a cube has been visited
};

void printBoard(struct board *gameBoard);
void getBoardInfo(struct board *gameBoard);
char makeRandomCharacter();
void buildBoard(struct board *gameBoard); //allocates heap memory for board
void fillBoard(struct board *gameBoard); //fills board with random chars
void freeBoard(struct board *gameBoard);

#endif //BOGGLE_BOARD_H
