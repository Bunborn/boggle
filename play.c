//
// Created by Brandon on 3/25/2019.
//

#include "play.h"

// All possible movements for DFS in X or Y direction
int row[] = { -1, -1, -1, 0, 1, 0, 1, 1 };
int col[] = { -1, 1, 0, -1, -1, 1, 0, 1 };

void findAllWords(struct board *gameBoard, struct dictionary *myDict, struct game *currGame) //DFS algo to find all legal words on the board
{
    char* path = malloc(sizeof(char) * 126); //stores current path
    setVisitedFlagsFalse(gameBoard); //sets all flags as not visited yet if carried over from previous game
    for(int i = 0; i < gameBoard->rows; i++)
    {
        for(int j = 0; j < gameBoard->cols; j++) //searches every letter cube on the board (FIXED: was gameBoard->rows)
        {
            printf("#"); //updates loading bar
            fflush(stdout);
            path[0] = '\0'; //zero out string
            search(gameBoard, myDict, currGame, i, j, myDict->root, path);
        }
    }
    free(path);
}

void search(struct board *gameBoard, struct dictionary *myDict, struct game *currGame, int i, int j, struct TrieNode *node, char* path)
{
    // Check if this letter continues a valid prefix in the trie
    char letter = tolower((unsigned char)gameBoard->cubes[i][j]);
    int idx = letter - 'a';
    if(idx < 0 || idx >= 26) return;

    struct TrieNode *nextNode = node->children[idx];
    if(nextNode == NULL) return; // no words with this prefix, prune entire branch

    int pathLength = strLength(path);
    if(pathLength > 49) return;

    gameBoard->isVisited[i][j] = true;
    path[pathLength] = letter;
    path[pathLength + 1] = '\0';

    if(nextNode->is_word && pathLength + 1 >= 3 && myDict->isFound[nextNode->word_index] == false)
    {
        myDict->isFound[nextNode->word_index] = true;
        currGame->numValidWords++;
        currGame->totalPossibleScore += findPoints(path);
    }

    for(int dir = 0; dir < 8; dir++) //try all directions possible around the cube
    {
        if(isAllowed(i + row[dir], j + col[dir], gameBoard))
        {
            search(gameBoard, myDict, currGame, i + row[dir], j + col[dir], nextNode, path);
        }
    }

    path[pathLength] = '\0'; //pop this letter from the path
    gameBoard->isVisited[i][j] = false;
}

int findPoints(char* string) //finds how many points a word is worth
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
void buildGame(struct game *currGame, struct dictionary *myDict) //allocates memory
{
    currGame->beenGuessed = malloc(sizeof(int) * myDict->numWords);
    currGame->multiPlayerScore = malloc(sizeof(int) * 10);
    currGame->validWordList = (char**) calloc((unsigned)myDict->numWords, sizeof(char*));
    for ( int i = 0; i < myDict->numWords; i++ )
    {
        currGame->validWordList[i] = (char*) calloc(40, sizeof(char));
        currGame->beenGuessed[i] = false;
    }
    currGame->highScore = 0;
    resetGame(currGame);
}
void printScore(struct game *currGame)
{
    printf("Player score is: %d\n", currGame->score);
    if(currGame->score == 0)
    {
        printf("Go get some points!\n");
    }
}
void fillValidWords(struct game *currGame, struct dictionary *myDictionary) //fill valid word list with words found on boggle board!
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
bool isAllowed(int row, int col, struct board *gameBoard) //checks if this is a legal direction DFS can move in
{
    return ((row>= 0 && row < gameBoard->rows) && (col >= 0 && col < gameBoard->cols) && gameBoard->isVisited[row][col] == false); //returns false if outside the board or already visited
}
void setVisitedFlagsFalse(struct board *gameBoard) //sets all isVisited flag for letter cubes as false for next optimized DFS algo
{
    for(int i = 0; i < gameBoard->rows; i++)
    {
        for (int j = 0; j < gameBoard->cols; j++) {
            gameBoard->isVisited[i][j] = false;
        }
    }
}
void resetGame(struct game * currGame) //resets some game stats
{
    currGame->numValidWords = 0;
    currGame->score = 0;
    currGame->totalPossibleScore = 0;
}
void resetMultiPlayerGame(struct game * currGame) //resets multi-player wins
{
    for(int i=0; i< 10; i++)
    {
        currGame->multiPlayerScore[i] = 0;
    }
}
void freeGame(struct game *currGame) //free memory
{
    for ( int i = 0; i < currGame->numValidWords; i++ )
    {
        free(currGame->validWordList[i]);
    }
    free(currGame->beenGuessed);
    free(currGame->validWordList);
    free(currGame->multiPlayerScore);
    free(currGame);
}

