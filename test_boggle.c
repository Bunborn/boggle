/*
 * test_boggle.c - MVP test suite for the Boggle game
 *
 * Written by a weary test engineer who has seen too many "simple" games
 * ship with broken dictionary lookups and off-by-one board traversals.
 *
 * Compile:
 *   gcc -o test_boggle test_boggle.c scanner.c board.c play.c words.c -I.
 *
 * Run:
 *   ./test_boggle
 *
 * Requires dictionary.txt in the working directory.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "board.h"
#include "words.h"
#include "play.h"
#include "scanner.h"

/* ---- Helpers ---- */

static int tests_passed = 0;
static int tests_failed = 0;

static void check(bool condition, const char *name) {
    if (condition) {
        printf("  PASS: %s\n", name);
        tests_passed++;
    } else {
        printf("  FAIL: %s\n", name);
        tests_failed++;
    }
}

/*
 * Build a small dictionary from an in-memory word list instead of
 * reading dictionary.txt.  This gives us deterministic, fast tests
 * that don't depend on the contents of the big file.
 */
static struct dictionary *make_tiny_dict(const char **wordList, int count) {
    struct dictionary *d = malloc(sizeof(struct dictionary));
    d->numWords = count;
    d->dictionaryPtr = NULL;
    d->isFound = malloc(sizeof(int) * count);
    d->words = calloc(count, sizeof(char *));
    for (int i = 0; i < count; i++) {
        d->words[i] = calloc(40, sizeof(char));
        strcpy(d->words[i], wordList[i]);
        d->isFound[i] = false;
    }
    /* Build the trie over these words */
    buildTrie(d);
    return d;
}

static void free_tiny_dict(struct dictionary *d) {
    for (int i = 0; i < d->numWords; i++) free(d->words[i]);
    free(d->words);
    free(d->isFound);
    freeTrie(d->root);
    free(d);
}

/*
 * Build a board with caller-supplied letters so we know exactly
 * what words are (or aren't) present.
 */
static struct board *make_board(int rows, int cols, const char *letters) {
    struct board *b = malloc(sizeof(struct board));
    b->rows = rows;
    b->cols = cols;
    buildBoard(b);
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            b->cubes[r][c] = letters[r * cols + c];
    setVisitedFlagsFalse(b);
    return b;
}

/* Free board AND its row arrays (the real freeBoard leaks them -- noted) */
static void free_board_full(struct board *b) {
    for (int i = 0; i < (int)b->rows; i++) {
        free(b->cubes[i]);
        free(b->isVisited[i]);
    }
    free(b->cubes);
    free(b->isVisited);
    free(b);
}

/* ================================================================
 * TEST 1 - Dictionary loading and trie lookup
 *
 * Why: The trie is the backbone of the word search.  If insertTrie
 *      or buildTrie silently drops words or mis-indexes them the
 *      whole game is broken and nobody will notice until a player
 *      complains that "cat" isn't a word.
 * ================================================================ */
static void test_dictionary_trie_lookup(void) {
    printf("\n[Test 1] Dictionary loading and trie lookup\n");

    const char *words[] = {"cat", "car", "card", "care", "bat", "ab"};
    int count = 6;
    struct dictionary *d = make_tiny_dict(words, count);

    /* ab is only 2 chars -- buildTrie skips words shorter than 3 */
    check(d->root != NULL, "trie root is not NULL");

    /* Walk the trie manually for "cat" */
    struct TrieNode *n = d->root;
    n = n->children['c' - 'a'];
    check(n != NULL, "trie has 'c' branch");
    n = n->children['a' - 'a'];
    check(n != NULL, "trie has 'ca' branch");
    struct TrieNode *t_node = n->children['t' - 'a'];
    check(t_node != NULL, "trie has 'cat' node");
    check(t_node->is_word == true, "'cat' is marked as a word");
    check(t_node->word_index == 0, "'cat' word_index is 0");

    /* "car" should also be present */
    struct TrieNode *r_node = n->children['r' - 'a'];
    check(r_node != NULL && r_node->is_word, "'car' is a word in trie");

    /* "card" extends "car" */
    struct TrieNode *d_node = r_node->children['d' - 'a'];
    check(d_node != NULL && d_node->is_word, "'card' is a word in trie");

    /* "ab" should NOT be in the trie (too short) */
    struct TrieNode *a_node = d->root->children['a' - 'a'];
    if (a_node != NULL) {
        struct TrieNode *b_node = a_node->children['b' - 'a'];
        check(b_node == NULL || b_node->is_word == false,
              "'ab' (2 chars) is NOT in trie");
    } else {
        check(true, "'ab' (2 chars) is NOT in trie (no 'a' branch at all)");
    }

    /* Non-existent word "caz" */
    struct TrieNode *z_node = d->root->children['c' - 'a']
                                     ->children['a' - 'a']
                                     ->children['z' - 'a'];
    check(z_node == NULL, "'caz' does not exist in trie");

    free_tiny_dict(d);
}

/* ================================================================
 * TEST 2 - Board building and filling
 *
 * Why: If buildBoard mis-allocates or fillBoard writes outside
 *      bounds we get heap corruption that shows up three hours
 *      later as an impossible crash.  Ask me how I know.
 * ================================================================ */
static void test_board_build_and_fill(void) {
    printf("\n[Test 2] Board building and filling\n");

    struct board *b = malloc(sizeof(struct board));
    b->rows = 4;
    b->cols = 4;
    buildBoard(b);
    fillBoard(b);

    check(b->cubes != NULL, "cubes array allocated");
    check(b->isVisited != NULL, "isVisited array allocated");

    /* Every cube should be an uppercase letter A-Z */
    bool allValid = true;
    bool allNotVisited = true;
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            char ch = b->cubes[r][c];
            if (ch < 'A' || ch > 'Z') allValid = false;
            if (b->isVisited[r][c] != false) allNotVisited = false;
        }
    }
    check(allValid, "all cubes are uppercase A-Z");
    check(allNotVisited, "all isVisited flags are false after fillBoard");

    /* Minimum board: 2x2 */
    struct board *small = malloc(sizeof(struct board));
    small->rows = 2;
    small->cols = 2;
    buildBoard(small);
    fillBoard(small);
    check(small->cubes[0][0] >= 'A' && small->cubes[0][0] <= 'Z',
          "2x2 board first cell is valid");
    check(small->cubes[1][1] >= 'A' && small->cubes[1][1] <= 'Z',
          "2x2 board last cell is valid");

    /* Cleanup (doing it properly since freeBoard leaks row arrays) */
    free_board_full(b);
    free_board_full(small);
}

/* ================================================================
 * TEST 3 - findPoints scoring correctness
 *
 * Why: The scoring table is printed in the rules, so if it's wrong
 *      players WILL notice and they WILL be upset.  Especially the
 *      ones who found an 8-letter word and only got 5 points.
 * ================================================================ */
static void test_findPoints(void) {
    printf("\n[Test 3] findPoints scoring correctness\n");

    /* Boundary / edge cases for every scoring bracket */
    check(findPoints("ab") == 0,        "2-letter word = 0 points");
    check(findPoints("cat") == 1,       "3-letter word = 1 point");
    check(findPoints("cats") == 1,      "4-letter word = 1 point");
    check(findPoints("carts") == 2,     "5-letter word = 2 points");
    check(findPoints("carton") == 3,    "6-letter word = 3 points");
    check(findPoints("cartoon") == 5,   "7-letter word = 5 points");
    check(findPoints("cartoons") == 11, "8-letter word = 11 points");
    check(findPoints("education") == 11,"9-letter word = 11 points");
    check(findPoints("") == 0,          "empty string = 0 points");
    check(findPoints("a") == 0,         "1-letter word = 0 points");
}

/* ================================================================
 * TEST 4 - findAllWords on a hand-crafted board
 *
 * Why: This is THE core mechanic.  If the DFS misses valid words
 *      or counts phantom ones, the game is fundamentally broken.
 *      We set up a tiny board where we know exactly which words
 *      the dictionary contains and which paths exist.
 *
 * Board (2x2):        Dictionary:
 *   C A                 "cat", "act", "car", "arc", "cab", "tac"
 *   T B
 *
 * Adjacency on a 2x2: every cell touches every other cell.
 * So valid 3-letter paths include: C-A-T, A-C-T, C-A-B, A-C-B, etc.
 * "car" requires an 'r' -- not on board, so 0 matches for that.
 * "tac" = T(1,0)->A(0,1)->C(0,0) -- valid path, word in dict.
 * ================================================================ */
static void test_findAllWords_handcrafted(void) {
    printf("\n[Test 4] findAllWords on hand-crafted 2x2 board\n");

    /*
     * Board layout (row-major):
     *   row 0: C A
     *   row 1: T B
     */
    struct board *b = make_board(2, 2, "CATB");

    const char *words[] = {"cat", "act", "car", "arc", "cab", "tac", "bat", "tab"};
    int count = 8;
    struct dictionary *d = make_tiny_dict(words, count);

    struct game *g = malloc(sizeof(struct game));
    buildGame(g, d);

    findAllWords(b, d, g);

    /*
     * Expected findable words:
     *   "cat" : C(0,0)->A(0,1)->T(1,0) -- all adjacent, valid
     *   "act" : A(0,1)->C(0,0)->T(1,0) -- valid
     *   "cab" : C(0,0)->A(0,1)->B(1,1) -- valid
     *   "tac" : T(1,0)->A(0,1)->C(0,0) -- valid (reverse of "cat" path)
     *   "bat" : B(1,1)->A(0,1)->T(1,0) -- valid
     *   "tab" : T(1,0)->A(0,1)->B(1,1) -- valid
     *   "car" : needs 'r', not on board -- NOT found
     *   "arc" : needs 'r', not on board -- NOT found
     */
    check(g->numValidWords >= 1, "at least one word found");

    /* Check specific isFound flags */
    check(d->isFound[0] == true,  "'cat' found on board");
    check(d->isFound[1] == true,  "'act' found on board");
    check(d->isFound[2] == false, "'car' NOT found (no R on board)");
    check(d->isFound[3] == false, "'arc' NOT found (no R on board)");
    check(d->isFound[4] == true,  "'cab' found on board");
    check(d->isFound[5] == true,  "'tac' found on board");
    check(d->isFound[6] == true,  "'bat' found on board");
    check(d->isFound[7] == true,  "'tab' found on board");

    /* Score check: 6 three-letter words at 1 pt each = 6 */
    check(g->totalPossibleScore == 6, "total possible score is 6 (6 x 1pt)");
    check(g->numValidWords == 6, "exactly 6 valid words found");

    /* Cleanup -- don't use freeGame because validWordList entries
       point into the dictionary's words array (fillValidWords does
       a shallow copy). We free manually. */
    free(g->beenGuessed);
    free(g->multiPlayerScore);
    /* Free the validWordList array (but not the strings -- they belong to dict) */
    for (int i = 0; i < d->numWords; i++) free(g->validWordList[i]);
    free(g->validWordList);
    free(g);
    free_board_full(b);
    free_tiny_dict(d);
}

/* ================================================================
 * TEST 5 - fillValidWords populates the valid word list
 *
 * Why: fillValidWords bridges the dictionary's isFound flags to
 *      the game's validWordList, which is what the player actually
 *      sees.  If this is wrong, the solver lies to the player.
 *      I've seen that exact bug ship.  Twice.
 * ================================================================ */
static void test_fillValidWords(void) {
    printf("\n[Test 5] fillValidWords populates valid word list\n");

    const char *words[] = {"alpha", "bravo", "charlie", "delta", "echo"};
    int count = 5;
    struct dictionary *d = make_tiny_dict(words, count);

    /* Simulate that findAllWords marked words 1 and 3 as found */
    d->isFound[1] = true;  /* bravo */
    d->isFound[3] = true;  /* delta */

    struct game *g = malloc(sizeof(struct game));
    buildGame(g, d);
    g->numValidWords = 2; /* as findAllWords would have set */

    fillValidWords(g, d);

    check(strcmp(g->validWordList[0], "bravo") == 0,
          "validWordList[0] is 'bravo'");
    check(strcmp(g->validWordList[1], "delta") == 0,
          "validWordList[1] is 'delta'");

    /* When nothing is found, list should be empty */
    for (int i = 0; i < count; i++) d->isFound[i] = false;
    /* Reset the word list entries that got shallow-copied */
    /* Rebuild game to get clean validWordList */
    /* (just re-calloc the first two slots that got overwritten) */
    g->validWordList[0] = calloc(40, sizeof(char));
    g->validWordList[1] = calloc(40, sizeof(char));
    g->numValidWords = 0;
    fillValidWords(g, d);
    /* validWordList[0] should still be empty since nothing was found */
    check(strlen(g->validWordList[0]) == 0,
          "validWordList[0] is empty when no words found");

    /* Mark ALL words found and verify order is preserved */
    for (int i = 0; i < count; i++) d->isFound[i] = true;
    g->numValidWords = 5;
    fillValidWords(g, d);
    bool orderCorrect = true;
    for (int i = 0; i < count; i++) {
        if (strcmp(g->validWordList[i], words[i]) != 0) {
            orderCorrect = false;
            break;
        }
    }
    check(orderCorrect, "all 5 words in correct order when all found");

    /* Cleanup */
    free(g->beenGuessed);
    free(g->multiPlayerScore);
    /* Some validWordList entries are shallow copies, some are calloc'd.
       We only free the ones we calloc'd (indices that weren't overwritten
       by fillValidWords).  Actually at this point all 5 are shallow copies
       from the last fillValidWords call.  The two we calloc'd earlier were
       overwritten.  This is the kind of ownership nightmare that keeps me
       up at night. Just leak a few bytes in a test, it's fine. */
    free(g->validWordList);
    free(g);
    free_tiny_dict(d);
}

/* ================================================================ */

int main(void) {
    printf("=== Boggle Test Suite ===\n");
    printf("If everything passes I'll be genuinely surprised.\n");

    srand(42); /* deterministic for board tests */

    test_dictionary_trie_lookup();
    test_board_build_and_fill();
    test_findPoints();
    test_findAllWords_handcrafted();
    test_fillValidWords();

    printf("\n=== Results: %d passed, %d failed ===\n",
           tests_passed, tests_failed);

    if (tests_failed > 0) {
        printf("Some tests failed. Color me unsurprised.\n");
        return 1;
    }
    printf("All tests passed. I don't trust it, but here we are.\n");
    return 0;
}
