//
// Created by Brandon on 3/25/2019.
//

#include "play.h"

// All possible movements for DFS
int row[] = { -1, -1, -1, 0, 1, 0, 1, 1 };
int col[] = { -1, 1, 0, -1, -1, 1, 0, 1 };

void findAllWords(struct board *gameBoard, struct dictionary *myDict, struct game *currGame) //DFS algo to find all legal words on the board
{
    char string[512] = "";
    bool isFirstRun = true;
    setVisitedFlagsFalse(gameBoard);
    for(int i = 0; i < gameBoard->rows; i++)
    {
        for(int j=0; j<gameBoard->rows; j++)
        {
            isFirstRun = true;
            printf("[i,j] = [%d,%d]\n", i, j);
            string[0] = '\0';
            search(gameBoard, myDict, currGame, i, j, isFirstRun, string);
        }
    }
}

void search(struct board *gameBoard, struct dictionary *myDict, struct game *currGame, int i, int j, bool isFirstRun, char* path)
{
    int pathLength = strLength(path);
    if((pathLength == 0 && isFirstRun == false) || pathLength > 10)
    {
        return;
    }
    gameBoard->isVisited[i][j] = true;
    isFirstRun = false;

    path[pathLength] = tolower(gameBoard->cubes[i][j]);
    path[pathLength+1] = '\0';

    printf("Path = %s\n", path);

    int wordIndex = findValidWord(path, myDict); //sees if in dictionary and if so where
    if(wordIndex>0)
    {
        printf("valid index at %d\n", wordIndex);
        printf("%s is valid!\n", path);
        myDict->isFound[wordIndex] = true;
        currGame->numValidWords++;
        currGame->totalPossibleScore += findPoints(path);
    }
    for(int k = 0; k < 8; k++)
    {
        if(isAllowed(i + row[k], j+col[k], gameBoard))
        {
            search(gameBoard, myDict, currGame, i + row[k], j + col[k], isFirstRun, path);
        }
    }
    pathLength = strLength(path);
    path[pathLength-1] = '\0';
    gameBoard->isVisited[i][j] = false;
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
    return points;
}
void buildGame(struct game *currGame, struct dictionary *myDict)
{
    currGame->beenGuessed = malloc(sizeof(int) * myDict->numWords);
    currGame->validWordList = (char**) calloc((unsigned)myDict->numWords, sizeof(char*));
    for ( int i = 0; i < myDict->numWords; i++ )
    {
        currGame->validWordList[i] = (char*) calloc(40, sizeof(char));
        currGame->beenGuessed[i] = false;
    }
    currGame->numValidWords = 0;
    currGame->score = 0;
    currGame->totalPossibleScore = 0;
    currGame->highScore = 0;
}
void printScore(struct game *currGame)
{
    printf("Player score is: %d\n", currGame->score);
}
void fillValidWords(struct game *currGame, struct dictionary *myDictionary)
{
    int counter=0;
    for(int i=0; i<myDictionary->numWords; i++)
    {
        if(myDictionary->isFound[i]==true)
        {
            currGame->validWordList[counter] = myDictionary->words[i];
            counter++;
        }
    }
}
bool isAllowed(int row, int col, struct board *gameBoard)
{
    return ((row>= 0 && row < gameBoard->rows) && (col >= 0 && col < gameBoard->cols) && gameBoard->isVisited[row][col] == false);
}
void setVisitedFlagsFalse(struct board *gameBoard)
{
    for(int i = 0; i < gameBoard->rows; i++)
    {
        for (int j = 0; j < gameBoard->cols; j++) {
            gameBoard->isVisited[i][j] = false;
        }
    }
}
void freeGame(struct game *currGame)
{
    for ( int i = 0; i < currGame->numValidWords; i++ )
    {
        free(currGame->validWordList[i]);
    }
    free(currGame->beenGuessed);
    free(currGame->validWordList);
    free(currGame);
}

