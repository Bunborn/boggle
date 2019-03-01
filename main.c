#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <zconf.h>

#define MAX_STRING_LEN 80

void getBoardInfo(); //returns totalLetters
void printIntro(); //prints intro text
void printBoard();
void printRules();
int getValidInt(); //gets input from user and verifies it is an integer
char getValidChar();
char makeRandomCharacter();

int xSize;
int ySize;
int totalLetters;

int main() {
    srand(time(0)); //use current time as seed for random generator.
    printIntro();
    printf("Time to boggle! Let's figure out our board size (minimum 3x3)\n");
    getBoardInfo();
    printf("You will be using a board size of %d by %d with a total of %d letters to tango with.\n", xSize, ySize, totalLetters);
    printBoard();

}

int getValidInt()
{
    char inputString[MAX_STRING_LEN];
    scanf("%s",inputString);

    char *ptr;
    long inputNum;

    inputNum = strtol(inputString, &ptr, 10);
    return (int)inputNum;
}
char getValidChar()
{
    char inputString[MAX_STRING_LEN];
    scanf("%s",inputString);

    char inputChar = inputString[0];
    return inputChar;
}
void getBoardInfo() //returns totalLetters
{
    printf("Number of horizontal letters:" );
    xSize = getValidInt();
    printf("Number of vertical letters: ");
    ySize = getValidInt();
    totalLetters = xSize * ySize;
    if(xSize < 3 || ySize < 3) //invalid board size
    {
        printf("This is not a valid board. (minimum 3x3)\n");
        getBoardInfo();
    }
}
char makeRandomCharacter()
{
    int randomInt = rand();
    randomInt = randomInt % 26;
    char randomCharacter = randomInt + 65; //convert random integer to ASCII uppercase letter
    return randomCharacter;
}
void printBoard()
{
    int i=0;
    int j=0;
    printf(" ");
    for(i=0; i<2*xSize+1; i++)
    {
        printf("-");
    }
    printf("\n");
    for(j=0; j< ySize; j++)
    {
        sleep(1);
        printf("|");
        for(i=0; i< xSize; i++)
        {
            printf(" %c", makeRandomCharacter());
        }
        printf(" |\n");
    }
    printf(" ");
    for(i=0; i<2*xSize+1; i++)
    {
        printf("-");
    }
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
    printf("This program will allow words from the extensive 2015 Collins Scrabble list.\n");
    printf("You cannot reuse letter cubes in a single word or reuse the same word.\n");
    printf("-------------------------------\n");
    printf("You will get more points for longer words as follows:\n");
    printf("3 Letters: 1 point\n4 Letters: 1 point\n5 Letters: 2 points\n6 Letters: 3 points\n7 Letters: 5 points\n8 or More Letters: 11 points\n");
    printf("-------------------------------\n");
}