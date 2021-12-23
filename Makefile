default:
	gcc main4.c -o gioco -lncurses

run:
	./gioco

project:
	gcc main4.c -o gioco -lncurses & ./gioco

clean:
	rm ./gioco