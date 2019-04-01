# Boggle
This readme is split into the following sections. A video ( https://youtu.be/6FsHfYGICFc ) demonstrates the game and basic setup.
### [Game overview](https://github.com/Bunborn/boggle/blob/master/README.md#game-overview-1)
### [Setup](https://github.com/Bunborn/boggle/blob/master/README.md#setup-1)
### [Code layout](https://github.com/Bunborn/boggle/blob/master/README.md#code-layout-1)
------------------------------------------------------------------------------------------------------------------------
# Game overview
C program that allows you to play boggle! In ascii format on Linux systems ( Tested on Ubuntu 18.04 LTS )
Features the following game modes -
* ### Single-player
  * Provide a game board size and time limit and then find as many words as you can!
  * Keeps track of your high score
  * See all words at the end of each round
* ### Multi-player
  * Provide a game board size and time limit as you compete against your friends to get the most points in Boggle!
  * Keeps track of each score and wins among players while in multi-player game mode while still tracking high score
  * See all words at the end of each multi-player round
* ### Boggle solver
  * Solve your own Boggle board!
  * Give the dimensions and letters of your Boggle board into the Boggle solver
  * Boggle solver will give you every legal word on the board as defined in the dictionary
------------------------------------------------------------------------------------------------------------------------
# Setup
There is an extra step in setting up the game where you define what your dictionary of words are.

You can use any dictionary you like, however it should store legal words in lowercase characters.

This file should be called **dictionary.txt** and be in the same directory as the rest of the program. If not, the program will inform
you to add it. After moving a dictionary file into the boggle directory, simply type **make** into your terminal to produce a boggle executable. You can run this executable by typing **./boggle**

A recommended dictionary file can be found in most Linux systems under **/usr/share/dict/words** (If you cannot find it there, type in this command into your Ubuntu machine - *sudo find / -name "words" -print* )

Move this words file into your boggle directory if it is not in /user/share/dict and then run the following command to shorten the word list of illegal charcters. You don't have to, but it is recommended! If you move the file into the boggle directory, just remove the /user/share/dict/ from the command!

**grep -v "'" /usr/share/dict/words >> dictionary.txt**
 


------------------------------------------------------------------------------------------------------------------------
# Code layout
The C program is split up into several files-
* Main.c
  * Main file that creates structs from play.h, words.h, board.h and uses them during the game runtime.
  * Has menu that calls out functions for the 3 game modes and has options to see rules or stats
  * Begins program and necessary memory allocation
* scanner.c
  * Borrowed from Dr. Lusth at The University of Alabama and modified by me
  * Has associated header scanner.h
  * Used for reading input from the user or the dictionary file
* play.c
  * Has associated header play.h
  * Houses game struct which tracks user scores and other gameplay elements
  * Contains the DFS optimized algorithm to search for all possible words on the board
* words.c
  * Has associated header words.h
  * Houses dictionary struct that stores dictionary file and associated data
  * Has features for string comparisons
* board.c
  * Has associated header board.h
  * Houses board struct to track the boggle board letters and associated data
  * Has features for making a random board and how to print it




