#build executable named boggle from main.c and support files
boggle: main.c scanner.c board.c play.c words.c
	gcc -o boggle main.c scanner.c board.c play.c words.c -I.

#benchmark binary - run before and after trie changes to compare performance
benchmark: benchmark.c scanner.c board.c play.c words.c
	gcc -O2 -o benchmark benchmark.c scanner.c board.c play.c words.c -I.
