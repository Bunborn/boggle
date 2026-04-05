---
name: Boggle C project testing status
description: Current test coverage, known bugs, and testing patterns for the C Boggle game
type: project
---

MVP test suite written (test_boggle.c in project root): 5 tests, 41 assertions, all passing.
Tests cover: trie lookup, board build/fill, findPoints scoring, findAllWords DFS, fillValidWords.

Known bugs found during testing:
- freeBoard() in board.c leaks row arrays (frees outer array but not per-row calloc'd arrays)
- fillValidWords() does shallow pointer copy from dictionary to game validWordList, creating fragile ownership

**Why:** These bugs weren't blocking test creation but affect memory safety in production use.
**How to apply:** If writing more tests, use custom free_board_full() helper. Watch for double-free if freeDictionary runs before freeGame.

Testing approach: in-memory dictionaries via make_tiny_dict() helper (no dictionary.txt dependency for tests). Hand-crafted boards via make_board() helper. No test framework -- plain C with PASS/FAIL printf.

Compile: gcc -o test_boggle test_boggle.c scanner.c board.c play.c words.c -I.
