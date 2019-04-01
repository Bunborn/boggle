#build executable named boggle from main.c and support files
boggle: main.c scanner.c board.c play.c words.c
	gcc -o boggle main.c scanner.c board.c play.c words.c -I.
