//
// Created by Brandon on 3/25/2019.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include "scanner.h"
#include "board.h"
#include "words.h"

void getBoardInfo(struct board *gameBoard) //returns totalLetters
{
    printf("Number of horizontal letters:" );
    gameBoard->rows = getValidInt();
    printf("Number of vertical letters: ");
    gameBoard->cols = getValidInt();
    if(gameBoard->rows < 3 || gameBoard->cols < 3) //invalid board size
    {
        printf("This is not a valid board. (minimum 3x3)\n");
        getBoardInfo(gameBoard);
    }
    if(gameBoard->rows > 20 || gameBoard->cols > 20)
    {
        printf("WARNING: Large game boards can lead to poor rendering and long load times.");
    }
}
void fillBoard(struct board *gameBoard)
{
    for(int cols = 0; cols < gameBoard->cols; cols++)
    {
        for(int rows=0; rows < gameBoard->rows; rows++)
        {
            gameBoard->cubes[cols][rows] = makeRandomCharacter();
            gameBoard->isVisted[cols][rows] = false;
        }
    }
}
void buildBoard(struct board *gameBoard)
{
    gameBoard->cubes = (char**) calloc(gameBoard->rows, sizeof(char*));
    gameBoard->isVisted = (bool**) calloc(gameBoard->rows, sizeof(bool*));
    for ( int i = 0; i < gameBoard->rows; i++ )
    {
        gameBoard->cubes[i] = (char*) calloc(gameBoard->cols, sizeof(char));
        gameBoard->isVisted[i] = (bool*) calloc(gameBoard->cols, sizeof(bool));
    }
}
char makeRandomCharacter()
{
    int randomInt = rand();
    randomInt = randomInt % 26;
    char randomCharacter = randomInt + 65; //convert random integer to ASCII uppercase letter
    return randomCharacter;
}
void printBoard(struct board *gameBoard)
{
    int i=0;
    int j=0;
    printf(" ");
    for(i=0; i<2*gameBoard->rows+1; i++)
    {
        printf("-");
    }
    printf("\n");
    for(j=0; j< gameBoard->cols; j++)
    {
        //sleep(1);
        printf("|");
        for(i=0; i< gameBoard->rows; i++)
        {
            printf(" %c", gameBoard->cubes[j][i]);
        }
        printf(" |\n");
    }
    printf(" ");
    for(i=0; i<2*gameBoard->rows+1; i++)
    {
        printf("-");
    }
}

void freeBoard(struct board *gameBoard)
{
    for ( int i = 0; i < gameBoard->rows; i++ )
    {
        free(gameBoard->cubes[i]);
    }
    free(gameBoard->cubes);
}

void findWords(struct board *gameBoard, struct dictionary *myDict) //DFS algo to find all legal words on the board
{
    for(int cols = 0; cols < gameBoard->cols; cols++)
    {
        for(int rows = 0; rows < gameBoard->rows; rows++)
        {

        }
    }
}