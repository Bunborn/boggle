# Boggle Test Suite

## Summary

Tests for the C Boggle game covering dictionary/trie operations, board construction,
scoring logic, the DFS word-finder, and the valid-word-list builder. The test file
lives in the project root (not in Tests/) because it needs to compile alongside the
source files and link against them directly.

## Test Files

| File | Location | Description |
|------|----------|-------------|
| `test_boggle.c` | Project root | MVP test suite: 5 tests, 41 assertions, no framework. Covers trie lookup, board build/fill, findPoints scoring, findAllWords DFS, and fillValidWords population. |

## How to Run

```
gcc -o test_boggle test_boggle.c scanner.c board.c play.c words.c -I.
./test_boggle
```

No external test framework required. Output is PASS/FAIL per assertion with a
summary at the end. Return code 0 = all pass, 1 = at least one failure.

Note: does NOT require dictionary.txt -- all tests use in-memory dictionaries.

## Known Gaps and Concerns

- **No system/integration tests with human-delay simulation yet.** The current 5 tests
  are unit/functional tests as requested. System tests simulating real player sessions
  would require stdin mocking or a test harness that can feed timed input.
- **freeBoard() leaks memory.** It frees the top-level `cubes` and `isVisited` arrays
  but not the individual row arrays allocated in `buildBoard`. Tests use a custom
  `free_board_full()` helper to avoid this. The bug is in board.c -- not our problem
  to fix, but worth knowing.
- **fillValidWords does a shallow copy** of word pointers from the dictionary into
  the game's validWordList. This means freeGame() can double-free if the dictionary
  is freed first. Ownership semantics are unclear and fragile.
- **timedReadLine is untestable** without a real Windows console (uses `_kbhit`/`_getch`).
- **No tests for printBoard, printScore, menu, or any UI output.** These require
  stdout capture which was out of scope.
- **Large board sizes (8x8+) untested.** The DFS could blow the stack or take
  unreasonable time on big boards with dense dictionaries.

## Last Updated

2026-04-05
