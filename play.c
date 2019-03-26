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

    string[stringLength] = tolower(gameBoard->cubes[rows][cols]);
    string[stringLength+1] = '\0';

    if(findValidWord(string, myDict)>0)
    {
        currGame->validWordList[currGame->numValidWords] = string;
        printf("added %s into validwordlist at index %d\n", currGame->validWordList[currGame->numValidWords], currGame->numValidWords);
        currGame->numValidWords++;
        currGame->totalPossibleScore += findPoints(string);
       // printf("%s is valid\n", string);
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

int findPoints(char* string)
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
void buildGame(struct game *currGame, struct dictionary *myDict)
{
    currGame->beenGuessed = malloc(sizeof(bool) * myDict->numWords);
    currGame->validWordList = (char**) calloc((unsigned)myDict->numWords, sizeof(char*));
    for ( int i = 0; i < myDict->numWords; i++ )
    {
        currGame->validWordList[i] = (char*) calloc(40, sizeof(char));
        currGame->beenGuessed = false;
    }

}
void printScore(struct game *currGame)
{
    printf("Current user score is: %d\n", currGame->score);
}

void freeGame(struct game *currGame)
{
    free(currGame);
}