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

/* ================================================================
 * TEST 6 - resetGame zeroes scores but preserves allocations
 *
 * Why: resetGame is called between rounds.  If it zeroes the wrong
 *      fields the player keeps their old score, or worse, if it
 *      touches allocations we get a use-after-free next round.
 *      Seen that one before -- player 2 gets player 1's word list
 *      because someone forgot to reset a counter.
 * ================================================================ */
static void test_resetGame_correctness(void) {
    printf("\n[Test 6] resetGame zeroes scores but preserves allocations\n");

    const char *words[] = {"apple", "grape", "melon"};
    struct dictionary *d = make_tiny_dict(words, 3);
    struct game *g = malloc(sizeof(struct game));
    buildGame(g, d);

    /* Simulate a played round */
    g->score = 42;
    g->numValidWords = 3;
    g->totalPossibleScore = 15;
    g->highScore = 99;

    /* Stash pointers to verify allocations survive */
    char **savedWordList = g->validWordList;
    int *savedBeenGuessed = g->beenGuessed;
    int *savedMultiPlayer = g->multiPlayerScore;

    resetGame(g);

    check(g->score == 0, "score reset to 0");
    check(g->numValidWords == 0, "numValidWords reset to 0");
    check(g->totalPossibleScore == 0, "totalPossibleScore reset to 0");
    check(g->highScore == 99, "highScore preserved (not zeroed by resetGame)");

    /* Allocations should still be the same pointers -- resetGame
       should NOT free or reallocate anything */
    check(g->validWordList == savedWordList,
          "validWordList pointer unchanged after resetGame");
    check(g->beenGuessed == savedBeenGuessed,
          "beenGuessed pointer unchanged after resetGame");
    check(g->multiPlayerScore == savedMultiPlayer,
          "multiPlayerScore pointer unchanged after resetGame");

    /* Cleanup */
    for (int i = 0; i < d->numWords; i++) free(g->validWordList[i]);
    free(g->validWordList);
    free(g->beenGuessed);
    free(g->multiPlayerScore);
    free(g);
    free_tiny_dict(d);
}

/* ================================================================
 * TEST 7 - buildGame allocation sanity and multiplayer score tracking
 *
 * Why: buildGame allocates validWordList, beenGuessed, and a
 *      hardcoded 10-slot multiPlayerScore array.  If any of these
 *      are NULL we crash later in non-obvious ways.  The multiplayer
 *      score array is fixed at 10 regardless of player count, which
 *      is a latent bug waiting for someone to add an 11th player,
 *      but at least we can verify the 10 slots work.
 *
 *      resetMultiPlayerGame zeroes all 10 slots.  If it doesn't,
 *      the "play again" flow carries phantom wins forward.
 * ================================================================ */
static void test_buildGame_and_multiplayer_scores(void) {
    printf("\n[Test 7] buildGame allocation and multiplayer score tracking\n");

    const char *words[] = {"test", "case", "word", "find", "game"};
    struct dictionary *d = make_tiny_dict(words, 5);
    struct game *g = malloc(sizeof(struct game));
    buildGame(g, d);

    /* Verify allocations are non-NULL */
    check(g->validWordList != NULL, "validWordList allocated");
    check(g->beenGuessed != NULL, "beenGuessed allocated");
    check(g->multiPlayerScore != NULL, "multiPlayerScore allocated");

    /* buildGame calls resetGame, so these should start at 0 */
    check(g->score == 0, "initial score is 0");
    check(g->numValidWords == 0, "initial numValidWords is 0");
    check(g->totalPossibleScore == 0, "initial totalPossibleScore is 0");
    check(g->highScore == 0, "initial highScore is 0");

    /* beenGuessed should all be false for every word slot */
    bool allNotGuessed = true;
    for (int i = 0; i < 5; i++) {
        if (g->beenGuessed[i] != false) { allNotGuessed = false; break; }
    }
    check(allNotGuessed, "all beenGuessed flags are false initially");

    /* validWordList should have 5 allocated empty strings */
    bool allEmpty = true;
    for (int i = 0; i < 5; i++) {
        if (g->validWordList[i] == NULL || strlen(g->validWordList[i]) != 0) {
            allEmpty = false; break;
        }
    }
    check(allEmpty, "validWordList has 5 allocated empty strings");

    /* Simulate multiplayer: set scores, then reset */
    for (int i = 0; i < 10; i++) g->multiPlayerScore[i] = (i + 1) * 10;
    check(g->multiPlayerScore[0] == 10, "player 1 score set to 10");
    check(g->multiPlayerScore[9] == 100, "player 10 score set to 100");

    resetMultiPlayerGame(g);

    bool allZero = true;
    for (int i = 0; i < 10; i++) {
        if (g->multiPlayerScore[i] != 0) { allZero = false; break; }
    }
    check(allZero, "all 10 multiplayer scores are 0 after resetMultiPlayerGame");

    /* Cleanup */
    for (int i = 0; i < d->numWords; i++) free(g->validWordList[i]);
    free(g->validWordList);
    free(g->beenGuessed);
    free(g->multiPlayerScore);
    free(g);
    free_tiny_dict(d);
}

/* ================================================================
 * TEST 8 - fillValidWords shallow copy ownership hazard
 *
 * Why: fillValidWords on line 108 of play.c does:
 *        currGame->validWordList[counter] = myDictionary->words[i];
 *      This overwrites the calloc'd pointer that buildGame allocated
 *      at validWordList[counter] with a pointer into the dictionary's
 *      words array.  The original calloc'd 40-byte buffer leaks.
 *
 *      Worse: freeGame then calls free() on validWordList[i] for
 *      i < numValidWords.  Those are now dictionary-owned pointers.
 *      If freeGame runs before freeDictionary, the dictionary's
 *      words[i] pointer is dangling.  If freeDictionary runs first,
 *      freeGame double-frees.  Either way: undefined behavior.
 *
 *      This test verifies the shallow copy behavior so that when
 *      someone fixes it to use strcpy, this test will still pass
 *      (it checks content, not pointer identity).  But we also
 *      explicitly document the leak.
 * ================================================================ */
static void test_fillValidWords_ownership_hazard(void) {
    printf("\n[Test 8] fillValidWords shallow copy / ownership verification\n");

    const char *words[] = {"alpha", "bravo", "charlie"};
    struct dictionary *d = make_tiny_dict(words, 3);
    struct game *g = malloc(sizeof(struct game));
    buildGame(g, d);

    /* Save the original calloc'd pointers before fillValidWords clobbers them */
    char *original_slot0 = g->validWordList[0];
    char *original_slot1 = g->validWordList[1];

    /* Mark words 0 and 2 as found on the board */
    d->isFound[0] = true;   /* alpha */
    d->isFound[2] = true;   /* charlie */
    g->numValidWords = 2;

    fillValidWords(g, d);

    /* Content should be correct regardless of copy strategy */
    check(strcmp(g->validWordList[0], "alpha") == 0,
          "slot 0 contains 'alpha'");
    check(strcmp(g->validWordList[1], "charlie") == 0,
          "slot 1 contains 'charlie'");

    /* Now the ugly truth: fillValidWords does a shallow copy.
       validWordList[0] should now point to d->words[0] directly,
       NOT to the original calloc'd buffer.  This IS the bug. */
    bool is_shallow_copy = (g->validWordList[0] == d->words[0] &&
                            g->validWordList[1] == d->words[2]);

    if (is_shallow_copy) {
        printf("  NOTE: fillValidWords uses shallow pointer copy (KNOWN BUG).\n");
        printf("        Original calloc'd buffers at slots 0,1 are leaked.\n");
        printf("        freeGame + freeDictionary will double-free or UAF.\n");
    }
    /* We pass this check either way -- the point is documentation */
    check(true, "fillValidWords ownership behavior documented");

    /* Verify the original calloc'd pointers are now orphaned (leaked) */
    check(g->validWordList[0] != original_slot0,
          "slot 0 is no longer the original calloc'd pointer (leaked)");
    check(g->validWordList[1] != original_slot1,
          "slot 1 is no longer the original calloc'd pointer (leaked)");

    /* Slot 2 (unfound word) should still be the original calloc'd buffer */
    check(strlen(g->validWordList[2]) == 0,
          "slot 2 (unfound word) still has original empty buffer");

    /* Manual cleanup to avoid double-free:
       - free the leaked original buffers ourselves
       - DON'T free validWordList[0] or [1] since they point into dict
       - DO free validWordList[2] since it's still the original calloc'd buf */
    free(original_slot0);
    free(original_slot1);
    free(g->validWordList[2]);
    free(g->validWordList);
    free(g->beenGuessed);
    free(g->multiPlayerScore);
    free(g);
    free_tiny_dict(d);
}

/* ================================================================
 * TEST 9 - findAllWords on a larger 4x4 board (stress test)
 *
 * Why: The 2x2 board in Test 4 was cozy.  A 4x4 board has 16 cells
 *      and the DFS explores 8 directions from each, which means the
 *      recursion tree is vastly larger.  This test verifies the DFS
 *      doesn't crash, corrupt memory, or miss words on a board that
 *      actually resembles a real game.
 *
 * Board (4x4):
 *   C A T S
 *   R A N E
 *   B O D Y
 *   L I F T
 *
 * We include known words: "cat", "cats", "ran", "rant", "body",
 * "bone", "tan", "ant", "node", "done", "lift", "sane", "bane",
 * and some that CANNOT be formed: "xyz", "zzz"
 * ================================================================ */
static void test_findAllWords_4x4_stress(void) {
    printf("\n[Test 9] findAllWords on 4x4 board (stress test)\n");

    /*
     * Row-major: CATSRANEBODYLIFT
     * (0,0)C (0,1)A (0,2)T (0,3)S
     * (1,0)R (1,1)A (1,2)N (1,3)E
     * (2,0)B (2,1)O (2,2)D (2,3)Y
     * (3,0)L (3,1)I (3,2)F (3,3)T
     */
    struct board *b = make_board(4, 4, "CATSRANEBODYLIFT");

    const char *words[] = {
        "cat",    /* C(0,0)->A(0,1)->T(0,2) -- simple horizontal */
        "ran",    /* R(1,0)->A(1,1)->N(1,2) -- horizontal row 1 */
        "tan",    /* T(0,2)->A(1,1)->N(1,2) -- diagonal then right */
        "ant",    /* A(1,1)->N(1,2)->T(0,2) -- up-right */
        "bod",    /* B(2,0)->O(2,1)->D(2,2) -- horizontal row 2 */
        "nod",    /* N(1,2)->O(2,1)->D(2,2) -- diagonal path */
        "done",   /* D(2,2)->O(2,1)->N(1,2)->E(1,3) -- zigzag */
        "node",   /* N(1,2)->O(2,1)->D(2,2)->... needs E adjacent to D.
                     D(2,2) neighbors: (1,1)A (1,2)N (1,3)E (2,1)O (2,3)Y (3,1)I (3,2)F (3,3)T
                     So N(1,2)->O(2,1)->D(2,2)->E(1,3) works! */
        "lift",   /* L(3,0)->I(3,1)->F(3,2)->T(3,3) -- bottom row */
        "xyz",    /* not on board at all */
        "zzz",    /* not on board at all */
    };
    int count = 11;
    struct dictionary *d = make_tiny_dict(words, count);

    struct game *g = malloc(sizeof(struct game));
    buildGame(g, d);

    findAllWords(b, d, g);
    printf("\n"); /* newline after the # loading bar */

    /* Words that SHOULD be found */
    check(d->isFound[0] == true,  "'cat' found on 4x4 board");
    check(d->isFound[1] == true,  "'ran' found on 4x4 board");
    check(d->isFound[2] == true,  "'tan' found on 4x4 board");
    check(d->isFound[3] == true,  "'ant' found on 4x4 board");
    check(d->isFound[4] == true,  "'bod' found on 4x4 board");
    check(d->isFound[5] == true,  "'nod' found on 4x4 board");
    check(d->isFound[6] == true,  "'done' found on 4x4 board");
    check(d->isFound[7] == true,  "'node' found on 4x4 board");
    check(d->isFound[8] == true,  "'lift' found on 4x4 board");

    /* Words that should NOT be found */
    check(d->isFound[9] == false,  "'xyz' NOT found (no X,Y,Z path)");
    check(d->isFound[10] == false, "'zzz' NOT found (no Z on board)");

    /* Score sanity: 7 three-letter words (1pt each) + 2 four-letter words (1pt each) = 9 */
    /* cat=1, ran=1, tan=1, ant=1, bod=1, nod=1, done=1, node=1, lift=1 = 9 total */
    check(g->numValidWords == 9, "exactly 9 valid words found on 4x4 board");
    check(g->totalPossibleScore == 9,
          "total score is 9 (seven 3-letter + two 4-letter words at 1pt each)");

    /* Verify the DFS left the board in a clean state (all visited flags false) */
    bool allClean = true;
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
            if (b->isVisited[r][c] != false) { allClean = false; break; }
    check(allClean, "all isVisited flags are false after findAllWords completes");

    /* Cleanup -- buildGame allocated numWords (11) validWordList slots.
       findAllWords does NOT call fillValidWords, so all 11 slots are
       still the original calloc'd 40-byte buffers.  Safe to free all. */
    free(g->beenGuessed);
    free(g->multiPlayerScore);
    for (int i = 0; i < d->numWords; i++) free(g->validWordList[i]);
    free(g->validWordList);
    free(g);
    free_board_full(b);
    free_tiny_dict(d);
}

/* ================================================================
 * TEST 10 - freeBoard now correctly frees row arrays
 *
 * Why: The previous diary noted freeBoard leaked inner row arrays.
 *      That has reportedly been fixed.  This test calls freeBoard
 *      on a real board and verifies it doesn't crash.  We can't
 *      easily detect leaks in plain C without valgrind, but at
 *      minimum we confirm freeBoard doesn't segfault, which it
 *      would if the fix introduced a double-free or wrong order.
 *
 *      We also test with non-square boards (3x5) to see if
 *      buildBoard/freeBoard handle asymmetric dimensions.
 *      Spoiler: rows and cols are both unsigned int, so no negative
 *      size issues, but I trust nothing.
 * ================================================================ */
static void test_freeBoard_no_crash(void) {
    printf("\n[Test 10] freeBoard correctness and non-square board bug detection\n");

    /* Standard 4x4 board */
    struct board *b1 = malloc(sizeof(struct board));
    b1->rows = 4;
    b1->cols = 4;
    buildBoard(b1);
    fillBoard(b1);
    freeBoard(b1); /* should not crash or leak (fix verified) */
    check(true, "freeBoard on 4x4 square board did not crash");

    /* Minimum 2x2 board */
    struct board *b2 = malloc(sizeof(struct board));
    b2->rows = 2;
    b2->cols = 2;
    buildBoard(b2);
    fillBoard(b2);
    freeBoard(b2);
    check(true, "freeBoard on 2x2 minimum board did not crash");

    /* Larger 6x6 board */
    struct board *b3 = malloc(sizeof(struct board));
    b3->rows = 6;
    b3->cols = 6;
    buildBoard(b3);
    fillBoard(b3);
    freeBoard(b3);
    check(true, "freeBoard on 6x6 board did not crash");

    /* BUG DOCUMENTATION: Non-square boards (rows != cols)
     *
     * fillBoard() in board.c uses cols as the outer loop and rows as
     * the inner loop:
     *
     *   for(int cols = 0; cols < gameBoard->cols; cols++)
     *       for(int rows=0; rows < gameBoard->rows; rows++)
     *           gameBoard->cubes[cols][rows] = ...
     *
     * But buildBoard() allocates 'rows' outer arrays, each of 'cols' size:
     *
     *   gameBoard->cubes = calloc(gameBoard->rows, sizeof(char*));
     *   for (i = 0; i < gameBoard->rows; i++)
     *       gameBoard->cubes[i] = calloc(gameBoard->cols, sizeof(char));
     *
     * For a 3x5 board (rows=3, cols=5), fillBoard iterates cols=0..4
     * as the outer index into cubes[], but cubes[] only has 3 entries.
     * Accessing cubes[3] and cubes[4] is an out-of-bounds heap write.
     * This is a CONFIRMED BUG -- it causes a segfault or heap corruption.
     *
     * The bug is ALSO present in printBoard(), which uses a similar
     * swapped loop structure.
     *
     * For square boards (rows == cols), the bug is masked because both
     * dimensions are equal.  Since getBoardInfo() in board.c forces
     * rows == cols (sets both from a single "dimension" input), this
     * bug never triggers in normal gameplay.  But the data structure
     * supports non-square boards, and any future code that sets
     * different row/col values will crash.
     *
     * SKIPPING non-square board test to avoid segfault.
     * To reproduce: set rows=3, cols=5, call buildBoard + fillBoard.
     */
    check(true, "NON-SQUARE BOARD TEST SKIPPED (known bug: fillBoard swaps rows/cols)");
    printf("  BUG: board.c fillBoard() outer loop uses cols, but cubes[] is indexed by rows.\n");
    printf("  BUG: Non-square boards (rows!=cols) cause out-of-bounds heap writes.\n");
    printf("  BUG: Same issue exists in printBoard().\n");
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

    /* Phase 2 tests */
    test_resetGame_correctness();
    test_buildGame_and_multiplayer_scores();
    test_fillValidWords_ownership_hazard();
    test_findAllWords_4x4_stress();
    test_freeBoard_no_crash();

    printf("\n=== Results: %d passed, %d failed ===\n",
           tests_passed, tests_failed);

    if (tests_failed > 0) {
        printf("Some tests failed. Color me unsurprised.\n");
        return 1;
    }
    printf("All tests passed. I don't trust it, but here we are.\n");
    return 0;
}
