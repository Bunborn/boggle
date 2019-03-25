#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include "board.h"
#include "scanner.h"
#include <zconf.h> //to be removed

void printIntro(); //prints intro text
void printRules();


int main() {
    srand(time(0)); //use current time as seed for random generator.


    printIntro();
    printf("Time to boggle! Let's figure out our board size (minimum 3x3)\n");
    struct board gameBoard;// = malloc(sizeof(struct board));
    getBoardInfo(&gameBoard);

    //https://www.geeksforgeeks.org/dynamically-allocate-2d-array-c/ better
    //struct board * gameBoard = malloc(sizeof(struct board));
    //gameBoard->cubes  = (char **)malloc(sizeof(char)*totalLetters);
    printf("You will be using a board size of %d by %d with a total of %d letters to tango with.\n", gameBoard.cols, gameBoard.rows, gameBoard.cols * gameBoard.rows);
    printBoard(&gameBoard);

}
void printIntro()
{
    printf("Welcome to boggle!");
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
    printf("Choose time challenge for the true boggle experience.\n");
    printf("The letters must form a chain - which can be connected horizontally, vertically, or diagonally from one another.\n");
    printf("Words must contain at least 3 letters.\n");
    printf("This program will allow words from the dictionary file \"dictionary.txt\".\n");
    printf("You cannot reuse letter cubes in a single word or reuse the same word.\n");
    printf("-------------------------------\n");
    printf("You will get more points for longer words as follows:\n");
    printf("3 Letters: 1 point\n4 Letters: 1 point\n5 Letters: 2 points\n6 Letters: 3 points\n7 Letters: 5 points\n8 or More Letters: 11 points\n");
    printf("-------------------------------\n");
}