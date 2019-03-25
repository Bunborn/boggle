//
// Created by Brandon on 3/25/2019.
//

#include "play.h"

void findAllWords(struct board *gameBoard, struct dictionary *myDict) //DFS algo to find all legal words on the board
{
    char string[25] = "";
    for(int rows = 0; rows < gameBoard->rows; rows++)
    {
        for(int cols = 0; cols < gameBoard->cols; cols++)
        {
            search(gameBoard, myDict, rows, cols, string);
        }
    }
}

void search(struct board *gameBoard, struct dictionary *myDict, int rows, int cols, char* string)
{
    int stringLength = strLength(string);
    gameBoard->isVisted[rows][cols] = true;

    string[stringLength] = toupper(gameBoard->cubes[rows][cols]);
    string[stringLength+1] = '\0';

    if(isValidWord(string, myDict))
    {
        printf("%s is a valid word!\n", string);
    }
    for(int i = rows - 1; i <= rows + 1 && i < gameBoard->rows; i++ )
    {
        for(int j = cols - 1; j <= cols+1 && j < gameBoard->cols; j++)
        {
            if(i>=0 && j>= 0 && gameBoard->isVisted[i][j] == false)
                search(gameBoard, myDict, i, j, string);
        }
    }
    string[stringLength-1] = '\0';
    gameBoard->isVisted[rows][cols] = false;
}