/*
 * Created by Brandon Stevens
 * Boggle game written in C to be ran on 18.04 Ubuntu LTS
 * Allows for singleplayer, multiplayer, and a boggle solver
 * More information on associated Github page
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include "board.h"
#include "scanner.h"
#include "words.h"
#include "play.h"

void menu(struct board * gameBoard, struct game * currGame, struct dictionary * myDict); //prints menu and allows user to choose what to do
void printRules(); //prints boggle rules
void printBoggleArt();
void printLoadingBar(struct board * gameBoard);
void singlePlayerGame(struct board * gameBoard, struct game * currGame, struct dictionary * myDict);
void multiPlayerGame(struct board * gameBoard, struct game * currGame, struct dictionary * myDict, int players);
void boggleSolver(struct board * gameBoard, struct game * currGame, struct dictionary * myDict);


int main() {
    srand(time(0)); //use current time as seed for random generator.
    FILE *fptr; //file pointer
    fptr = fopen("dictionary.txt", "r"); //opens dictionary file
    if (fptr == NULL)
    {
        printf("Cannot open dictionary.txt file. Refer to readme for help. \n");
        exit(1);
    }
    struct dictionary *myDictionary = malloc(sizeof(struct dictionary));
    myDictionary->dictionaryPtr = fptr; //points dictionary struct to text file
    countDictionaryWords(myDictionary);    
    buildDictionary(myDictionary);
    readDictionaryFile(myDictionary);

    struct game *currGame = malloc(sizeof(struct game));
    buildGame(currGame, myDictionary);

    struct board *gameBoard = malloc(sizeof(struct board));
    //will be 'built' at runtime

    menu(gameBoard, currGame, myDictionary); //prints game menu

    freeBoard(gameBoard);
    freeDictionary(myDictionary);
    freeGame(currGame);
}
void menu(struct board * gameBoard, struct game * currGame, struct dictionary * myDict)
{
    printBoggleArt();
    while (1)
    {
        printf("1 for single-player, 2 for multi-player, 3 for solver,\n4 to see rules, 5 to see this session's highscore, 6 to exit program \n");
        char* response = readLine(stdin);
        if(strncmp(response, "1", 2) == 0)
        {
            singlePlayerGame(gameBoard, currGame, myDict);
        }
        else if(strncmp(response, "2", 2) == 0)
        {
            resetMultiPlayerGame(currGame);
            printf("How many players (2-10)?\n");
            int playerNum = getValidInt();
            while(playerNum < 2 || playerNum > 10)
            {
                printf("Invalid input\n");
                playerNum = getValidInt();
            }
            multiPlayerGame(gameBoard, currGame, myDict, playerNum);
        }
        else if(strncmp(response, "3", 2) == 0)
        {
            boggleSolver(gameBoard,currGame,myDict);
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
            printf("See you next time for more boggle'ing!\n");
            exit(0);
        }
        else //not a choice
        {
            printf("%s is not a valid input.\n", response);
        }
    }
}
void singlePlayerGame(struct board * gameBoard, struct game * currGame, struct dictionary * myDict)
{
    resetGame(currGame);
    for(int i =0; i<myDict->numWords; i++)
    {
        myDict->isFound[i] = false;
    }
    printf("Welcome to the single player boggle challenge.\n");
    printf("Let's figure out our board size (minimum 2x2, standard 4x4)\n");
    printf("Large game boards can lead to both poor screen rendering or very long initial load time.\n");
    getBoardInfo(gameBoard);
    buildBoard(gameBoard);
    fillBoard(gameBoard);
    printf("How long would you like to play for in seconds (180 is standard play)?");
    int gameTime = getValidInt();
    printf("Enter \'1\' to re-print the board, \'2\' to print current score, \'3\' to view time left, \'4\' to end early.\n");
    printf("Loading...\n");
    printLoadingBar(gameBoard);
    findAllWords(gameBoard, myDict, currGame); //finds all valid words on boggle board
    fillValidWords(currGame, myDict);
    printf("]\nFinished loading. Hit enter to start Boggle single player.\n");
    readLine(stdin);
    printBoard(gameBoard);
    double timeElapsed = 0.0; //counts time elapsed since user began guessing
    char* userInput; //stores user guesses and other input
    bool guessedCorrect;
    time_t start = time(NULL); //time index for startTime of program
    while(timeElapsed<gameTime)
    {
        userInput = readLine(stdin);
        guessedCorrect = false;
        timeElapsed = (double)(time(NULL) - start);
        if(timeElapsed > gameTime)
        {
            break; //if out of time, don't let this guess go through
        }

        if(strncmp(userInput,"1",2) == 0)
            printBoard(gameBoard);
        else if(strncmp(userInput,"2",2) == 0)
            printScore(currGame);
        else if(strncmp(userInput,"3",2) == 0)
            printf("Time left = %.2f seconds.\n", gameTime - timeElapsed);
        else if(strncmp(userInput,"4",2) == 0)
            break;
        else
        {
            for(int wordNum=0; wordNum<currGame->numValidWords; wordNum++) //searches the valid boggle words found
            {
                if((strncmp(userInput,currGame->validWordList[wordNum], 50) == 0))
                {
                    if(currGame->beenGuessed[wordNum] == true )
                    {
                        guessedCorrect = true; //did guess a correct word
                        printf("You've already used word %s, no points\n", userInput);
                        break;
                    }
                    int points = findPoints(userInput);
                    printf("%s is a match! You receive %d points", userInput, points);
                    currGame->score += points;
                    currGame->beenGuessed[wordNum] = true;
                    guessedCorrect = true;
                    break;
                }
            }
            if(guessedCorrect == false)
            {
                printf("%s is NOT a match.", userInput);
            }
        }
    }
    printf("Game over! You scored a total of %d\n",currGame->score);
    printf("The total possible SCORE was %d with %d words.\n", currGame->totalPossibleScore, currGame->numValidWords);
    if(currGame->numValidWords>0) //if there were words to be found on the board
    {
        if(currGame->score == currGame->totalPossibleScore)
            printf("Nice job! You guessed every possible word in the board\n");
        printf("Hit enter to see all valid words\n");
        readLine(stdin);
        for(int i =0; i<currGame->numValidWords; i++)
        {
            printf("Word %d: %s\n", i+1, currGame->validWordList[i]);
        }
    }

    if(currGame->score > currGame->highScore)
    {
        currGame->highScore = currGame->score;
    }
    printf("Enter 1 to replay single player or anything else to go back to menu");
    char* buffer = readLine(stdin);
    if(strncmp(buffer,"1",3) == 0)
        singlePlayerGame(gameBoard, currGame, myDict);
    else
        menu(gameBoard, currGame, myDict);
}
void multiPlayerGame(struct board * gameBoard, struct game * currGame, struct dictionary * myDict, int players)
{
    resetGame(currGame);
    for(int i =0; i<myDict->numWords; i++)
    {
        myDict->isFound[i] = false;
    }
    printf("Welcome to the multi player boggle challenge.\n");
    printf("We will take turns alternating playing on the same board, highest score wins! Only one player looks at the screen during his/her round\n");
    printf("Let's figure out our board size (minimum 2x2, standard 4x4)\n");
    printf("Game boards above 4x4 can lead to either poor rendering or long initial load time.\n");
    getBoardInfo(gameBoard);
    buildBoard(gameBoard);
    fillBoard(gameBoard);
    printf("How long would you like to play for in seconds (180 is standard play)?");
    int gameTime = getValidInt();
    printf("Enter \'1\' to re-print the board, \'2\' to print current score, \'3\' to view time left, \'4\' to end early.\n");
    printf("Loading...\n");
    printLoadingBar(gameBoard);
    findAllWords(gameBoard, myDict, currGame); //finds all valid words on boggle board
    fillValidWords(currGame, myDict);
    printf("]\n");
    int playerScore[players];
    for(int currPlayerNum = 1; currPlayerNum < players+1; currPlayerNum++) //1 based indexed
    {
        for(int i=0; i< currGame->numValidWords; i++) //resets what words have been guessed
        {
            currGame->beenGuessed[i] = false;
        }
        printf("Hit enter to start Boggle play for player %d", currPlayerNum);
        readLine(stdin);
        printBoard(gameBoard);
        currGame->score = 0;
        double timeElapsed = 0.0;
        time_t start = time(NULL); //stores time index for start time
        char* userInput;
        bool guessedCorrect;
        while(timeElapsed<gameTime)
        {
            userInput = readLine(stdin);
            guessedCorrect = false;
            timeElapsed = (double)(time(NULL) - start); //finds time passed since guessting started
            if(timeElapsed > gameTime)
            {
                break; //if out of time, don't let the guess go through
            }
            if(strncmp(userInput,"1",2) == 0)
                printBoard(gameBoard);
            else if(strncmp(userInput,"2",2) == 0)
                printScore(currGame);
            else if(strncmp(userInput,"3",2) == 0)
                printf("Time left = %.2f seconds.\n", gameTime - timeElapsed);
            else if(strncmp(userInput,"4",2) == 0)
                break;
            else
            {
                for(int wordNum=0; wordNum<currGame->numValidWords; wordNum++) //search all valid boggle words on the board
                {
                    if((strncmp(userInput,currGame->validWordList[wordNum], 50) == 0))
                    {
                        if(currGame->beenGuessed[wordNum] == true )
                        {
                            guessedCorrect = true; //did guess a correct word
                            printf("You've already used word %s, no points", userInput);
                            break;
                        }
                        int points = findPoints(userInput);
                        printf("%s is a match! You receive %d points", userInput, points);
                        currGame->score += points;
                        currGame->beenGuessed[wordNum] = true;
                        guessedCorrect = true;
                        break;
                    }
                }
                if(guessedCorrect == false)
                {
                    printf("%s is NOT a match.", userInput);
                }
            }
        }
        playerScore[currPlayerNum] = currGame->score; //stores score of this player
        printf("Game over for player %d! You scored a total of %d\n",currPlayerNum, playerScore[currPlayerNum]);
        printf("The total possible SCORE was %d with %d words.\n", currGame->totalPossibleScore, currGame->numValidWords);
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
    int maxScore = 0;
    int winningPlayer = 0;
    for(int i=1; i<players+1; i++) //1 based indexing
    {
        printf("Player %d: %d\n", i, playerScore[i]);
        if(playerScore[i] > maxScore) //compares to see what the max score is and who had it
        {
            maxScore = playerScore[i];
            winningPlayer = i;
        }
    }
    bool isOneWinner = true;
    for(int i=1; i<players+1; i++) //1 based indexing
    {
        if(playerScore[i] == maxScore && i != winningPlayer) //tie for top score, sees if two people drew
        {
            isOneWinner = false;
        }
    }
    if (winningPlayer==0) //means no one won any points
    {
        printf("No one got any points! No winner\n");
    }
    else if(isOneWinner == false)
    {
        printf("Draw! No one gets a win\n");
    }
    else
    {
        printf("Player %d wins!\n", winningPlayer);
        currGame->multiPlayerScore[winningPlayer]++;
    }
    if(currGame->numValidWords>0) //if there were words to guess
    {
        if(currGame->score == currGame->totalPossibleScore)
            printf("Nice job! You guessed every possible word in the board\n");
        printf("Hit enter to see all valid words\n");
        readLine(stdin);
        for(int i =0; i<currGame->numValidWords; i++)
        {
            printf("Word %d: %s\n", i+1, currGame->validWordList[i]);
        }
    }
    bool toMenu = false; //return to menu if players done
    fflush(stdin);
    while(toMenu == false)
    {
        printf("Enter 1 to replay multi-player with same number of players, 2 to see current score of players,\nor anything else to exit multi-player and return to menu");
        char* buffer = readLine(stdin);
        if(strncmp(buffer,"1",2) == 0)
            multiPlayerGame(gameBoard, currGame, myDict, players); //restart multi-player, keeping stats
        else if(strncmp(buffer,"2",2) == 0)
        {
            for(int i=1; i<=players; i++)
            {
                printf("Number of wins for player %d: %d\n", i, currGame->multiPlayerScore[i]);
            }
        }
        else
            toMenu = true; //return to menu
    }
    menu(gameBoard, currGame, myDict);

}
void boggleSolver(struct board * gameBoard, struct game * currGame, struct dictionary * myDict)
{
    resetGame(currGame);
    for(int i =0; i<myDict->numWords; i++)
    {
        myDict->isFound[i] = false;
    }
    printf("Welcome to the boggle solver.\n");
    printf("Input your boggle board and we will find all the words possible that are also in your dictionary file\n");
    printf("What is the board size you want to solve?\n");
    getBoardInfo(gameBoard);
    buildBoard(gameBoard);
    char* buffer;
    char letter;
    for(int row = 0; row < gameBoard->rows; row++)
    {
        for(int col=0; col < gameBoard->cols; col++)
        {
            printf("What is the letter in row %d, col %d?", row, col);
            buffer = readLine(stdin);
            letter = buffer[0];
            gameBoard->cubes[row][col] = letter; //fills in the boggle board by looking at first charcter entered
        }
    }
    printf("Loading...\n");
    printLoadingBar(gameBoard);
    findAllWords(gameBoard, myDict, currGame);
    fillValidWords(currGame, myDict);
    printf("]\nFinished loading all answers for the following boggle board:\n");
    printBoard(gameBoard);
    printf("The total possible SCORE was %d with %d words.\n", currGame->totalPossibleScore, currGame->numValidWords);
    if(currGame->numValidWords>0)
    {
        printf("Hit enter to see all valid words");
        readLine(stdin);
        for(int i =0; i<currGame->numValidWords; i++)
        {
            printf("Word %d: %s\n", i+1, currGame->validWordList[i]);
        }
    }
    else
    {
        printf("No valid words in this board\n");
    }
}
void printLoadingBar(struct board * gameBoard) //prints empty loading bar
{
    printf("[");
    for(int i =0; i< gameBoard->rows * gameBoard->cols; i++)
    {
        printf("_");
    }
    printf("]\n[");
}
void printRules()
{
    printf("\nFind as many words as you can on the given board!\n");
    printf("You will have a time limit to find these words.\n");
    printf("The letters must form a chain - which can be connected horizontally, vertically, or diagonally from one another.\n");
    printf("Words must contain at least 3 letters.\n");
    printf("This program will allow words from the dictionary file \"dictionary.txt\".\n");
    printf("You cannot reuse letter cubes in a single word or reuse the same word.\n");
    printf("-------------------------------\n");
    printf("You will get more points for longer words as follows:\n");
    printf("3 Letters: 1 point\n4 Letters: 1 point\n5 Letters: 2 points\n6 Letters: 3 points\n7 Letters: 5 points\n8 or More Letters: 11 points\n");
    printf("-------------------------------\n");
    printf("Hit enter to return to main menu");
    readLine(stdin);
}
void printBoggleArt()
{
    printf(".__ .__..__ .__ .   .___\n");
    printf("[__)|  |[ __[ __|   [__ \n");
    printf("[__)|__|[_./[_./|___[___\n\n");
}
