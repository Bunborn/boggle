#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include "board.h"
#include "scanner.h"
#include "words.h"
#include "play.h"
#include <time.h>

void printIntro(); //prints intro text
void printRules();


int main() {
    srand(time(0)); //use current time as seed for random generator.
    FILE *fptr;
    fptr = fopen("dictionary.txt", "r");
    if (fptr == NULL)
    {
        printf("Cannot open dictionary.txt file. Refer to readme for help. \n");
        exit(0);
    }
    struct game *currGame = malloc(sizeof(struct game));
    struct dictionary *myDictionary = malloc(sizeof(struct dictionary));
    buildDictionary(myDictionary);
    myDictionary->dictionaryPtr = fptr;
    countDictionaryWords(myDictionary);
    //printf("You are using %d words\n", totalWords);
    readDictionaryFile(myDictionary);

    printIntro();
    printf("Time to boggle! Let's figure out our board size (minimum 3x3)\n");
    printf("Game boards above 4x4 can lead to either poor rendering or long initial load time.\n");
    struct board *gameBoard = malloc(sizeof(struct board));
    getBoardInfo(gameBoard);
    buildBoard(gameBoard);
    fillBoard(gameBoard);
    printf("You will be using a board size of %d by %d with a total of %d letters to tango with.\n", gameBoard->cols, gameBoard->rows, gameBoard->cols * gameBoard->rows);
    printf("Enter \'1\' to re-print the board, \'2\' to print current score, \'3\' to end early\n");
    printf("Loading...\n");
    findAllWords(gameBoard, myDictionary, currGame);
    printBoard(gameBoard);



    clock_t startTime, currentTime;
    double timeElapsed = 0.0;
    startTime = clock();
    char* userInput;
    while(timeElapsed<10.0)
    {
        currentTime = clock();
        timeElapsed = ((double)currentTime - (double)startTime)/CLOCKS_PER_SEC;
        userInput = readLine(stdin);
        if(strcmp(userInput,"1") == 0)
            printBoard(gameBoard);
        if(strcmp(userInput,"2") == 0)
            ;//print score
        if(strcmp(userInput,"3") == 0)
            break;

    }
    printf("Game over! You scored a total of ");
    printf("The total possible score was %d", currGame->totalPossibleScore);


    freeBoard(gameBoard);
    freeDictionary(myDictionary);
    freeGame(currGame);
}
void printIntro()
{
    printf("Welcome to boggle!\n");
    printf("Would you like to see the rules? y/n ");
    while (1)
    {
        char response = getValidChar();
        if(response == 'y')
        {
            printRules();
            break;
        }
        else if(response == 'n')
        {
            break;
        }
        else
        {
            printf("Invalid input. Would you like to see the rules? y/n ");
        }
    }
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
}