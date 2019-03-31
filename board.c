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
    printf("Choose your board dimension (MxM):\n" );
    int dimension = getValidInt();
    gameBoard->rows = dimension;
    gameBoard->cols = dimension;
    if(dimension < 2) //invalid board size
    {
        printf("This is not a valid board. (minimum 2x2)\n");
        getBoardInfo(gameBoard);
    }
    if(gameBoard->rows > 8 || gameBoard->cols > 8)
    {
        printf("WARNING: Large game board detected. Longer than normal load time and possible screen rendering issues.\n");
    }
}
void fillBoard(struct board *gameBoard)
{
    for(int cols = 0; cols < gameBoard->cols; cols++)
    {
        for(int rows=0; rows < gameBoard->rows; rows++)
        {
            gameBoard->cubes[cols][rows] = makeRandomCharacter();
            gameBoard->isVisited[cols][rows] = false;
        }
    }
}
void buildBoard(struct board *gameBoard)
{
    gameBoard->cubes = (char**) calloc(gameBoard->rows, sizeof(char*));
    gameBoard->isVisited = (int**) calloc(gameBoard->rows, sizeof(int*));
    for ( int i = 0; i < gameBoard->rows; i++ )
    {
        gameBoard->cubes[i] = (char*) calloc(gameBoard->cols, sizeof(char));
        gameBoard->isVisited[i] = (int*) calloc(gameBoard->cols, sizeof(int));
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
    printf("\n");
}

void freeBoard(struct board *gameBoard)
{
    free(gameBoard->cubes);
    free(gameBoard->isVisited);
    free(gameBoard);
}

