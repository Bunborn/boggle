//
// Created by Brandon on 3/25/2019.
//
#include "words.h"
#ifndef BOGGLE_BOARD_H
#define BOGGLE_BOARD_H
//Add:struct for a letter cube
struct board
{
    unsigned int rows;
    unsigned int cols;
    char** cubes;
    bool** isVisited;
};

void printBoard(struct board *gameBoard);
void getBoardInfo(struct board *gameBoard);
char makeRandomCharacter();
void buildBoard(struct board *gameBoard); //allocates heap memory for board
void fillBoard(struct board *gameBoard); //fills board with random chars
void freeBoard(struct board *gameBoard);


#endif //BOGGLE_BOARD_H
