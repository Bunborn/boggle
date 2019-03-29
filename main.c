#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include "board.h"
#include "scanner.h"
#include "words.h"
#include "play.h"

//TODO: make the scanner.c readInt use the readline! customer timer could be nice. loading bar??
//strcmp to strncmp's
//fix the isVisted flag
//find out why bool isFound isn't working
//flesh out menu
//2 player game mode
//finish the "free" functions
void menu(struct board * gameBoard, struct game * currGame, struct dictionary * myDict); //prints menu and allows user to choose what to do
void printRules(); //prints boggle rules
void singlePlayerGame(struct board * gameBoard, struct game * currGame, struct dictionary * myDict);


int main() {
    srand(time(0)); //use current time as seed for random generator.
    FILE *fptr; //file pointer
    fptr = fopen("dictionary.txt", "r"); //opens dictionary file
    if (fptr == NULL)
    {
        printf("Cannot open dictionary.txt file. Refer to readme for help. \n");
        exit(0);
    }
    struct dictionary *myDictionary = malloc(sizeof(struct dictionary));
    buildDictionary(myDictionary);
    myDictionary->dictionaryPtr = fptr;
    countDictionaryWords(myDictionary);
    readDictionaryFile(myDictionary);

    struct game *currGame = malloc(sizeof(struct game));
    buildGame(currGame, myDictionary);
    currGame->numValidWords = 0;
    currGame->score = 0;
    currGame->totalPossibleScore = 0;

    struct board *gameBoard = malloc(sizeof(struct board));
    getBoardInfo(gameBoard);
    buildBoard(gameBoard);
    fillBoard(gameBoard);

    menu(gameBoard, currGame, myDictionary);
    printf("Time to boggle! Let's figure out our board size (minimum 3x3)\n");
    printf("Game boards above 4x4 can lead to either poor rendering or long initial load time.\n");








    freeBoard(gameBoard);
    freeDictionary(myDictionary);
    freeGame(currGame);
}
void menu(struct board * gameBoard, struct game * currGame, struct dictionary * myDict)
{
    printf("Welcome to boggle!\n");
    //type 1 for single player
    //type 2 for 2 player
    //type 3 for boggle solver
    //type 4 to see rules
    //type 5 to exit

    while (1)
    {
        fflush((stdin));
        printf("1 for sp, 2 for 2p, 3 for solver, 4 for rules, 5 to exit ");
        char* response = readLine(stdin);
        if(strncmp(response, "1", 2) == 0)
        {
            singlePlayerGame(gameBoard, currGame, myDict);
        }
        else if(strncmp(response, "4", 2) == 0)
        {
            printRules();
        }
        else if(strncmp(response, "5", 2) == 0)
        {
            printf("See you next time for more boggle'ing!");
            exit(0);
        }
        else
        {
            printf("%s is not a valid input.\n", response);
        }
    }
}
void singlePlayerGame(struct board * gameBoard, struct game * currGame, struct dictionary * myDict)
{
    printf("Welcome to the single player boggle challenge.\n");
    printf("You will be using a board size of %d by %d with a total of %d letters to tango with.\n", gameBoard->cols, gameBoard->rows, gameBoard->cols * gameBoard->rows);
    printf("Enter \'1\' to re-print the board, \'2\' to print current score, \'3\' to end early\n");
    printf("Loading...\n");
    findAllWords(gameBoard, myDict, currGame);
    fillValidWords(currGame, myDict);
    printBoard(gameBoard);
    //single player game
    currGame->score = 0;
    clock_t startTime, currentTime;
    double timeElapsed = 0.0;
    startTime = clock();
    currentTime = clock();
    char* userInput;
    fflush(stdin);
    bool guessedCorrect = false;
    while(timeElapsed<30.0)
    {
        userInput = readLine(stdin);
        printf("time elapsed = %lf\n", timeElapsed);
        guessedCorrect = false;
        currentTime = clock();
        timeElapsed = (double)(currentTime - startTime)/CLOCKS_PER_SEC;
        if(strcmp(userInput,"1") == 0)
            printBoard(gameBoard);
        if(strcmp(userInput,"2") == 0)
            printScore(currGame);
        if(strcmp(userInput,"3") == 0)
            break;
        for(int i=0; i<currGame->numValidWords; i++)
        {
            //printf("3.i = %d\nword = %s\nnumValidWords = %d\n", i, currGame->validWordList[i], currGame->numValidWords);
            if((strncmp(userInput,currGame->validWordList[i], 45) == 0))// && currGame->beenGuessed[i] == false)
            {
                printf("%s is a match! You receive %d points", userInput, findPoints(userInput));
                currGame->score += findPoints((userInput));
                guessedCorrect = true;
                //currGame->beenGuessed[i] = true; //crashing program
                //printf("test");
                break;
            }
        }
        if(guessedCorrect == false)
        {
            printf("%s is NOT a match.", userInput);
        }

    }
    printf("Game over! You scored a total of %d\n",currGame->score);
    printf("The total possible score was %d.", currGame->totalPossibleScore);
}
void printRules()
{
    printf("\nFind as many words as you can on the given board!\n");
    printf("You will have 3 minutes to find these words.\n");
    printf("The letters must form a chain - which can be connected horizontally, vertically, or diagonally from one another.\n");
    printf("Words must contain at least 3 letters.\n");
    printf("This program will allow words from the dictionary file \"dictionary.txt\".\n");
    printf("You cannot reuse letter cubes in a single word or reuse the same word.\n");
    printf("-------------------------------\n");
    printf("You will get more points for longer words as follows:\n");
    printf("3 Letters: 1 point\n4 Letters: 1 point\n5 Letters: 2 points\n6 Letters: 3 points\n7 Letters: 5 points\n8 or More Letters: 11 points\n");
    printf("-------------------------------\n");
    //press any key to return to main menu
}