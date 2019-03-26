//
// Created by Brandon on 3/25/2019.
//

#include "play.h"

void findAllWords(struct board *gameBoard, struct dictionary *myDict, struct game *currGame) //DFS algo to find all legal words on the board
{
    char string[25] = "";
    for(int rows = 0; rows < gameBoard->rows; rows++)
    {
        for(int cols = 0; cols < gameBoard->cols; cols++)
        {
            search(gameBoard, myDict, currGame, rows, cols, string);
        }
    }
}

void search(struct board *gameBoard, struct dictionary *myDict, struct game *currGame, int rows, int cols, char* string)
{
    int stringLength = strLength(string);
    gameBoard->isVisted[rows][cols] = true;

    string[stringLength] = toupper(gameBoard->cubes[rows][cols]);
    string[stringLength+1] = '\0';

    if(isValidWord(string, myDict))
    {
        currGame->numValidWords++;
        currGame->totalPossibleScore += findPoints(string, currGame);
        //currGame->
    }
    for(int i = rows - 1; i <= rows + 1 && i < gameBoard->rows; i++ ) //dfs in all 8 possible directions
    {
        for(int j = cols - 1; j <= cols+1 && j < gameBoard->cols; j++)
        {
            if(i>=0 && j>= 0 && gameBoard->isVisted[i][j] == false)
                search(gameBoard, myDict, currGame, i, j, string);
        }
    }
    string[stringLength-1] = '\0';
    gameBoard->isVisted[rows][cols] = false;
}

int findPoints(char* string, struct game *currGame)
{
    int points=0;
    int stringLength = strLength(string);
    if(stringLength == 3 || stringLength == 4)
        points = 1;
    else if(stringLength == 5)
        points = 2;
    else if(stringLength == 6)
        points = 3;
    else if(stringLength == 7)
        points = 5;
    else if(stringLength >= 8)
        points = 11;
    printf("%s is worth %d\n", string, points);
    return points;
}

void freeGame(struct game *currGame)
{
    free(currGame);
}