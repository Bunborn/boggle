/*
 * Benchmark for the Boggle word search algorithm.
 * Creates a fixed 10x10 board (seed=42) and times findAllWords.
 * Run before and after optimizations to compare performance.
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "board.h"
#include "scanner.h"
#include "words.h"
#include "play.h"

int main() {
    srand(42); // fixed seed for reproducibility

    FILE *fptr = fopen("dictionary.txt", "r");
    if (fptr == NULL) {
        printf("Cannot open dictionary.txt\n");
        return 1;
    }

    struct dictionary *myDict = malloc(sizeof(struct dictionary));
    myDict->dictionaryPtr = fptr;
    countDictionaryWords(myDict);
    buildDictionary(myDict);
    readDictionaryFile(myDict);
    buildTrie(myDict);

    struct game *currGame = malloc(sizeof(struct game));
    buildGame(currGame, myDict);

    struct board *gameBoard = malloc(sizeof(struct board));
    gameBoard->rows = 10;
    gameBoard->cols = 10;
    buildBoard(gameBoard);
    fillBoard(gameBoard);

    printf("Board:\n");
    printBoard(gameBoard);

    for (int i = 0; i < myDict->numWords; i++) myDict->isFound[i] = false;
    resetGame(currGame);

    // Print loading bar placeholder (findAllWords prints '#' per cell)
    printf("[");
    for (int i = 0; i < gameBoard->rows * gameBoard->cols; i++) printf("_");
    printf("]\n[");
    fflush(stdout);

    clock_t start = clock();
    findAllWords(gameBoard, myDict, currGame);
    clock_t end = clock();

    double ms = (double)(end - start) / CLOCKS_PER_SEC * 1000.0;

    printf("]\n");
    printf("-----------------------------\n");
    printf("Time:        %.2f ms\n", ms);
    printf("Words found: %d\n", currGame->numValidWords);
    printf("Max score:   %d\n", currGame->totalPossibleScore);
    printf("-----------------------------\n");

    freeBoard(gameBoard);
    freeDictionary(myDict);
    freeGame(currGame);
    return 0;
}
