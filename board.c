//
// Created by Brandon on 3/25/2019.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <zconf.h>
#include "scanner.h"
#include "board.h"

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
        printf("Warning: Large game boards can lead to poor rendering on your monitor.");
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
        sleep(1);
        printf("|");
        for(i=0; i< gameBoard->rows; i++)
        {
            printf(" %c", makeRandomCharacter());
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
    free(gameBoard);
}