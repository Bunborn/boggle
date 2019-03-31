#build executable named boggle from boggle.c
bogglemake: main.c scanner.c board.c play.c words.c
	gcc -o bogglemake main.c scanner.c board.c play.c words.c -I.
