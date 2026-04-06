# Boggle Test Suite

## Summary

Tests for the C Boggle game covering dictionary/trie operations, board construction,
scoring logic, the DFS word-finder, valid-word-list builder, game reset/init logic,
multiplayer score tracking, memory ownership verification, and freeBoard correctness.
The test file lives in the project root (not in Tests/) because it needs to compile
alongside the source files and link against them directly.

## Test Files

| File | Location | Description |
|------|----------|-------------|
| `test_boggle.c` | Project root | Full test suite: 10 tests, 84 assertions. Phase 1 (tests 1-5): trie lookup, board build/fill, findPoints scoring, findAllWords DFS on 2x2, fillValidWords population. Phase 2 (tests 6-10): resetGame correctness, buildGame allocation + multiplayer scores, fillValidWords ownership hazard, findAllWords 4x4 stress test, freeBoard + non-square board bug detection. |

## How to Run

```
gcc -o test_boggle test_boggle.c scanner.c board.c play.c words.c -I.
./test_boggle
```

No external test framework required. Output is PASS/FAIL per assertion with a
summary at the end. Return code 0 = all pass, 1 = at least one failure.

Note: does NOT require dictionary.txt -- all tests use in-memory dictionaries.

## Known Bugs Found by Tests

### CONFIRMED BUG: fillBoard() and printBoard() swap rows/cols (board.c)

`fillBoard()` uses `cols` as the outer loop index into `cubes[]`, but `buildBoard()`
allocates `rows` outer arrays. For non-square boards where `cols > rows`, this causes
out-of-bounds heap writes and segfaults. The same loop inversion exists in `printBoard()`.

The bug is masked in normal gameplay because `getBoardInfo()` forces `rows == cols`
(both set from a single dimension input). But the data structure supports non-square
boards, and any future code that sets different values will crash.

**File:** board.c, functions `fillBoard()` (line ~29) and `printBoard()` (line ~57)
**Fix:** Change `fillBoard` outer loop to iterate rows, inner loop to iterate cols.

### KNOWN BUG: fillValidWords() shallow pointer copy (play.c)

`fillValidWords()` overwrites `validWordList[i]` with a direct pointer to
`dictionary->words[j]`, leaking the original calloc'd buffer and creating
double-free / use-after-free hazards between freeGame() and freeDictionary().

**File:** play.c, function `fillValidWords()` (line ~108)
**Fix:** Use `strcpy()` instead of pointer assignment.

## Known Gaps and Concerns

- **No system/integration tests with human-delay simulation.** Requires stdin
  mocking or a test harness for timed input, which is not feasible with the
  current timedReadLine implementation (Windows console APIs).
- **timedReadLine is untestable** without a real Windows console (`_kbhit`/`_getch`).
- **No tests for printBoard, printScore, menu, or any UI output.** Requires
  stdout capture which is out of scope for this plain-C test harness.
- **Large board sizes (8x8+) with real dictionaries untested.** Could blow
  the stack or take unreasonable time.
- **multiPlayerScore is hardcoded to 10 slots.** No bounds checking if someone
  tries more than 10 players.
- **freeGame only frees numValidWords entries** from validWordList, leaking
  the remaining (numWords - numValidWords) calloc'd buffers.

## Last Updated

2026-04-05 (Phase 2)
