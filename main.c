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
//fix DFS
//fix the guessing same word twice

void menu(struct board * gameBoard, struct game * currGame, struct dictionary * myDict); //prints menu and allows user to choose what to do
void printRules(); //prints boggle rules
void printBoggleArt();
void singlePlayerGame(struct board * gameBoard, struct game * currGame, struct dictionary * myDict);
void multiPlayerGame(struct board * gameBoard, struct game * currGame, struct dictionary * myDict, int players);


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

    struct board *gameBoard = malloc(sizeof(struct board));

    menu(gameBoard, currGame, myDictionary); //prints game menu

    freeBoard(gameBoard);
    freeDictionary(myDictionary);
    freeGame(currGame);
}
void menu(struct board * gameBoard, struct game * currGame, struct dictionary * myDict)
{
    printBoggleArt();

    //type 1 for single player
    //type 2 for 2 player
    //type 3 for boggle solver
    //type 4 to see rules
    //type 5 to exit

    while (1)
    {
        fflush((stdin));
        printf("1 for single-player, 2 for multi-player, 3 for solver, 4 for rules, 5 to see highscore, 6 to exit \n");
        char* response = readLine(stdin);
        if(strncmp(response, "1", 2) == 0)
        {
            singlePlayerGame(gameBoard, currGame, myDict);
        }
        else if(strncmp(response, "2", 2) == 0)
        {
            printf("How many players?");
            int playerNum = getValidInt();
            multiPlayerGame(gameBoard, currGame, myDict, playerNum);
        }
        else if(strncmp(response, "3", 2) == 0)
        {
            printf("Prepare to enter your boggle board to solve");
        }
        else if(strncmp(response, "4", 2) == 0)
        {
            printRules();
        }
        else if(strncmp(response, "5", 2) == 0)
        {
            printf("High score for this session: %d\n", currGame->highScore);
        }
        else if(strncmp(response, "6", 2) == 0)
        {
            printf("See you next time for more boggle'ing!");
            freeBoard(gameBoard);
            freeDictionary(myDict);
            freeGame(currGame);
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
    printf("Let's figure out our board size (minimum 2x2, standard 4x4)\n");
    printf("Game boards above 4x4 can lead to either poor rendering or long initial load time.\n");
    getBoardInfo(gameBoard);
    buildBoard(gameBoard);
    fillBoard(gameBoard);
    printf("How long would you like to play for in seconds (180 is standard play)?");
    int gameTime = getValidInt();
    printf("Enter \'1\' to re-print the board, \'2\' to print current score, \'3\' to end early\n");
    printf("Loading...\n");
    findAllWords(gameBoard, myDict, currGame);
    fillValidWords(currGame, myDict);
    printf("Finished loading. Hit enter to start Boggle single player.\n");
    fflush(stdin);
    readLine(stdin);
    printBoard(gameBoard);
    currGame->score = 0;
    clock_t startTime, currentTime;
    double timeElapsed = 0.0;
    startTime = clock();
    char* userInput;
    fflush(stdin);
    bool guessedCorrect = false;
    while(timeElapsed<gameTime)
    {
        userInput = readLine(stdin);
        guessedCorrect = false;
        currentTime = clock();
        timeElapsed = (double)(currentTime - startTime)/CLOCKS_PER_SEC;
        if(timeElapsed > gameTime)
        {
            break;
        }
        if(strncmp(userInput,"1",2) == 0)
            printBoard(gameBoard);
        if(strncmp(userInput,"2",2) == 0)
            printScore(currGame);
        if(strncmp(userInput,"3",2) == 0)
            break;
        for(int i=0; i<currGame->numValidWords; i++)
        {
            printf("validWord = %s\n", currGame->validWordList[i]);
            if(currGame->beenGuessed[i] == true )
            {
                guessedCorrect = true; //did guess a correct word
                printf("You've already used word %s, no points", userInput);
                break;
            }
            if((strncmp(userInput,currGame->validWordList[i], 45) == 0))
            {
                printf("%s is a match! You receive %d points", userInput, findPoints(userInput));
                currGame->score += findPoints((userInput));
                currGame->beenGuessed[i] = true;
                guessedCorrect = true;
                break;
            }
        }
        if(guessedCorrect == false)
        {
            printf("%s is NOT a match.", userInput);
        }

    }
    printf("Game over! You scored a total of %d\n",currGame->score);
    printf("The total possible score was %d.\n", currGame->totalPossibleScore);
    if(currGame->numValidWords>0)
    {
        if(currGame->score == currGame->totalPossibleScore)
            printf("Nice job! You guessed every possible word in the board\n");
        printf("Hit enter to see all valid words\n");
        fflush(stdin);
        getchar();
        for(int i =0; i<currGame->numValidWords; i++)
        {
            printf("Word %d: %s\n", i+1, currGame->validWordList[i]);
        }
    }

    printf("Hit enter to return to main menu");
    fflush(stdin);
    getchar();
    freeBoard(gameBoard);
    if(currGame->score > currGame->highScore)
    {
        currGame->highScore = currGame->score;
    }
    menu(gameBoard, currGame, myDict);
}
void multiPlayerGame(struct board * gameBoard, struct game * currGame, struct dictionary * myDict, int players)
{
    printf("Welcome to the multi player boggle challenge.\n");
    printf("We will take turns alternating playing on the same board, highest score wins! Only one player looks at the screen during his/her round\n");
    printf("Let's figure out our board size (minimum 2x2, standard 4x4)\n");
    printf("Game boards above 4x4 can lead to either poor rendering or long initial load time.\n");
    getBoardInfo(gameBoard);
    buildBoard(gameBoard);
    fillBoard(gameBoard);
    printf("How long would you like to play for in seconds (180 is standard play)?");
    int gameTime = getValidInt();
    printf("Enter \'1\' to re-print the board, \'2\' to print current score, \'3\' to end early\n");
    printf("Loading...\n");
    findAllWords(gameBoard, myDict, currGame);
    fillValidWords(currGame, myDict);
    int playerScore[players];
    for(int currPlayerNum = 1; currPlayerNum < players+1; currPlayerNum++) //1 based indexed
    {
        for(int i=0; i< currGame->numValidWords; i++)
        {
            currGame->beenGuessed[i] = false;
        }
        printf("Hit enter to start Boggle play for player %d", currPlayerNum);
        fflush(stdin);
        getchar();
        printBoard(gameBoard);
        currGame->score = 0;
        clock_t startTime;
        clock_t currentTime;
        double timeElapsed = 0.0;
        startTime = clock();
        char* userInput;
        fflush(stdin);
        bool guessedCorrect = false;
        while(timeElapsed<gameTime)
        {
            userInput = readLine(stdin);
            guessedCorrect = false;
            currentTime = clock();
            timeElapsed = (double)(currentTime - startTime)/CLOCKS_PER_SEC;
            if(timeElapsed > gameTime)
            {
                break;
            }
            if(strncmp(userInput,"1",2) == 0)
                printBoard(gameBoard);
            if(strncmp(userInput,"2",2) == 0)
                printScore(currGame);
            if(strncmp(userInput,"3",2) == 0)
                break;
            for(int i=0; i<currGame->numValidWords; i++)
            {
                //printf("validWord = %s\n", currGame->validWordList[i]);
                if(currGame->beenGuessed[i] == true )
                {
                    guessedCorrect = true; //did guess a correct word
                    printf("You've already used word %s, no points", userInput);
                    break;
                }
                if((strncmp(userInput,currGame->validWordList[i], 45) == 0))
                {
                    printf("%s is a match! You receive %d points", userInput, findPoints(userInput));
                    currGame->score += findPoints((userInput));
                    currGame->beenGuessed[i] = true;
                    guessedCorrect = true;
                    break;
                }
            }
            if(guessedCorrect == false)
            {
                printf("%s is NOT a match.", userInput);
            }
        }
        playerScore[currPlayerNum] = currGame->score;
        printf("Game over for player %d! You scored a total of %d\n",currPlayerNum, currGame->score);
        printf("The total possible score was %d.\n", currGame->totalPossibleScore);
        if(currGame->score == currGame->totalPossibleScore && currGame->numValidWords>0)
        {
            printf("Nice job! You guessed every possible word in the board\n");
        }
        if(currGame->score > currGame->highScore)
        {
            currGame->highScore = currGame->score;
        }
    }
    printf("Our total scores for each player:\n");
    //int winningScore = 0;
    for(int i=1; i<players+1; i++) //1 based indexing
    {
        printf("Player %d: %d\n", i, playerScore[i]);
    }
    printf("Hit enter to return to main menu");
    fflush(stdin);
    getchar();
    freeBoard(gameBoard);
    menu(gameBoard, currGame, myDict);
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
    printf("Hit enter to return to main menu");
    getchar();
}
void printBoggleArt()
{
    printf(".__ .__..__ .__ .   .___\n");
    printf("[__)|  |[ __[ __|   [__ \n");
    printf("[__)|__|[_./[_./|___[___\n\n");
}