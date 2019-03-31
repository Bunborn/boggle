//
// Created by Brandon on 3/25/2019.
//

#include "play.h"

void findAllWords(struct board *gameBoard, struct dictionary *myDict, struct game *currGame) //DFS algo to find all legal words on the board
{
    //char string[25] = "";
    setVisitedFlagsFalse(gameBoard);
    for(int i = 0; i < gameBoard->rows; i++)
    {
        for(int j = 0; j < gameBoard->cols; j++)
        {
            char string[512] = "";
            //setVisitedFlagsFalse(gameBoard);
            //string[0] = tolower(gameBoard->cubes[i][j]);
            //string[1] = '\0';
            printf(" inspecting letter cube %c at [%d][%d]\n", gameBoard->cubes[i][j], i, j);
            search(gameBoard, myDict, currGame, i, j, string);
            //printf("FINDALLWORDS, Checking index [%d,%d]\n", row, cols);

        }
        printf("\n");
    }
    int counter = 0;
    for(int i=0; i<myDict->numWords;i++)
    {
        if(myDict->isOnBoard[i] == true)
        {
            currGame->validWordList[i] = myDict->words[i];
            printf("added %s into valid word list\n", currGame->validWordList[i]);
            counter++;
        }
    }
}

void search(struct board *gameBoard, struct dictionary *myDict, struct game *currGame, int i, int j, char* string)
{ //gets passed in a gameboard with all spots as not visited and [i][j] index for a letter cube along with empty string

    int stringLength = strLength(string);
    gameBoard->isVisted[i][j] = true;

    string[stringLength] = tolower(gameBoard->cubes[i][j]);
    string[stringLength+1] = '\0';

    int wordIndex = findValidWord(string, myDict); //sees if in dictionary and if so where
    //printf("string = %s\n", string); //debug, prints stack
    if(wordIndex>0) //you can ignore this, this is for me to build a list of real words on the board
    {
        myDict->isOnBoard[wordIndex] = true;
        currGame->numValidWords++;
        currGame->totalPossibleScore += findPoints(string);
    }
    for(int row = i - 1; row <= i + 1 && row < gameBoard->rows; row++ ) //dfs in all 8 possible directions
    {
        for(int col = j - 1; col <= j + 1 && col < gameBoard->cols; col++ )
        {
            if(row>=0 && col>= 0 && gameBoard->isVisted[col][row] == false)
            {
                search(gameBoard, myDict, currGame, col, row, string);
            }
        }
    }
    string[stringLength-1] = '\0';
    gameBoard->isVisted[i][j] = false;
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
        currGame->beenGuessed = false;
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
        if(myDictionary->isOnBoard[i]==true)
        {
            currGame->validWordList[counter] = myDictionary->words[i];
            counter++;
        }
    }
}
void setVisitedFlagsFalse(struct board *gameBoard)
{
    for(int i = 0; i < gameBoard->rows; i++)
    {
        for (int j = 0; j < gameBoard->cols; j++) {
            gameBoard->isVisted[i][j] = false;
        }
    }
}
void freeGame(struct game *currGame)
{
    for ( int i = 0; i < currGame->numValidWords; i++ )
    {
        free(currGame->validWordList[i]);
    }
    free(currGame->validWordList);
    free(currGame);
}