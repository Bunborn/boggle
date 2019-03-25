//
// Created by Brandon on 3/25/2019.
//
#ifndef BOGGLE_BOARD_H
#define BOGGLE_BOARD_H
//Add:struct for a letter cube
struct board
{
    unsigned int rows;
    unsigned int cols;
    char** cubes;
    bool** isVisted;
};

void printBoard(struct board *gameBoard);
void getBoardInfo(struct board *gameBoard); //returns totalLetters
char makeRandomCharacter();
void fillBoard(struct board *gameBoard);
void freeBoard();

#endif //BOGGLE_BOARD_H
