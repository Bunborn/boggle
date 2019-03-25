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
    struct dictionary *myDictionary = malloc(sizeof(struct dictionary));
    buildDictionary(myDictionary);
    myDictionary->dictionaryPtr = fptr;
    countDictionaryWords(myDictionary);
    //printf("You are using %d words\n", totalWords);
    readDictionaryFile(myDictionary);

    printIntro();
    printf("Time to boggle! Let's figure out our board size (minimum 3x3)\n");
    printf("Large game boards can lead to poor rendering and long load times.\n");
    struct board *gameBoard = malloc(sizeof(struct board));
    getBoardInfo(gameBoard);
    buildBoard(gameBoard);
    fillBoard(gameBoard);
    printf("You will be using a board size of %d by %d with a total of %d letters to tango with.\n", gameBoard->cols, gameBoard->rows, gameBoard->cols * gameBoard->rows);
    printf("Loading...\n");
    findAllWords(gameBoard, myDictionary);
    printBoard(gameBoard);


    freeBoard(gameBoard);
    freeDictionary(myDictionary);
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